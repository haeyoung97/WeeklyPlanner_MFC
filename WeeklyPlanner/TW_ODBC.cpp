
#include "stdafx.h"
#include "TW_ODBC.h"

#include "MainFrm.h"
#include "WeeklyPlannerView.h"
#include "HistoryView.h"

#pragma comment(lib, "odbc32.lib")


// 데이터를 가져오는 최대 단위를 설정한다.
#define MAX_COUNT 8
#define MAX_SOUND 50

// 서버탐색기를 이용하여 ODBC 연결하기
// 스키마 이름, 아이디, 패스워드, 일정내용명, DB칼럼명 (날짜, 내용) 기록
//#define DBSchemas L"WeeklyPlanner"
//#define DBID L"haey"
//#define DBPassword L""
//#define DBSchemas L"WeeklyPlanner"
//#define DBID L"root"
//#define DBPassword L"1234"
#define DBContents Memo
#define DBDdaytitle Title
#define DBSchemas L"wp_db"
#define DBID L"WP_User"
#define DBPassword L"tndtlfeo"
//#define DBContents Todolist
//#define DBDdaytitle Title

struct TodoList {
	wchar_t date[25];
	bool isDone;
	wchar_t DBContents[100];
};

struct DdayData {
	wchar_t date[11];
	wchar_t Title[100];
};

struct profile {
	wchar_t path[256];
	wchar_t message[100];
};

struct sound {
	wchar_t soundPath[256];
	wchar_t soundName[100];
};

TW_ODBC::TW_ODBC()
	: record_num(0)
	, m_dbDataCnt(0)
	, record_sound(0)
{
	// ODBC 드라이버에 연결을 위한 기본 정보를 설정한다.
	SQLSetEnvAttr(NULL, SQL_ATTR_CONNECTION_POOLING, (SQLPOINTER)SQL_CP_ONE_PER_DRIVER, SQL_IS_INTEGER);

	// ODBC 기술을 사용하기 위한 환경을 구성한다
	// 구성된 환경 정보에 대한 핸들 값은 mh_environment에 저장한다.
	if (SQL_ERROR != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &mh_environment)) {
		// 사용할 ODBC 버전 정보를 설정한다.
		SQLSetEnvAttr(mh_environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);
		SQLSetEnvAttr(mh_environment, SQL_ATTR_CP_MATCH, (SQLPOINTER)SQL_CP_RELAXED_MATCH, SQL_IS_INTEGER);

		// ODBC 함수를 사용하기 위한 정보를 구성한다.
		// 이 정보에 대한 핸들 값은 mh_odbc에 저장한다.
		if (SQL_ERROR != SQLAllocHandle(SQL_HANDLE_DBC, mh_environment, &mh_odbc)) {
			RETCODE ret_code = SQLConnect(mh_odbc,
				// 접속할 DSN 설정
				(SQLWCHAR *)DBSchemas, SQL_NTS,
				// 접속에 사용할 ID
				(SQLWCHAR *)DBID, SQL_NTS,
				// 접속에 사용할 Password
				(SQLWCHAR *)DBPassword, SQL_NTS);
			if (ret_code == SQL_SUCCESS || ret_code == SQL_SUCCESS_WITH_INFO) {
				// ODBC를 사용하여 데이터베이스 서버에 성공적으로 접속한 경우
				m_connect_flag = 1;
				AfxMessageBox(_T("데이터베이스 서버 접속 성공"));
			}
			else {
				// 접속에 실패한 경우, 구성했던 메모리를 제거한다.
				AfxMessageBox(_T("데이터베이스 서버 접속 실패"));

				if (mh_odbc != SQL_NULL_HDBC)
					SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
				if (mh_environment != SQL_NULL_HENV)
					SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
			}
		}
	}
}


TW_ODBC::~TW_ODBC()
{
	// 서버와 연결되어 있는 경우에만 제거한다. 서버에 연결을 실패했다면
	// 실패한 시점에서 아래의 정보가 정리되었기 때문이다.
	if (m_connect_flag == 1) {
		if (mh_odbc != SQL_NULL_HDBC)
			SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
		if (mh_environment != SQL_NULL_HENV)
			SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
	}
}


int TW_ODBC::ImportData(CString strToday, CString strTomorrow)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	int checkEnabled = 0;
	CString tmp;
	tmp.Format(L"SELECT * FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	wchar_t query_str_forDday[256] = L"SELECT * FROM dday";
	wchar_t query_str_forProfile[256] = L"SELECT * FROM profile";
	wchar_t query_str_forSound[256] = L"SELECT * FROM sound";

	// table 가져오기
	wchar_t* query_str = tmp.GetBuffer();

	// 읽어온 데이터의 상태를 기록할 변수
	unsigned short record_state[MAX_COUNT];
	unsigned short record_state_forDday;
	unsigned short record_state_forProfile[MAX_COUNT];
	unsigned short record_state_forSound[MAX_SOUND];


	// 읽어온 데이터를 저장할 변수
	TodoList raw_data[MAX_COUNT];
	DdayData raw_data_forDday;
	profile raw_data_forProfile[100];
	sound raw_data_forSound[MAX_SOUND];


	CString str;
	CString strProfile;
	CString strSound;

	//데이터를 저장할 배열을 초기화 한다.
	memset(raw_data, 0, sizeof(raw_data));
	memset(&raw_data_forDday, 0, sizeof(raw_data_forDday));
	memset(raw_data_forProfile, 0, sizeof(raw_data_forProfile));
	memset(raw_data_forSound, 0, sizeof(raw_data_forSound));

	HSTMT h_statement, h_statement_forDday, h_statement_forProfile, h_statement_forSound;
	RETCODE ret_code, ret_code_forDday, ret_code_forProfile, ret_code_forSound;
	// Query 문을 위한 메모리를 할당한다.
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement) && SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement_forDday) && SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement_forProfile) && SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement_forSound)) {
		record_num = 0;
		record_sound = 0;
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);


		// 가져온 데이터를 저장할 메모리의 크기를 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data), 0);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data_forDday), 0);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data_forProfile), 0);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data_forSound), 0);


		// 데이터를 가져올 때 동시성에 대한 방식을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);

		// 데이터를 가져오는 최대 단위를 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_SOUND, SQL_IS_UINTEGER);

		// 읽은 데이터의 상태를 저장할 변수의 주소를 전달한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_STATUS_PTR, record_state, 0);
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROW_STATUS_PTR, &record_state_forDday, 0);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROW_STATUS_PTR, record_state_forProfile, 0);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROW_STATUS_PTR, record_state_forSound, 0);

		// 읽은 데이터의 개수를 저장할 변수의 주소를 전달한다.
		SQLSetStmtAttr(h_statement_forDday, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);
		SQLSetStmtAttr(h_statement_forProfile, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);
		SQLSetStmtAttr(h_statement_forSound, SQL_ATTR_ROWS_FETCHED_PTR, &record_sound, 0);

		// 테이블에서 가져온 데이터를 속성별로 raw_data 변수에 저장하기 위해서
		// 속성별로 저장할 메모리 위치를 설정한다.

		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, raw_data[0].DBContents, sizeof(wchar_t) * 100, NULL);

		SQLBindCol(h_statement_forDday, 1, SQL_WCHAR, raw_data_forDday.date, sizeof(wchar_t) * 11, NULL);
		SQLBindCol(h_statement_forDday, 2, SQL_WCHAR, raw_data_forDday.Title, sizeof(wchar_t) * 20, NULL);
		
		SQLBindCol(h_statement_forProfile, 1, SQL_WCHAR, raw_data_forProfile[0].path, sizeof(wchar_t) * 80, NULL);
		SQLBindCol(h_statement_forProfile, 2, SQL_WCHAR, raw_data_forProfile[0].message, sizeof(wchar_t) * 20, NULL);

		SQLBindCol(h_statement_forSound, 1, SQL_WCHAR, raw_data_forSound[0].soundPath, sizeof(wchar_t) * 80, NULL);
		SQLBindCol(h_statement_forSound, 2, SQL_WCHAR, raw_data_forSound[0].soundName, sizeof(wchar_t) * 80, NULL);

		// SQL 명령문을 실행한다.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);
		ret_code_forDday = SQLExecDirect(h_statement_forDday, (SQLWCHAR*)query_str_forDday, SQL_NTS);
		ret_code_forProfile = SQLExecDirect(h_statement_forProfile, (SQLWCHAR*)query_str_forProfile, SQL_NTS);
		ret_code_forSound = SQLExecDirect(h_statement_forSound, (SQLWCHAR*)query_str_forSound, SQL_NTS);

		if (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// SQL 명령문의 실행 결과로 받은 데이터를 ListBox에 추가한다.
			ret_code = SQLFetchScroll(h_statement, SQL_FETCH_PREV, 0);
			while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

				// 데이터 개수만큼 반복하면서 작업한다.
				// 가져온 데이터가 삭제된 정보가 아니라면 해당 속성으로
				// 합쳐서 문자열로 구성하고 AfxMessageBox에 등록한다.
				if (record_state[0] != SQL_ROW_DELETED && record_state[0] != SQL_ROW_ERROR) {
					str.Format(L"%s, %d, %s", raw_data[0].date, raw_data[0].isDone, raw_data[0].DBContents);
					//데이터 불러온 후 체크리스트에 추가하기 (출력?)
					while (1) {
						if (pView->m_arrayTodoCheck[m_dbDataCnt]->IsWindowEnabled() == false) {
							break;
						}
						m_dbDataCnt++;
					}
					CString strContents;
					strContents.Format(_T("%s"), raw_data[0].DBContents);
					pView->m_arrayTodoCheck[m_dbDataCnt]->SetWindowText(strContents);

					if (raw_data[0].isDone == 1) {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(1);
						pView->m_bChecked[m_dbDataCnt] = false;
						pView->m_checkCnt++;
						checkEnabled++;
					}
					else if (raw_data[0].isDone == 0) {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(0);
						pView->m_bChecked[m_dbDataCnt] = false;
						checkEnabled++;
					}
					else {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(false);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(false);
						pView->m_bChecked[m_dbDataCnt] = true;
					}
				}
			}
		}
		else {
			DataSaveTodolist();
		}

		while (ret_code_forDday = SQLFetchScroll(h_statement_forDday, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

			CListCtrl* ctrlDdayList = &(pView->m_ctrlDdayList);

			for (unsigned int i = 0; i < record_num; i++) {
				if (record_state_forDday != SQL_ROW_DELETED && record_state_forDday != SQL_ROW_ERROR) {
					CTime nowTime, Ddate;
					CString str, strDday;
					CString stry, strm, strd;
					int year, month, day, index;

					AfxExtractSubString(stry, raw_data_forDday.date, 0, '-');
					AfxExtractSubString(strm, raw_data_forDday.date, 1, '-');
					AfxExtractSubString(strd, raw_data_forDday.date, 2, '-');
					year = _ttoi(stry);
					month = _ttoi(strm);
					day = _ttoi(strd);
					Ddate = CTime(year, month, day, 0, 0, 0, 0);
					str.Format(_T("%s-%s-%s"), stry, strm, strd);


					nowTime = CTime::GetCurrentTime();
					CTimeSpan dDay = Ddate - nowTime;

					//*******************
					int m_dDay = dDay.GetTotalSeconds();
					if (dDay > 0)
						strDday.Format(_T("D-%d"), abs(m_dDay / (24 * 60 * 60) + 1));
					else if (abs(m_dDay / (24 * 60 * 60)) == 0)
						strDday.Format(_T("D-Day"));
					else
						strDday.Format(_T("D+%d"), abs(m_dDay / (24 * 60 * 60)));
					pView->m_strNewDdayDate = strDday;



					index = ctrlDdayList->InsertItem(ctrlDdayList->GetItemCount(), str);
					ctrlDdayList->SetItem(index, 1, LVIF_TEXT, raw_data_forDday.Title, 0, 0, 0, NULL);
					ctrlDdayList->SetItem(index, 2, LVIF_TEXT, strDday, 0, 0, 0, NULL);


				}

			}
		}
		if (ret_code_forProfile = SQLFetchScroll(h_statement_forProfile, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// SQL 명령문의 실행 결과로 받은 데이터를 ListBox에 추가한다.
			ret_code_forProfile = SQLFetchScroll(h_statement_forProfile, SQL_FETCH_PREV, 0);
			while (ret_code_forProfile = SQLFetchScroll(h_statement_forProfile, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

				// 데이터 개수만큼 반복하면서 작업한다.
				// 가져온 데이터가 삭제된 정보가 아니라면 해당 속성으로
				// 합쳐서 문자열로 구성하고 AfxMessageBox에 등록한다.
				if (record_state_forProfile[0] != SQL_ROW_DELETED && record_state_forProfile[0] != SQL_ROW_ERROR) {
					strProfile.Format(L"%s, %s", raw_data_forProfile[0].path, raw_data_forProfile[0].message);

					//strProfile.Format(L"DELETE FROM weeklyplanner.profile WHERE path = '%s'", raw_data_forProfile[0].path);
					int i = 0;
					

						CString strMessage, strPath;
						strMessage.Format(_T("%s"), raw_data_forProfile[i].message);
						pView->m_EditMessage.SetWindowTextW(strMessage);

						strPath.Format(_T("%s"), raw_data_forProfile[i].path);
						//strPath.Replace(_T("\\"), _T("\\\\"));

						//AfxMessageBox(strPath);
						pView->m_strDefaultImagePath = strPath;
					
				}
			}
		}
		else {
			
			CString strPath = pView->m_strDefaultImagePath;
			CString strOld = pView->m_strOldPath;

			SaveProfilePath(strPath, strOld);
			
		}
		if (ret_code_forSound = SQLFetchScroll(h_statement_forSound, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			
			CString strPath, strName;
			ret_code_forSound = SQLFetchScroll(h_statement_forSound, SQL_FETCH_PREV, 0);

			int i = 0;
			//for (unsigned int i = 0; i < record_sound; i++) {
			while (ret_code_forSound = SQLFetchScroll(h_statement_forSound, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
				
				// 데이터 개수만큼 반복하면서 작업한다.
				// 가져온 데이터가 삭제된 정보가 아니라면 해당 속성으로
				// 합쳐서 문자열로 구성하고 AfxMessageBox에 등록한다.
				if (record_state_forSound[0] != SQL_ROW_DELETED && record_state_forSound[0] != SQL_ROW_ERROR) {
					strSound.Format(L"%s, %s", raw_data_forSound[0].soundPath, raw_data_forSound[0].soundName);
					
					//CString* str;

					strName.Format(_T("%s"), raw_data_forSound[0].soundName);

					pView->m_soundSP.m_strSoundName[i] = strName;
					strPath.Format(_T("%s"), raw_data_forSound[0].soundPath);
					pView->m_soundSP.m_strSoundPath[i] = strPath;
					i++;

				}

			}
			
			pView->m_soundSP.m_nSoundIndex = i;
		}

		else {

			for (int i = 0; i < 9; i++) {
				pView->m_soundSP.m_strSoundName[i] = pView->SoundName[i];
				pView->m_soundSP.m_strSoundPath[i] = pView->SoundPath[i];
				//ttmp.Format(_T("%s"), strName[i]);
				//AfxMessageBox(ttmp);
			}
			pView->m_soundSP.m_nSoundIndex = 9;
			DataSaveSound(pView->SoundPath, pView->SoundName);
		}

		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forDday);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forProfile);
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement_forSound);

	}

	return checkEnabled;
}


void TW_ODBC::DataSaveTodolist()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strContents;
	int result = 0;
	char isDone = NULL;
	for (int i = 0; i < 8; i++) {
		pView->m_arrayTodoCheck[i]->GetWindowText(strContents);
		boolean isEnabled = pView->m_arrayTodoCheck[i]->IsWindowEnabled();
		if (isEnabled) {
			if (pView->m_arrayTodoCheck[i]->GetCheck() == BST_UNCHECKED)
				isDone = 0;
			else if (pView->m_arrayTodoCheck[i]->GetCheck() == BST_CHECKED)
				isDone = 1;
		}
		else {
			isDone = 2;
		}
		
		str.Format(L"INSERT INTO todolist VALUES (default,'%d', '%s')", isDone, strContents);
		const wchar_t* tmp = str;
		if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
			// Query 문을 실행할 때 타임 아웃을 설정한다.
			SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

			// SQL 명령문을 실행한다.
			RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
			
			// 성공적으로 완료되었는지 체크한다.
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) result = 1;

			// 명령 수행이 완료되었다는 것을 설정한다.
			SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
			// Query 문을 위해 할당한 메모리를 해제한다.
			SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
		}
		isDone = NULL;
	}
	
}


void TW_ODBC::DataRemoveTodolist(CString strToday, CString strTomorrow)
{
	SQLHSTMT h_statement;
	CString str;
	int result = 0;

	str.Format(L"DELETE FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	const wchar_t* tmp = str;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
	
		// SQL 명령문을 실행한다.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);

		// 성공적으로 완료되었는지 체크한다.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			result = 1;
		// 명령 수행이 완료되었다는 것을 설정한다.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}


SQLHANDLE TW_ODBC::GetMh_Env()
{
	return mh_environment;
}


SQLHDBC TW_ODBC::GetMh_odbc()
{
	return mh_odbc;
}


void TW_ODBC::LoadHistoryTodolist(CString strToday, CString strTomorrow)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	CString strPercent;
	for (int i = 0; i < 8; i++)
		(pView->view).m_arrayHistoryCheck[i]->SetWindowText(_T(""));
	(pView->view).m_progressHistory.SetPos(0);
	strPercent.Format(_T("0"));
	(pView->view.m_historyPercent).SetWindowText(strPercent);

	int checkEnable = 0, writeList = 0, arrayCnt = 0;
	CString tmp, str;
	tmp.Format(L"SELECT * FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	
	// table 가져오기
	wchar_t* query_str = tmp.GetBuffer();

	// 읽어온 데이터의 상태를 기록할 변수
	unsigned short record_state[MAX_COUNT];

	// 읽어온 데이터를 저장할 변수
	TodoList raw_data[MAX_COUNT];

	//데이터를 저장할 배열을 초기화 한다.
	memset(raw_data, 0, sizeof(raw_data));

	HSTMT h_statement;	RETCODE ret_code;

	// Query 문을 위한 메모리를 할당한다.
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// 가져온 데이터를 저장할 메모리의 크기를 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_BIND_TYPE, (SQLPOINTER)sizeof(raw_data), 0);

		// 데이터를 가져올 때 동시성에 대한 방식을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_CURSOR_TYPE, (SQLPOINTER)SQL_CURSOR_KEYSET_DRIVEN, SQL_IS_UINTEGER);

		// 데이터를 가져오는 최대 단위를 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_NUMBER, (SQLPOINTER)MAX_COUNT, SQL_IS_UINTEGER);

		// 읽은 데이터의 상태를 저장할 변수의 주소를 전달한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROW_STATUS_PTR, record_state, 0);


		// 테이블에서 가져온 데이터를 속성별로 raw_data 변수에 저장하기 위해서
		// 속성별로 저장할 메모리 위치를 설정한다.

		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, &raw_data[0].DBContents, sizeof(wchar_t) * 100, NULL);

		// SQL 명령문을 실행한다.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);

		if (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// SQL 명령문의 실행 결과로 받은 데이터를 ListBox에 추가한다.
			ret_code = SQLFetchScroll(h_statement, SQL_FETCH_PREV, 0);
			while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {

				if (record_state[0] != SQL_ROW_DELETED && record_state[0] != SQL_ROW_ERROR) {
					str.Format(L"%s, %d, %s", raw_data[0].date, raw_data[0].isDone, raw_data[0].DBContents);
					//데이터 불러온 후 체크리스트에 추가하기 (출력?)
					
					CString strContents;

					if (raw_data[0].isDone == 1) {
						strContents.Format(_T("< Complete >  %s"), raw_data[0].DBContents);
						(pView->view).m_arrayHistoryCheck[arrayCnt]->SetWindowText(strContents);
						checkEnable++;
						writeList++;
					}
					else if (raw_data[0].isDone == 0) {
						strContents.Format(_T("< Proceeding >  %s"), raw_data[0].DBContents);
						(pView->view).m_arrayHistoryCheck[arrayCnt]->SetWindowText(strContents);
						writeList++;
					}
					else {
						strContents.Format(_T("< No Events >"));
						(pView->view).m_arrayHistoryCheck[arrayCnt]->SetWindowText(strContents);
					}
				}
				arrayCnt++;
			}
			if (writeList == 0 || checkEnable == 0) {
				(pView->view).m_progressHistory.SetPos(0);
				strPercent.Format(_T("0"));
				(pView->view.m_historyPercent).SetWindowText(strPercent);
			}
			else {
				if (writeList == checkEnable) {
					(pView->view).m_progressHistory.SetPos(1000);
					strPercent.Format(_T("100"));
					(pView->view.m_historyPercent).SetWindowText(strPercent);
				}
				else {
					int percent = 1000 / writeList;
					percent = 1000 / writeList;
					(pView->view).m_progressHistory.SetPos(checkEnable*percent);
					percent = 100 / writeList;
					strPercent.Format(_T("%d"), percent);
					(pView->view.m_historyPercent).SetWindowText(strPercent);
				}
			}

		}
		else {
			CString strContents;
			strContents.Format(_T("< There is no record of this day. >"), raw_data[0].DBContents);
			(pView->view).m_arrayHistoryCheck[0]->SetWindowText(strContents);
		}
	}
}


void TW_ODBC::SaveProfilePath(CString strPath, CString strOldPath)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strMessage;
	int result = 0;

	strMessage = pView->m_strProfileMessage;
	strPath = pView->m_strDefaultImagePath;
	strOldPath = pView->m_strOldPath;

	strPath.Replace(_T("\\"), _T("\\\\"));

	str.Format(L"INSERT INTO profile VALUES ('%s', '%s')", strPath, strMessage);
	
	//AfxMessageBox(strPath);
	//AfxMessageBox(str);

	const wchar_t* tmp = str;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);


		// SQL 명령문을 실행한다.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
		// 성공적으로 완료되었는지 체크한다.
		

		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			result = 1;
			//AfxMessageBox(_T("히히헤헤"));
		}

		// 명령 수행이 완료되었다는 것을 설정한다.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}


void TW_ODBC::DeleteProfilePath(CString strPath, CString strOld)
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strMessage, str2;
	int result = 0;

	//strPath = pView->m_strDefaultImagePath;
	strMessage = pView->m_strProfileMessage;
	//strOld = pView->m_strOldPath;

	//strPath.Replace(_T("\\"), _T("\\\\"));

	//str2.Format(L"SET SQL_SAFE_UPDATES = 0");
	str.Format(L"DELETE FROM profile");
	//AfxMessageBox(_T("헤헤헤헤"));

	const wchar_t* tmp = str;
	const wchar_t* tmp2 = str2;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// SQL 명령문을 실행한다.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
		RETCODE ret2 = SQLExecDirect(h_statement, (SQLWCHAR *)tmp2, SQL_NTS);

		// 성공적으로 완료되었는지 체크한다.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			result = 1;
			//AfxMessageBox(_T("히히"));
		}
		if (ret2 == SQL_SUCCESS || ret2 == SQL_SUCCESS_WITH_INFO) {
			//AfxMessageBox(_T("헤헿"));
		}

		// 명령 수행이 완료되었다는 것을 설정한다.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
	
}



void TW_ODBC::DeleteSound()
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();
	CSoundPlayer pSound;

	SQLHSTMT h_statement;

	CString str, str2;
	int result = 0;

	//str2.Format(L"SET SQL_SAFE_UPDATES = 0");
	str.Format(L"DELETE FROM sound");


	const wchar_t* tmp = str;
	const wchar_t* tmp2 = str2;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

		// SQL 명령문을 실행한다.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);
		RETCODE ret2 = SQLExecDirect(h_statement, (SQLWCHAR *)tmp2, SQL_NTS);

		// 성공적으로 완료되었는지 체크한다.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
			result = 1;

		}
		if (ret2 == SQL_SUCCESS || ret2 == SQL_SUCCESS_WITH_INFO) {

		}

		// 명령 수행이 완료되었다는 것을 설정한다.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}


void TW_ODBC::DataSaveSound(CString SoundPath[50], CString SoundName[50])
{
	CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
	CWeeklyPlannerView* pView = (CWeeklyPlannerView*)pFrame->GetActiveView();

	SQLHSTMT h_statement;
	CString str, strPath, strName;
	int result = 0;
	int i = 0;

	//	pView->m_soundSP.m_strSoundPath = SoundPath;

	while (SoundPath[i] != "") {
		strPath = SoundPath[i];
		strName = SoundName[i];
		strPath.Replace(_T("\\"), _T("\\\\"));
		str.Format(L"INSERT INTO sound VALUES ('%s', '%s')", strPath, strName);

		const wchar_t* tmp = str;
		if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
			// Query 문을 실행할 때 타임 아웃을 설정한다.
			SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);

			// SQL 명령문을 실행한다.
			RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);

			// 성공적으로 완료되었는지 체크한다.
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) {
				result = 1;
			}

			// 명령 수행이 완료되었다는 것을 설정한다.
			SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		}
		i++;

		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}
