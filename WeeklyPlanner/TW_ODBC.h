#pragma once

#include <odbcinst.h>
#include <sqlext.h>

class TW_ODBC
{
private:
	SQLHANDLE mh_environment;
	SQLHDBC mh_odbc;
	char m_connect_flag = 0;
public:
	TW_ODBC();
	~TW_ODBC();
	int ImportData();
	unsigned long int record_num;
	int m_dbDataCnt;
};

