//
// Created by diana on 1/4/2023.
//

#include <stdio.h>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include "platform_win.h"

typedef char *(WINAPI
        *test_dll_func)(char *);

typedef SQLRETURN (WINAPI
        *SQLAllocHandle_Func)(
        SQLSMALLINT HandleType, SQLHANDLE
InputHandle,
        SQLHANDLE *OutputHandle
);

SQLAllocHandle_Func _SQLAllocHandle = NULL;

typedef SQLRETURN (WINAPI
        *SQLFreeHandle_Func)(
        SQLSMALLINT HandleType, SQLHANDLE
InputHandle);

SQLFreeHandle_Func _SQLFreeHandle = NULL;

typedef SQLRETURN (WINAPI
        *SQLConnectW_Func)(
        SQLHDBC ConnectionHandle, SQLCHAR
*ServerName,
        SQLSMALLINT NameLength1,
        SQLCHAR
        *UserName,
        SQLSMALLINT NameLength2, SQLCHAR
        *Authentication,
        SQLSMALLINT NameLength3
);

SQLConnectW_Func _SQLConnectW = NULL;

typedef SQLRETURN (WINAPI
        *SQLDriverConnectW_Func)(
        SQLHDBC ConnectionHandle,
        SQLHWND WindowHandle,
        SQLWCHAR *InConnectionString,
        SQLSMALLINT StringLength1,
        SQLWCHAR *OutConnectionString,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLength2Ptr,
        SQLUSMALLINT DriverCompletion
);

SQLDriverConnectW_Func _SQLDriverConnectW = NULL;

typedef SQLRETURN (WINAPI
        *SQLDisconnect_Func)(
        SQLHDBC ConnectionHandle
);

SQLDisconnect_Func _SQLDisconnect = NULL;

typedef SQLRETURN (WINAPI *SQLPrepare_Func)(
        SQLHSTMT StatementHandle,
        SQLCHAR *StatementText,
        SQLINTEGER TextLength);

SQLPrepare_Func _SQLPrepare = NULL;

typedef SQLRETURN (WINAPI *SQLFreeStmt_Func)(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT Option);

SQLFreeStmt_Func _SQLFreeStmt = NULL;

typedef SQLRETURN (WINAPI *SQLExecute_Func)(
        SQLHSTMT StatementHandle);

SQLExecute_Func _SQLExecute = NULL;

typedef SQLRETURN (SQL_API *SQLGetInfoW_Func)(
        SQLHDBC ConnectionHandle,
        SQLUSMALLINT InfoType,
        SQLPOINTER InfoValuePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr);

SQLGetInfoW_Func _SQLGetInfoW = NULL;

SQLHANDLE test_env() {
    SQLRETURN sr = SQL_SUCCESS;
    SQLHANDLE henv = SQL_NULL_HANDLE;
    sr = _SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (FAILED(sr)) return SQL_INVALID_HANDLE;
    return henv;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file

int test_statement(SQLHANDLE hconn, char *stmt) {
    SQLHANDLE hstmt = SQL_NULL_HANDLE;
    SQLRETURN sr = _SQLAllocHandle(SQL_HANDLE_STMT, hconn, &hstmt);
    if (FAILED(sr)) {
        printf("_SQLAllocHandle SQL_HANDLE_STMT failed\n");
        return -1;
    } else {
        printf("_SQLAllocHandle SQL_HANDLE_STMT passed\n");
    }
    sr = _SQLPrepare(hstmt, stmt, SQL_NTS);
    if (FAILED(sr)) {
        printf("_SQLPrepare failed\n");
        _SQLFreeStmt(hstmt, SQL_NTS);
        return -1;
    } else {
        printf("_SQLPrepare passed\n");
        sr = _SQLExecute(hstmt);
        if (FAILED(sr)) {
            printf("_SQLExecute %s failed\n", stmt);
            _SQLFreeStmt(hstmt, SQL_NTS);
            return -1;
        } else {
            printf("_SQLExecute %s passed\n", stmt);
        }
        _SQLFreeStmt(hstmt, SQL_NTS);
    }
    return 0;
}

int test_connect(SQLHANDLE henv, const char *server, const char *uid, const char *pwd) {

    SQLRETURN sr = SQL_SUCCESS;
    SQLHANDLE hconn = SQL_NULL_HANDLE;
    sr = _SQLAllocHandle(SQL_HANDLE_DBC, henv, &hconn);
    if (FAILED(sr)) {
        printf("_SQLAllocHandle SQL_HANDLE_DBC failed\n");
        return -1;
    } else {
        printf("_SQLAllocHandle SQL_HANDLE_DBC success, %lld\n", (ULONG) hconn);
    }
    sr = _SQLConnectW(hconn, (SQLCHAR *)
            server, SQL_NTS, (SQLCHAR *)
                              uid, SQL_NTS, (SQLCHAR *)
                              pwd, SQL_NTS);
    if (FAILED(sr)) {
        printf("connect [server:%s,uid:%s,pwd:%s] failed\n", server, uid, pwd);
        return -1;
    } else {
        printf("connect [server:%s,uid:%s,pwd:%s] passed\n", server, uid, pwd);
    }
    const char *stmt = "create database odbc_test;";
//    int r = test_statement(hconn, stmt);
    sr = _SQLDisconnect(hconn);
    if (FAILED(sr)) {
        printf("disconnect [server:%s] failed\n", server);
    } else {
        printf("disconnect [server:%s] passed\n", server);
    }
    _SQLFreeHandle(SQL_HANDLE_DBC, hconn);
    return 0;
}

int test_getinfo(SQLHANDLE hconn) {
    SQLRETURN sr = SQL_SUCCESS;
    char buf[10];
    memset(buf, 0, 10);
    int infoLen = 0;
    sr = _SQLGetInfoW(hconn, SQL_DRIVER_ODBC_VER, buf, 10, &infoLen);
    if (FAILED(sr)) {
        printf("_SQLGetInfoW SQL_DRIVER_ODBC_VER failed\n");
        return -1;
    } else {
        printf("_SQLGetInfoW SQL_DRIVER_ODBC_VER success, %s\n", buf);
    }
    return 0;
}

int test_driver_connect(SQLHANDLE henv, const SQLWCHAR *connStr) {
    SQLRETURN sr = SQL_SUCCESS;
    SQLHANDLE hconn = SQL_NULL_HANDLE;
    sr = _SQLAllocHandle(SQL_HANDLE_DBC, henv, &hconn);
    if (FAILED(sr)) {
        printf("_SQLAllocHandle SQL_HANDLE_DBC failed\n");
        return -1;
    } else {
        printf("_SQLAllocHandle SQL_HANDLE_DBC success, %lld\n", (ULONG) hconn);
    }
    sr = _SQLDriverConnectW(hconn, NULL, connStr, SQL_NTS, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);
    if (FAILED(sr)) {
        printf("driver connect [%ls] failed\n", connStr);
        return -1;
    } else {
        printf("driver connect [%ls] passed\n", connStr);
        test_getinfo(hconn);
    }
    sr = _SQLDisconnect(hconn);
    if (FAILED(sr)) {
        printf("driver disconnect [server:%ls] failed\n", connStr);
    } else {
        printf("driver disconnect [server:%ls] passed\n", connStr);
    }
    _SQLFreeHandle(SQL_HANDLE_DBC, hconn);
    return 0;
}

int main(int argc, char *argv[]) {
    HMODULE m = LoadLibrary(
            "D:\\workspaces\\taos_odbc\\debug\\taos_odbc_driver\\taos_odbc_driver.dll");
    if (m != NULL) {
//        test_dll_func test_dll = (test_dll_func) GetProcAddress(m, "test_dll");
//        printf(test_dll("dll hello world\n"));
        _SQLAllocHandle = (SQLAllocHandle_Func) GetProcAddress(m, "SQLAllocHandle");
        _SQLFreeHandle = (SQLFreeHandle_Func) GetProcAddress(m, "SQLFreeHandle");
        _SQLConnectW = (SQLConnectW_Func) GetProcAddress(m, "SQLConnectW");
        _SQLDriverConnectW = (SQLDriverConnectW_Func) GetProcAddress(m, "SQLDriverConnectW");
        _SQLDisconnect = (SQLDisconnect_Func) GetProcAddress(m, "SQLDisconnect");
        _SQLPrepare = (SQLPrepare_Func) GetProcAddress(m, "SQLPrepare");
        _SQLFreeStmt = (SQLFreeStmt_Func) GetProcAddress(m, "SQLFreeStmt");
        _SQLExecute = (SQLExecute_Func) GetProcAddress(m, "SQLExecute");
        _SQLGetInfoW = (SQLGetInfoW_Func) GetProcAddress(m, "SQLGetInfoW");
        SQLHANDLE henv = test_env();
        if (henv == SQL_INVALID_HANDLE) {
            printf("env test fail, SQL_INVALID_HANDLE\n");
        } else {
            printf("env test passed, %lld\n", (ULONG) henv);
//            int r = test_connect(henv, "192.168.137.21", NULL, NULL);
//            if (r >= 0) {
//
//            }
            const SQLWCHAR *connStr = L"driver=TAOSODBC;server=192.168.137.21;port=6030";
            printf("ConnStr: %ls\n", connStr);
            int r = test_driver_connect(henv, connStr);
            if (r >= 0) {

            }
            SQLRETURN sr = _SQLFreeHandle(SQL_HANDLE_ENV, henv);
            if (FAILED(sr)) {
                printf("SQLFreeHandle fail\n");
            } else {
                printf("SQLFreeHandle success\n");
            }
        }
        FreeLibrary(m);
    } else {
        printf("can not load dll\n");
    }
    return 0;
}