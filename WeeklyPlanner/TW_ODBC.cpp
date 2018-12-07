
#include "stdafx.h"
#include "TW_ODBC.h"

#include "MainFrm.h"
#include "WeeklyPlannerView.h"

#pragma comment(lib, "odbc32.lib")


// 데이터를 가져오는 최대 단위를 설정한다.
#define MAX_COUNT 8

// 서버탐색기를 이용하여 ODBC 연결하기
// 스키마 이름, 아이디, 패스워드, 일정내용명 기록
#define DBSchemas L"WeeklyPlanner"
#define DBID L"haey"
#define DBPassword L""
#define DBContents todolist


struct TodoList {
	wchar_t date[25];
	bool isDone;
	wchar_t DBContents[100];
};


TW_ODBC::TW_ODBC()
	: record_num(0)
	, m_dbDataCnt(0)
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

	int cnt = -1;
	AfxMessageBox(_T("TEST"));
	CString tmp;
	tmp.Format(L"SELECT * FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	AfxMessageBox(tmp);
	
	// table 가져오기
	wchar_t* query_str = tmp.GetBuffer();
	//wchar_t query_str = (L"SELECT * FROM todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	// 읽어온 데이터의 상태를 기록할 변수
	unsigned short record_state[MAX_COUNT];
	// 읽어온 데이터를 저장할 변수
	TodoList raw_data[MAX_COUNT];
	CString str;

	//데이터를 저장할 배열을 초기화 한다.
	memset(raw_data, 0, sizeof(raw_data));

	HSTMT h_statement;
	RETCODE ret_code;
	// Query 문을 위한 메모리를 할당한다.
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		record_num = 0;
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

		// 읽은 데이터의 개수를 저장할 변수의 주소를 전달한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);

		// 테이블에서 가져온 데이터를 속성별로 raw_data 변수에 저장하기 위해서
		// 속성별로 저장할 메모리 위치를 설정한다.

		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, &raw_data[0].DBContents, sizeof(wchar_t) * 100, NULL);

		// SQL 명령문을 실행한다.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);
		CString a;
		a.Format(_T("서버 내 데이터 수 %d"), record_num);
		AfxMessageBox(a);

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
					CString tmp;
					tmp.Format(_T("m_dbDataCnt = %d"), m_dbDataCnt);
					AfxMessageBox(tmp);
					CString strContents;
					strContents.Format(_T("%s"), raw_data[0].DBContents);

					pView->m_arrayTodoCheck[m_dbDataCnt]->SetWindowText(strContents);
					

					if (raw_data[0].isDone == 1) {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(1);
						pView->m_bChecked[m_dbDataCnt] = false;
						pView->m_checkCnt++;
					}
					else if(raw_data[0].isDone == 0){
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(true);
						pView->m_arrayTodoCheck[m_dbDataCnt]->SetCheck(0);
						pView->m_bChecked[m_dbDataCnt] = false;
					}
					else {
						pView->m_arrayTodoCheck[m_dbDataCnt]->EnableWindow(false);
						pView->m_arrayTodoBtn[m_dbDataCnt]->EnableWindow(false);
						pView->m_bChecked[m_dbDataCnt] = true;
					}
					cnt = m_dbDataCnt;
				}

			}
			// Query 문을 위해 할당한 메모리를 해제한다.
			SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
		}
			
	}

	return cnt;
}


void TW_ODBC::DataSaveTodolist(CString strToday, CString strTomorrow)
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

	str.Format(L"DELETE FROM weeklyplanner.todolist WHERE date >= '%s' AND date < '%s'", strToday, strTomorrow);
	const wchar_t* tmp = str;
	if (SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, mh_odbc, &h_statement)) {
		// Query 문을 실행할 때 타임 아웃을 설정한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_QUERY_TIMEOUT, (SQLPOINTER)15, SQL_IS_UINTEGER);
	
		// SQL 명령문을 실행한다.
		RETCODE ret = SQLExecDirect(h_statement, (SQLWCHAR *)tmp, SQL_NTS);

		// 성공적으로 완료되었는지 체크한다.
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) result = 1;

		if (result)
			AfxMessageBox(_T("DELETE = SUCCESS!!"));

		// 명령 수행이 완료되었다는 것을 설정한다.
		SQLEndTran(SQL_HANDLE_ENV, mh_environment, SQL_COMMIT);
		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}
