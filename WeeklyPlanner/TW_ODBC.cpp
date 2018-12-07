#include "stdafx.h"
#include "TW_ODBC.h"

#pragma comment(lib, "odbc32.lib")

// 데이터를 가져오는 최대 단위를 설정한다.
#define MAX_COUNT 100

// 서버탐색기를 이용하여 ODBC 연결하기
// 스키마 이름, 아이디, 패스워드 기록
#define DBSchemas L"WeeklyPlanner"
#define DBID L"haey"
#define DBPassword L""


struct TodoList {
	wchar_t date[25];
	int isDone;
	wchar_t todolist[100];
};


TW_ODBC::TW_ODBC()
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
			}
			else {
				// 접속에 실패한 경우, 구성했던 메모리를 제거한다.
				if (mh_odbc != SQL_NULL_HDBC)
					SQLFreeHandle(SQL_HANDLE_DBC, mh_odbc);
				if (mh_environment != SQL_NULL_HENV)
					SQLFreeHandle(SQL_HANDLE_ENV, mh_environment);
			}
		}
	}
	
	if (m_connect_flag)
		AfxMessageBox(_T("TRUE"));
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


void TW_ODBC::ImportData()
{
	AfxMessageBox(_T("TEST"));

	// table 가져오기
	wchar_t query_str[256] = L"SELECT * FROM todolist";
	// 읽어온 데이터의 상태를 기록할 변수
	unsigned short record_state[MAX_COUNT];
	// 읽어온 데이터를 저장할 변수
	TodoList raw_data[MAX_COUNT];
	CString str;
	// 읽은 데이터의 개수를 저장할 변수
	unsigned long int record_num = 0;

	//데이터를 저장할 배열을 초기화 한다.
	memset(raw_data, 0, sizeof(raw_data));

	HSTMT h_statement;
	RETCODE ret_code;
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
		// 읽은 데이터의 개수를 저장할 변수의 주소를 전달한다.
		SQLSetStmtAttr(h_statement, SQL_ATTR_ROWS_FETCHED_PTR, &record_num, 0);
		
		// 테이블에서 가져온 데이터를 속성별로 raw_data 변수에 저장하기 위해서
		// 속성별로 저장할 메모리 위치를 설정한다.
		SQLBindCol(h_statement, 1, SQL_WCHAR, raw_data[0].date, sizeof(wchar_t) * 20, NULL);
		SQLBindCol(h_statement, 2, SQL_SMALLINT, &raw_data[0].isDone, sizeof(int), NULL);
		SQLBindCol(h_statement, 3, SQL_WCHAR, &raw_data[0].todolist, sizeof(wchar_t) * 20, NULL);
		// SQL 명령문을 실행한다.
		ret_code = SQLExecDirect(h_statement, (SQLWCHAR*)query_str, SQL_NTS);
		// SQL 명령문의 실행 결과로 받은 데이터를 ListBox에 추가한다.
		while (ret_code = SQLFetchScroll(h_statement, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
			// 데이터 개수만큼 반복하면서 작업한다.
			for (unsigned long int i = 0; i < record_num; i++) {
				CString tem;
				tem.Format(_T("for : record_num : %d"), record_num);
				AfxMessageBox(tem);

				// 가져온 데이터가 삭제된 정보가 아니라면 해당 속성으로
				// 합쳐서 문자열로 구성하고 AfxMessageBox에 등록한다.
				if (record_state[i] != SQL_ROW_DELETED && record_state[i] != SQL_ROW_ERROR) {
					str.Format(L"%s, %d, %s", raw_data[i].date, raw_data[i].isDone, raw_data[i].todolist);
					AfxMessageBox(str);
					//m_data_list.InsertString(-1, str);
				}
			}
		}
		// Query 문을 위해 할당한 메모리를 해제한다.
		SQLFreeHandle(SQL_HANDLE_STMT, h_statement);
	}
}
