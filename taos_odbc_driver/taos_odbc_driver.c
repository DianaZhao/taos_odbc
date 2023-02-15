// taos_odbc_driver.cpp : Defines the entry point for the application.
//

#include "taos_odbc_driver.h"
#include "platform_win.h"

int __stdcall tdengine_init(int argc, char **argv, char **groups) {
    return 0;
}

int __stdcall tdengine_end() {
    return 0;
}

int __stdcall td_thread_init() {
    return 0;
}

int __stdcall td_thread_end() {
    return 0;
}

BOOL __stdcall DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    switch (fdwReason) {
        case DLL_PROCESS_ATTACH:
            tdengine_init(0, NULL, NULL);
            break;
        case DLL_PROCESS_DETACH:
            tdengine_end();
            break;
        case DLL_THREAD_ATTACH:
            td_thread_init();
            break;
        case DLL_THREAD_DETACH:
            td_thread_end();
            break;
    }
    return TRUE;
}

static SQLRETURN do_alloc_env(
        SQLHANDLE *OutputHandle) {
    if (!OutputHandle) return SQL_INVALID_HANDLE;

    *OutputHandle = SQL_NULL_HANDLE;

    env_t *env = env_create();
    if (!env) return SQL_ERROR;

    *OutputHandle = (SQLHANDLE) env;
    return SQL_SUCCESS;
}

char *__stdcall test_dll(
        char *msg) {
    if (msg && *msg > 0) {
        return strcat("from dll: ", msg);
    } else {
        return NULL;
    }
}

SQLRETURN SQL_API SQLAllocHandle(
        SQLSMALLINT HandleType,
        SQLHANDLE InputHandle,
        SQLHANDLE *OutputHandle) {
    env_t *env;
    conn_t *conn;

    switch (HandleType) {
        case SQL_HANDLE_ENV:
            return do_alloc_env(OutputHandle);
        case SQL_HANDLE_DBC:
            if (InputHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;
            if (!OutputHandle) return SQL_INVALID_HANDLE;

            env = (env_t *) InputHandle;
            env_clr_errs(env);
            return env_alloc_conn(env, OutputHandle);
        case SQL_HANDLE_STMT:
            if (InputHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;
            if (!OutputHandle) return SQL_INVALID_HANDLE;

            conn = (conn_t *) InputHandle;
            conn_clr_errs(conn);
            return conn_alloc_stmt(conn, OutputHandle);
        case SQL_HANDLE_DESC:
            if (InputHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;
            if (!OutputHandle) return SQL_INVALID_HANDLE;

            conn = (conn_t *) InputHandle;
            conn_clr_errs(conn);
            return conn_alloc_desc(conn, OutputHandle);
        default:
            if (InputHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

            return SQL_ERROR;
    }
}

SQLRETURN SQL_API SQLFreeHandle(
        SQLSMALLINT HandleType,
        SQLHANDLE Handle) {
    if (Handle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    switch (HandleType) {
        case SQL_HANDLE_ENV:
            env_clr_errs((env_t *) Handle);
            return env_free((env_t *) Handle);
        case SQL_HANDLE_DBC:
            conn_clr_errs((conn_t *) Handle);
            return conn_free((conn_t *) Handle);
        case SQL_HANDLE_STMT:
            stmt_clr_errs((stmt_t *) Handle);
            return stmt_free((stmt_t *) Handle);
        case SQL_HANDLE_DESC:
            return desc_free((desc_t *) Handle);
        default:
            return SQL_ERROR;
    }
}

SQLRETURN SQL_API SQLDriverConnect(
        SQLHDBC ConnectionHandle,
        SQLHWND WindowHandle,
        SQLCHAR *InConnectionString,
        SQLSMALLINT StringLength1,
        SQLCHAR *OutConnectionString,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLength2Ptr,
        SQLUSMALLINT DriverCompletion) {
    if (ConnectionHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;
    conn_t *conn = (conn_t *) ConnectionHandle;
    conn_clr_errs(conn);
    return conn_driver_connect(conn, WindowHandle, InConnectionString, StringLength1, OutConnectionString, BufferLength,
                               StringLength2Ptr, DriverCompletion);
}

SQLRETURN SQL_API SQLDriverConnectW(
        SQLHDBC ConnectionHandle,
        SQLHWND WindowHandle,
        SQLWCHAR *InConnectionString,
        SQLSMALLINT StringLength1,
        SQLWCHAR *OutConnectionString,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLength2Ptr,
        SQLUSMALLINT DriverCompletion) {
    printf("SQLDriverConnectW\n");
    SQLRETURN ret = SQL_ERROR;
    SQLULEN Length = 0; /* Since we need bigger(in bytes) buffer for utf8 string, the length may be > max SQLSMALLINT */
    char *InConnStrA = NULL;
    SQLULEN InStrAOctLen = 0;
    char *OutConnStrA = NULL;

    if (!ConnectionHandle) {
        return SQL_INVALID_HANDLE;
    }

    InConnStrA = ConvertFromWChar(InConnectionString, StringLength1, &InStrAOctLen, &utf8, NULL);

    /* Allocate buffer for Asc OutConnectionString */
    if (OutConnectionString && BufferLength) {
        Length = BufferLength * 4 /*Max bytes per utf8 character */;
        OutConnStrA = (char *) calloc(Length, sizeof(char));

        if (OutConnStrA == NULL) {
            free(InConnStrA);
            free(OutConnStrA);
            return ret;
        }
    }

    ret = SQLDriverConnect(ConnectionHandle, WindowHandle, (SQLCHAR *) InConnStrA, InStrAOctLen,
                           (SQLCHAR *) OutConnStrA,
                           Length, StringLength2Ptr, DriverCompletion);
    if (!SQL_SUCCEEDED(ret)) {
        free(InConnStrA);
        free(OutConnStrA);
        return ret;
    }

    if (OutConnectionString) {
        Length = SetString(&utf8, OutConnectionString, BufferLength,
                                OutConnStrA, SQL_NTS);
        if (StringLength2Ptr)
            *StringLength2Ptr = (SQLSMALLINT) Length;
    }

    return ret;
}

SQLRETURN SQL_API SQLDisconnect(
        SQLHDBC ConnectionHandle) {
    if (ConnectionHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    conn_t *conn = (conn_t *) ConnectionHandle;

    conn_clr_errs(conn);

    conn_disconnect(conn);

    return SQL_SUCCESS;
}

SQLRETURN SQL_API SQLExecDirect(
        SQLHSTMT StatementHandle,
        SQLCHAR *StatementText,
        SQLINTEGER TextLength) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);
    return stmt_exec_direct(stmt, (const char *) StatementText, TextLength);
}

SQLRETURN SQL_API SQLSetEnvAttr(
        SQLHENV EnvironmentHandle,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength) {
    if (EnvironmentHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    env_t *env = (env_t *) EnvironmentHandle;

    env_clr_errs(env);

    return env_set_attr(env, Attribute, ValuePtr, StringLength);
}

SQLRETURN SQL_API SQLGetInfo(
        SQLHDBC ConnectionHandle,
        SQLUSMALLINT InfoType,
        SQLPOINTER InfoValuePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr) {
    if (ConnectionHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    conn_t *conn = (conn_t *) ConnectionHandle;

    conn_clr_errs(conn);

    return conn_get_info(conn, InfoType, InfoValuePtr, BufferLength, StringLengthPtr);
}

SQLRETURN SQL_API SQLGetInfoW(
        SQLHDBC ConnectionHandle,
        SQLUSMALLINT InfoType,
        SQLPOINTER InfoValuePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr) {
    return SQLGetInfo(ConnectionHandle, InfoType, InfoValuePtr, BufferLength, StringLengthPtr);
}

SQLRETURN SQL_API SQLEndTran(
        SQLSMALLINT HandleType,
        SQLHANDLE Handle,
        SQLSMALLINT CompletionType) {
    if (Handle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    switch (HandleType) {
        case SQL_HANDLE_ENV:
            assert(0);
            env_clr_errs((env_t *) Handle);
            return env_end_tran((env_t *) Handle, CompletionType);
        case SQL_HANDLE_DBC:
            conn_clr_errs((conn_t *) Handle);
            return conn_end_tran((conn_t *) Handle, CompletionType);
        default:
            return SQL_ERROR;
    }
}

SQLRETURN SQL_API SQLSetConnectAttr(
        SQLHDBC ConnectionHandle,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength) {
    if (ConnectionHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    conn_t *conn = (conn_t *) ConnectionHandle;

    conn_clr_errs(conn);

    return conn_set_attr(conn, Attribute, ValuePtr, StringLength);
}

SQLRETURN SQL_API SQLSetStmtAttr(
        SQLHSTMT StatementHandle,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_set_attr(stmt, Attribute, ValuePtr, StringLength);
}

SQLRETURN SQL_API SQLRowCount(
        SQLHSTMT StatementHandle,
        SQLLEN *RowCountPtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_get_row_count((stmt_t *) StatementHandle, RowCountPtr);
}

SQLRETURN SQL_API SQLNumResultCols(
        SQLHSTMT StatementHandle,
        SQLSMALLINT *ColumnCountPtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_get_col_count((stmt_t *) StatementHandle, ColumnCountPtr);
}

SQLRETURN SQL_API SQLDescribeCol(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber,
        SQLCHAR *ColumnName,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *NameLengthPtr,
        SQLSMALLINT *DataTypePtr,
        SQLULEN *ColumnSizePtr,
        SQLSMALLINT *DecimalDigitsPtr,
        SQLSMALLINT *NullablePtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_describe_col(stmt,
                             ColumnNumber,
                             ColumnName,
                             BufferLength,
                             NameLengthPtr,
                             DataTypePtr,
                             ColumnSizePtr,
                             DecimalDigitsPtr,
                             NullablePtr);
}

SQLRETURN SQL_API SQLBindCol(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber,
        SQLSMALLINT TargetType,
        SQLPOINTER TargetValuePtr,
        SQLLEN BufferLength,
        SQLLEN *StrLen_or_IndPtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_bind_col(stmt,
                         ColumnNumber,
                         TargetType,
                         TargetValuePtr,
                         BufferLength,
                         StrLen_or_IndPtr);
}

SQLRETURN SQL_API SQLFetch(
        SQLHSTMT StatementHandle) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_fetch(stmt);
}

SQLRETURN SQL_API SQLFetchScroll(
        SQLHSTMT StatementHandle,
        SQLSMALLINT FetchOrientation,
        SQLLEN FetchOffset) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_fetch_scroll(stmt, FetchOrientation, FetchOffset);
}

SQLRETURN SQL_API SQLFreeStmt(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT Option) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_free_stmt(stmt, Option);
}

SQLRETURN SQL_API SQLGetDiagRec(
        SQLSMALLINT HandleType,
        SQLHANDLE Handle,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr) {
    if (Handle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    switch (HandleType) {
        case SQL_HANDLE_ENV:
            return env_get_diag_rec((env_t *) Handle, RecNumber, SQLState, NativeErrorPtr, MessageText, BufferLength,
                                    TextLengthPtr);
        case SQL_HANDLE_DBC:
            return conn_get_diag_rec((conn_t *) Handle, RecNumber, SQLState, NativeErrorPtr, MessageText, BufferLength,
                                     TextLengthPtr);
        case SQL_HANDLE_STMT:
            return stmt_get_diag_rec((stmt_t *) Handle, RecNumber, SQLState, NativeErrorPtr, MessageText, BufferLength,
                                     TextLengthPtr);
        default:
            return SQL_ERROR;
    }

    return SQL_ERROR;
}

SQLRETURN SQL_API SQLGetDiagRecW(
        SQLSMALLINT HandleType,
        SQLHANDLE Handle,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr) {

    return SQLGetDiagRec(HandleType, Handle, RecNumber, SQLState, NativeErrorPtr, MessageText, BufferLength,
                         TextLengthPtr);
}

SQLRETURN SQL_API SQLGetDiagField(
        SQLSMALLINT HandleType,
        SQLHANDLE Handle,
        SQLSMALLINT RecNumber,
        SQLSMALLINT DiagIdentifier,
        SQLPOINTER DiagInfoPtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr) {
    if (Handle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    (void) HandleType;
    (void) RecNumber;
    (void) DiagIdentifier;
    (void) DiagInfoPtr;
    (void) BufferLength;
    (void) StringLengthPtr;
    assert(0);
}

SQLRETURN SQL_API SQLGetData(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT Col_or_Param_Num,
        SQLSMALLINT TargetType,
        SQLPOINTER TargetValuePtr,
        SQLLEN BufferLength,
        SQLLEN *StrLen_or_IndPtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_get_data(stmt, Col_or_Param_Num, TargetType, TargetValuePtr, BufferLength, StrLen_or_IndPtr);
}

SQLRETURN SQL_API SQLPrepare(
        SQLHSTMT StatementHandle,
        SQLCHAR *StatementText,
        SQLINTEGER TextLength) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_prepare(stmt, StatementText, TextLength);
}

SQLRETURN SQL_API SQLNumParams(
        SQLHSTMT StatementHandle,
        SQLSMALLINT *ParameterCountPtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_get_num_params(stmt, ParameterCountPtr);
}

SQLRETURN SQL_API SQLDescribeParam(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT ParameterNumber,
        SQLSMALLINT *DataTypePtr,
        SQLULEN *ParameterSizePtr,
        SQLSMALLINT *DecimalDigitsPtr,
        SQLSMALLINT *NullablePtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_describe_param(
            stmt,
            ParameterNumber,
            DataTypePtr,
            ParameterSizePtr,
            DecimalDigitsPtr,
            NullablePtr);
}

SQLRETURN SQL_API SQLBindParameter(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT ParameterNumber,
        SQLSMALLINT InputOutputType,
        SQLSMALLINT ValueType,
        SQLSMALLINT ParameterType,
        SQLULEN ColumnSize,
        SQLSMALLINT DecimalDigits,
        SQLPOINTER ParameterValuePtr,
        SQLLEN BufferLength,
        SQLLEN *StrLen_or_IndPtr) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_bind_param(stmt,
                           ParameterNumber,
                           InputOutputType,
                           ValueType,
                           ParameterType,
                           ColumnSize,
                           DecimalDigits,
                           ParameterValuePtr,
                           BufferLength,
                           StrLen_or_IndPtr);
}

SQLRETURN SQL_API SQLExecute(
        SQLHSTMT StatementHandle) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);

    return stmt_execute(stmt);
}

SQLRETURN SQL_API SQLConnectW(
        SQLHDBC ConnectionHandle,
        SQLCHAR *ServerName,
        SQLSMALLINT NameLength1,
        SQLCHAR *UserName,
        SQLSMALLINT NameLength2,
        SQLCHAR *Authentication,
        SQLSMALLINT NameLength3) {
    if (ConnectionHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    conn_t *conn = (conn_t *) ConnectionHandle;
    conn_clr_errs(conn);

    return conn_connect(
            conn,
            ServerName, NameLength1,
            UserName, NameLength2,
            Authentication, NameLength3);
}

SQLRETURN SQL_API SQLConnect(
        SQLHDBC ConnectionHandle,
        SQLCHAR *ServerName,
        SQLSMALLINT NameLength1,
        SQLCHAR *UserName,
        SQLSMALLINT NameLength2,
        SQLCHAR *Authentication,
        SQLSMALLINT NameLength3) {

    return SQLConnectW(ConnectionHandle, ServerName, NameLength1, UserName, NameLength2, Authentication, NameLength3);
}

SQLRETURN SQL_API SQLColAttribute(
        SQLHSTMT StatementHandle,
        SQLUSMALLINT ColumnNumber,
        SQLUSMALLINT FieldIdentifier,
        SQLPOINTER CharacterAttributePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr,
        SQLLEN *NumericAttributePtr) {
    (void) StatementHandle;
    (void) ColumnNumber;
    (void) FieldIdentifier;
    (void) CharacterAttributePtr;
    (void) BufferLength;
    (void) StringLengthPtr;
    (void) NumericAttributePtr;

    assert(0);
    return SQL_ERROR;
}

SQLRETURN SQL_API SQLTables(
        SQLHSTMT StatementHandle,
        SQLCHAR *CatalogName,
        SQLSMALLINT NameLength1,
        SQLCHAR *SchemaName,
        SQLSMALLINT NameLength2,
        SQLCHAR *TableName,
        SQLSMALLINT NameLength3,
        SQLCHAR *TableType,
        SQLSMALLINT NameLength4) {
    if (StatementHandle == SQL_NULL_HANDLE) return SQL_INVALID_HANDLE;

    stmt_t *stmt = (stmt_t *) StatementHandle;

    stmt_clr_errs(stmt);
    return stmt_tables(stmt,
                       CatalogName, NameLength1,
                       SchemaName, NameLength2,
                       TableName, NameLength3,
                       TableType, NameLength4);
}
