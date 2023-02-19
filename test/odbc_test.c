//
// Created by diana on 1/4/2023.
//

#include <stdio.h>
#include <Windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <odbcss.h>
#include "platform_win.h"

#define MAXBUFLEN 256

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

typedef SQLRETURN (WINAPI *SQLExecDirect_Func)(
        SQLHSTMT StatementHandle,
        SQLCHAR *StatementText,
        SQLINTEGER TextLength);

SQLExecDirect_Func _SQLExecDirect = NULL;

typedef SQLRETURN (SQL_API *SQLGetInfoW_Func)(
        SQLHDBC ConnectionHandle,
        SQLUSMALLINT InfoType,
        SQLPOINTER InfoValuePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr);

SQLGetInfoW_Func _SQLGetInfoW = NULL;

typedef SQLRETURN (SQL_API *SQLGetDiagRec_Func)(
        SQLSMALLINT HandleType,
        SQLHANDLE Handle,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr);

SQLGetDiagRec_Func _SQLGetDiagRec = NULL;

typedef SQLRETURN (SQL_API *SQLFetch_Func)(
        SQLHSTMT StatementHandle);

SQLFetch_Func _SQLFetch = NULL;

typedef SQLRETURN (SQL_API *SQLGetData_Func)(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT Col_or_Param_Num,
        SQLSMALLINT TargetType,
        SQLPOINTER TargetValuePtr,
        SQLLEN BufferLength,
        SQLLEN *StrLen_or_IndPtr);

SQLGetData_Func _SQLGetData = NULL;

void ProcessLogMessages(SQLSMALLINT plm_handle_type, SQLHANDLE plm_handle) {
    RETCODE plm_retcode = SQL_SUCCESS;
    UCHAR plm_szSqlState[MAXBUFLEN] = "", plm_szErrorMsg[MAXBUFLEN] = "";
    SDWORD plm_pfNativeError = 0L;
    SWORD plm_pcbErrorMsg = 0;
    SQLSMALLINT plm_cRecNmbr = 1;
    SDWORD plm_SS_MsgState = 0, plm_SS_Severity = 0;
    SQLINTEGER plm_Rownumber = 0;
    USHORT plm_SS_Line;
    SQLSMALLINT plm_cbSS_Procname, plm_cbSS_Srvname;
    SQLCHAR plm_SS_Procname[MAXBUFLEN], plm_SS_Srvname[MAXBUFLEN];

    while (plm_retcode != SQL_NO_DATA_FOUND) {
        plm_retcode = _SQLGetDiagRec(plm_handle_type, plm_handle, plm_cRecNmbr,
                                     plm_szSqlState, &plm_pfNativeError, plm_szErrorMsg,
                                     MAXBUFLEN - 1, &plm_pcbErrorMsg);

        // Note that if the application has not yet made a successful connection,
        // the SQLGetDiagField information has not yet been cached by ODBC Driver Manager and
        // these calls to SQLGetDiagField will fail.
        if (plm_retcode != SQL_NO_DATA_FOUND) {
//            if (ConnInd) {
//                plm_retcode = _SQLGetDiagField( plm_handle_type, plm_handle, plm_cRecNmbr,
//                                               SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
//                                               SQL_IS_INTEGER, NULL);
//
//                plm_retcode = _SQLGetDiagField( plm_handle_type, plm_handle, plm_cRecNmbr,
//                                               SQL_DIAG_SS_LINE, &plm_SS_Line, SQL_IS_INTEGER, NULL);
//
//                plm_retcode = _SQLGetDiagField( plm_handle_type, plm_handle, plm_cRecNmbr,
//                                               SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
//                                               SQL_IS_INTEGER, NULL);
//
//                plm_retcode = _SQLGetDiagField( plm_handle_type, plm_handle, plm_cRecNmbr,
//                                               SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
//                                               SQL_IS_INTEGER, NULL);
//
//                plm_retcode = _SQLGetDiagField( plm_handle_type, plm_handle, plm_cRecNmbr,
//                                               SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
//                                               sizeof(plm_SS_Procname), &plm_cbSS_Procname);
//
//                plm_retcode = _SQLGetDiagField( plm_handle_type, plm_handle, plm_cRecNmbr,
//                                               SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
//                                               sizeof(plm_SS_Srvname), &plm_cbSS_Srvname);
//            }

            printf("szSqlState = %s\n", plm_szSqlState);
            printf("pfNativeError = %d\n", plm_pfNativeError);
            printf("szErrorMsg = %s\n", plm_szErrorMsg);
            printf("pcbErrorMsg = %d\n\n", plm_pcbErrorMsg);

//            if (ConnInd) {
//                printf("ODBCRowNumber = %d\n", plm_Rownumber);
//                printf("SSrvrLine = %d\n", plm_Rownumber);
//                printf("SSrvrMsgState = %d\n", plm_SS_MsgState);
//                printf("SSrvrSeverity = %d\n", plm_SS_Severity);
//                printf("SSrvrProcname = %s\n", plm_SS_Procname);
//                printf("SSrvrSrvname = %s\n\n", plm_SS_Srvname);
//            }
        }

        plm_cRecNmbr++;   // Increment to next diagnostic record.
    }
}

SQLHANDLE test_env() {
    SQLRETURN sr = SQL_SUCCESS;
    SQLHANDLE henv = SQL_NULL_HANDLE;
    sr = _SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv);
    if (FAILED(sr)) return SQL_INVALID_HANDLE;
    return henv;
}

int test_data(SQLHANDLE hconn) {
    SQLHANDLE hstmt = SQL_NULL_HANDLE;
    SQLCHAR b[10];
    SQLINTEGER ts, ti, si, i, bi;
    SQLFLOAT f, d;
    SQLRETURN sr = _SQLAllocHandle(SQL_HANDLE_STMT, hconn, &hstmt);
    if (FAILED(sr)) {
        printf("test_data _SQLAllocHandle SQL_HANDLE_STMT failed\n");
        return -1;
    } else {
        printf("test_data _SQLAllocHandle SQL_HANDLE_STMT passed\n");
    }
    sr = _SQLExecDirect(hstmt, "SELECT * FROM tbl_test;", SQL_NTS);
    if (FAILED(sr)) {
        printf("test_data _SQLExecDirect failed\n");
        ProcessLogMessages(SQL_HANDLE_STMT, hstmt);
        _SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return -1;
    } else {
        printf("test_data _SQLExecDirect passed\n");
        while (TRUE) {
            sr = _SQLFetch(hstmt);
            if (sr == SQL_ERROR || sr == SQL_SUCCESS_WITH_INFO) {
                ProcessLogMessages(SQL_HANDLE_STMT, hstmt);
            }
            if (sr == SQL_SUCCESS || sr == SQL_SUCCESS_WITH_INFO) {

                /* Get data for columns 1, 2, and 3 */
                printf("test_data _SQLFetch passed\n");
                _SQLGetData(hstmt, 1, SQL_C_ULONG, &ts, 0, &ts);
                _SQLGetData(hstmt, 2, SQL_C_ULONG, &ti, 0, &ti);
                _SQLGetData(hstmt, 3, SQL_C_ULONG, &si, 0, &si);
                _SQLGetData(hstmt, 4, SQL_C_ULONG, &i, 0, &i);
                _SQLGetData(hstmt, 5, SQL_C_ULONG, &bi, 0, &bi);
                _SQLGetData(hstmt, 6, SQL_C_FLOAT, &f, 0, &f);
                _SQLGetData(hstmt, 7, SQL_C_DOUBLE, &d, 0, &d);
                _SQLGetData(hstmt, 8, SQL_C_CHAR, b, 10, &b);

                /* Print the row of data */

                printf("%lld | %ld |  %ld | %ld | %ld | %f | %f | %*s\n", ts, ti, si, i, bi, f, d, b);
            } else {
                break;
            }
        }
    }
    _SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
    return 0;
}

int test_statement(SQLHANDLE hconn, const char *stmt) {
    SQLHANDLE hstmt = SQL_NULL_HANDLE;
    SQLRETURN sr = _SQLAllocHandle(SQL_HANDLE_STMT, hconn, &hstmt);
    if (FAILED(sr)) {
        printf("_SQLAllocHandle SQL_HANDLE_STMT failed\n");
        return -1;
    } else {
        printf("_SQLAllocHandle SQL_HANDLE_STMT passed\n");
    }
    sr = _SQLExecDirect(hstmt, stmt, SQL_NTS);
    if (FAILED(sr)) {
        printf("_SQLExecDirect failed\n");
        ProcessLogMessages(SQL_HANDLE_STMT, hstmt);
        _SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
        return -1;
//    } else {
//        printf("_SQLPrepare passed\n");
//        sr = _SQLExecute(hstmt);
//        if (FAILED(sr)) {
//            printf("_SQLExecute %s failed\n", stmt);
//            _SQLFreeStmt(hstmt, SQL_NTS);
//            return -1;
//        } else {
//            printf("_SQLExecute %s passed\n", stmt);
//        }
//        _SQLFreeStmt(hstmt, SQL_NTS);
    }
    _SQLFreeHandle(SQL_HANDLE_STMT, hstmt);
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
        const char *stmt = "use odbc_test;";
        test_statement(hconn, stmt);
        test_data(hconn);
//        SQLCHAR *qstr = calloc(1000, sizeof(SQLCHAR));
//        for (int i = 0; i < 10; ++i) {
//            memset(qstr, 0, 1000 * sizeof(SQLCHAR));
//            sprintf(qstr, "insert into tbl_test values (%lld, %d, %d, %d, %d, %f, %lf, '%s')",
//                    (1546300800000 + i * 1000), i, i, i, i * 10000000, i * 1.0, i * 2.0, "hello");
//            printf("qstr: %s\n", qstr);
//            test_statement(hconn, qstr);
//        }
//        const char *stmt1 = "create table tbl_test (ts timestamp, ti tinyint, si smallint, i int, bi bigint, f float, d double, b binary(10));";
//        test_statement(hconn, stmt1);

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
        _SQLExecDirect = (SQLExecDirect_Func) GetProcAddress(m, "SQLExecDirect");
        _SQLGetInfoW = (SQLGetInfoW_Func) GetProcAddress(m, "SQLGetInfoW");
        _SQLGetDiagRec = (SQLGetDiagRec_Func) GetProcAddress(m, "SQLGetDiagRec");
        _SQLFetch = (SQLFetch_Func) GetProcAddress(m, "SQLFetch");
        _SQLGetData = (SQLGetData_Func) GetProcAddress(m, "SQLGetData");

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