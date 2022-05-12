#include <iostream>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <algorithm>
#include <string>

#include "loginCredentials.hpp"

void showSQLError(unsigned int handleType, const SQLHANDLE& handle)
{
	SQLCHAR SQLState[1024];
	SQLCHAR message[1024];
	if (SQL_SUCCESS == SQLGetDiagRecA(
			handleType,
			handle,
			1,
			SQLState,
			NULL,
			message,
			1024,
			NULL)
		)
	{
		std::cout << "SQL driver message: " <<
		message << std::endl << "SQL state: " <<
		SQLState << "." << std::endl;
	}
}

int main()
{
	SQLHANDLE SQLEnvHandle = NULL;
	SQLHANDLE SQLConnectionHandle = NULL;
	SQLHANDLE SQLStatementHandle_fetch = NULL;
	SQLHANDLE SQLStatementHandle_insert = NULL;
	SQLRETURN retCode = 0;
	char SQLQuery[] = "SHOW DATABASES";

	do 
	{
		if (SQL_SUCCESS != SQLAllocHandle(
			SQL_HANDLE_ENV,
			SQL_NULL_HANDLE,
			&SQLEnvHandle)
			)
		{
			break;
		}

		if (SQL_SUCCESS != SQLSetEnvAttr(
			SQLEnvHandle,
			SQL_ATTR_ODBC_VERSION,
			(SQLPOINTER)SQL_OV_ODBC3,
			0)
			)
		{
			break;
		}

		if (SQL_SUCCESS != SQLAllocHandle(
			SQL_HANDLE_DBC,
			SQLEnvHandle,
			&SQLConnectionHandle)
			)
		{
			break;
		}

		if (SQL_SUCCESS != SQLSetConnectAttr(
			SQLConnectionHandle,
			SQL_LOGIN_TIMEOUT,
			(SQLPOINTER)5,
			0)
			)
		{
			break;
		}

		SQLCHAR retConString[1024];

		// generate login credentials and cast to const char (SQLCHAR)
		struct loginCredentials lc;
		std::string str =
			"DRIVER=" + lc.SQLldriver + ";" +
			"SERVER=" + lc.SQLserver + ";" +
			"DATABASE=" + lc.SQLdatabase + ";" +
			"UID=" + lc.SQLuid + ";" +
			"PWD=" + lc.SQLpassword + ";";
		SQLCHAR loginCredentials[1024]{};
		std::copy(str.cbegin(), str.cend(), loginCredentials);

		switch (SQLDriverConnect(
			SQLConnectionHandle,
			NULL,
			loginCredentials,
			SQL_NTS,
			retConString,
			1024,
			NULL,
			SQL_DRIVER_NOPROMPT)
			)
		{
		case SQL_SUCCESS:
			break;
		case SQL_SUCCESS_WITH_INFO:
			break;
		case SQL_NO_DATA_FOUND:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_INVALID_HANDLE:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		case SQL_ERROR:
			showSQLError(SQL_HANDLE_DBC, SQLConnectionHandle);
			retCode = -1;
			break;
		default:
			break;
		}

		if (retCode == -1)
		{
			break;
		}

		if (SQL_SUCCESS != SQLAllocHandle(
			SQL_HANDLE_STMT,
			SQLConnectionHandle,
			&SQLStatementHandle_fetch)
			)
		{
			break;
		}

		if (SQL_SUCCESS != SQLAllocHandle(
			SQL_HANDLE_STMT,
			SQLConnectionHandle,
			&SQLStatementHandle_insert)
			)
		{
			break;
		}

		// fetch data
		if (SQL_SUCCESS != SQLExecDirectA(
			SQLStatementHandle_fetch,
			(SQLCHAR*)SQLQuery,
			SQL_NTS)
			) 
		{
			showSQLError(SQL_HANDLE_STMT, SQLStatementHandle_fetch);
			std::cout << "error fetching data" << std::endl;
			break;
		}
		else
		{
			while (SQLFetch(SQLStatementHandle_fetch) == SQL_SUCCESS) 
			{
 				char result[256] = "";
 				SQLGetData(
					SQLStatementHandle_fetch,
					1,
					SQL_C_DEFAULT,
					&result,
					sizeof(result),
					NULL
				);
				std::cout << result << std::endl;
			}
		}
	}
	while (false);

	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle_fetch);
	SQLFreeHandle(SQL_HANDLE_STMT, SQLStatementHandle_insert);
	SQLDisconnect(SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, SQLConnectionHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, SQLEnvHandle);
}
