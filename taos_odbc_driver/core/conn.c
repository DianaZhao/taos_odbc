/*
 * MIT License
 *
 * Copyright (c) 2022 DianaZhao <dianaz@outlook.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include <Winsock2.h>
#include "conn.h"
#include "desc.h"
#include "errs.h"
#include "enums.h"
#include "stmt.h"
#include "td_platform_win32.h"

#include <odbcinst.h>
#include <string.h>

static char *conn_str_varkeys[] = {"dsn", "dsnname", "server", "ip", "addr", "port",
                                   "user", "uid", "password", "pwd", "database", "db"};

static char *conn_str_keys[] = {"dsn", "dsn", "ip", "ip", "ip", "port",
                                "uid", "uid", "pwd", "pwd", "db", "db"};

static void _conn_init(conn_t *conn, env_t *env) {
    conn->env = env_ref(env);
    int prev = atomic_fetch_add(&env->conns, 1);
    assert(prev >= 0);

    errs_init(&conn->errs);

    conn->refc = 1;
}

static void _conn_release(conn_t *conn) {
    assert(conn->taos == NULL);

    int prev = atomic_fetch_sub(&conn->env->conns, 1);
    assert(prev >= 1);
    int stmts = atomic_load(&conn->stmts);
    assert(stmts == 0);
    env_unref(conn->env);
    conn->env = NULL;

    connection_cfg_release(&conn->cfg);
    errs_release(&conn->errs);

    return;
}

conn_t *conn_create(env_t *env) {
    conn_t *conn = (conn_t *) calloc(1, sizeof(*conn));
    if (!conn) {
//    env_oom(env);
        env_t *__env = env;
        errs_append_x(&__env->errs, "_file_name_", 66, "_function_name_", "", "HY001", 0, "Memory allocation error");
        return NULL;
    }

    _conn_init(conn, env);

    return conn;
}

conn_t *conn_ref(conn_t *conn) {
    int prev = atomic_fetch_add(&conn->refc, 1);
    assert(prev > 0);
    return conn;
}

conn_t *conn_unref(conn_t *conn) {
    int prev = atomic_fetch_sub(&conn->refc, 1);
    if (prev > 1) return conn;
    assert(prev == 1);

    _conn_release(conn);
    free(conn);

    return NULL;
}

SQLRETURN conn_free(conn_t *conn) {
    int outstandings = atomic_load(&conn->outstandings);
    if (outstandings) {
//        conn_append_err_format(conn, "HY000", 0, "General error:%d outstandings still active", outstandings);
        conn_t *__conn = conn;
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "General error:%d outstandings still active" "", outstandings);
        errs_append_x(&__conn->errs, "_file_name_", 94, "_function_name_",
                      __conn->cfg.dsn ? __conn->cfg.dsn : (__conn->cfg.driver ? __conn->cfg.driver : ""), "HY000", 0,
                      _buf);
        return SQL_ERROR;
    }

    int stmts = atomic_load(&conn->stmts);
    if (stmts) {
//        conn_append_err_format(conn, "HY000", 0, "General error:%d statements are still outstanding", stmts);
        conn_t *__conn = conn;
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "General error:%d statements are still outstanding" "", stmts);
        errs_append_x(&__conn->errs, "_file_name_", 106, "_function_name_",
                      __conn->cfg.dsn ? __conn->cfg.dsn : (__conn->cfg.driver ? __conn->cfg.driver : ""), "HY000", 0,
                      _buf);
        return SQL_ERROR;
    }

    int descs = atomic_load(&conn->descs);
    if (descs) {
//        conn_append_err_format(conn, "HY000", 0, "General error:%d descriptors are still outstanding", descs);
        conn_t *__conn = conn;
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "General error:%d descriptors are still outstanding" "", descs);
        errs_append_x(&__conn->errs, "_file_name_", 118, "_function_name_",
                      __conn->cfg.dsn ? __conn->cfg.dsn : (__conn->cfg.driver ? __conn->cfg.driver : ""), "HY000", 0,
                      _buf);;
        return SQL_ERROR;
    }

    conn_unref(conn);
    return SQL_SUCCESS;
}

static SQLRETURN _do_conn_connect(conn_t *conn) {
    assert(conn->taos == NULL);
    const connection_cfg_t *cfg = &conn->cfg;
    printf("dsn: %s, ip: %s, port: %d\n", cfg->dsn, cfg->ip, cfg->port);
    conn->taos = taos_connect(cfg->ip, cfg->uid, cfg->pwd, cfg->db, cfg->port);
    if (!conn->taos) {
        fixed_buf_t buffer;
        buffer.cap = 1024;
        buffer.nr = 0;
        fixed_buf_sprintf(&buffer, "taos_odbc://");
        if (cfg->uid) {
            fixed_buf_sprintf(&buffer, "%s:*@", cfg->uid);
        }
        if (cfg->ip) {
            if (cfg->port) {
                fixed_buf_sprintf(&buffer, "%s:%d", cfg->ip, cfg->port);
            } else {
                fixed_buf_sprintf(&buffer, "%s", cfg->ip);
            }
        } else {
            fixed_buf_sprintf(&buffer, "localhost");
        }
        if (cfg->db) {
            fixed_buf_sprintf(&buffer, "/%s", cfg->db);
        }
        conn_t *__conn = conn;
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "Client unable to establish connection:[%s][%s]" "", buffer.buf,
                 taos_errstr((void *) 0));
        errs_append_x(&__conn->errs, "_file_name_", 157, "_function_name_",
                      __conn->cfg.dsn ? __conn->cfg.dsn : (__conn->cfg.driver ? __conn->cfg.driver : ""), "08001",
                      taos_errno(((void *) 0)), _buf);
        return SQL_ERROR;
    }

    return SQL_SUCCESS;
}

static void _conn_fill_out_connection_str(
        conn_t *conn,
        SQLCHAR *OutConnectionString,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLength2Ptr) {
    char *p = (char *) OutConnectionString;
    if (BufferLength > 0) *p = '\0';
    size_t count = 0;
    int n = 0;

    fixed_buf_t buffer;
    buffer.buf = p;
    buffer.cap = BufferLength;
    buffer.nr = 0;
    if (conn->cfg.driver) {
        fixed_buf_sprintf(&buffer, "Driver={%s};", conn->cfg.driver);
        n = buffer.nr;
    } else {
        fixed_buf_sprintf(&buffer, "DSN=%s;", conn->cfg.dsn);
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (conn->cfg.uid) {
        fixed_buf_sprintf(&buffer, "UID=%s;", conn->cfg.uid);
        n = buffer.nr;
    } else {
        fixed_buf_sprintf(&buffer, "UID=;");
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (conn->cfg.pwd) {
        fixed_buf_sprintf(&buffer, "PWD=*;");
        n = buffer.nr;
    } else {
        fixed_buf_sprintf(&buffer, "PWD=;");
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (conn->cfg.ip) {
        if (conn->cfg.port) {
            fixed_buf_sprintf(&buffer, "Server=%s:%d;", conn->cfg.ip, conn->cfg.port);
            n = buffer.nr;
        } else {
            fixed_buf_sprintf(&buffer, "Server=%s;", conn->cfg.ip);
            n = buffer.nr;
        }
    } else {
        fixed_buf_sprintf(&buffer, "Server=;");
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (conn->cfg.db) {
        fixed_buf_sprintf(&buffer, "DB=%s;", conn->cfg.db);
        n = buffer.nr;
    } else {
        fixed_buf_sprintf(&buffer, "DB=;");
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (conn->cfg.unsigned_promotion) {
        fixed_buf_sprintf(&buffer, "UNSIGNED_PROMOTION=1;");
        n = buffer.nr;
    } else {
        fixed_buf_sprintf(&buffer, "UNSIGNED_PROMOTION=;");
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (conn->cfg.cache_sql) {
        fixed_buf_sprintf(&buffer, "CACHE_SQL=1;");
        n = buffer.nr;
    } else {
        fixed_buf_sprintf(&buffer, "CACHE_SQL=;");
        n = buffer.nr;
    }
    if (n > 0) count += n;

    if (buffer.nr + 1 == buffer.cap) {
        char *x = buffer.buf + buffer.nr;
        for (int i = 0; i < 3 && x > buffer.buf; ++i, --x) x[-1] = '.';
    }

    if (StringLength2Ptr) {
        *StringLength2Ptr = count;
    }
}

int parse_conn_str(SQLCHAR *conn_str, const char *delim, connection_cfg_t *cfg) {
    int i = 0;
    char *kv = strtok(conn_str, delim);;
    while (kv != NULL) {
        char *vs = strchr(kv, '=');
        if (vs != NULL) {
            int ks = (vs - kv) * sizeof(char);
            char *key = malloc(ks);
            memcpy(key, kv, ks);
            for (int j = 0; j < 11; j++) {
                if (_stricmp(key, conn_str_varkeys[j])) {
                    char *val = vs + 1;
                    key = conn_str_keys[j];
                    if (strcmp(key, "ip") == 0) {
                        cfg->ip = val;
                    } else if (strcmp(key, "dsn") == 0) {
                        cfg->dsn = val;
                    } else if (strcmp(key, "db") == 0) {
                        cfg->db = val;
                    } else if (strcmp(key, "uid") == 0) {
                        cfg->uid = val;
                    } else if (strcmp(key, "pwd") == 0) {
                        cfg->pwd = val;
                    } else if (strcmp(key, "port") == 0) {
                        cfg->port = atoi(val);
                    }
                    i++;
                    break;
                }
            }
        }
        kv = strtok(NULL, delim);
    }
    return i;
}

SQLRETURN conn_driver_connect(
        conn_t *conn,
        SQLHWND WindowHandle,
        SQLCHAR *InConnectionString,
        SQLSMALLINT StringLength1,
        SQLCHAR *OutConnectionString,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLength2Ptr,
        SQLUSMALLINT DriverCompletion) {
    (void) WindowHandle;

    switch (DriverCompletion) {
        case SQL_DRIVER_NOPROMPT:
            break;
        default: conn_append_err_format(conn, "HY000", 0,
                                        "General error:`%s[%d/0x%x]` not supported yet",
                                        sql_driver_completion(DriverCompletion), DriverCompletion, DriverCompletion);
            return SQL_ERROR;
    }

    if (conn->taos) {
        conn_append_err(conn, "HY000", 0, "General error:Already connected");
        return SQL_ERROR;
    }

//    parser_param_t param;
//
//    param.debug_flex = env_get_debug_flex(conn->env);
//    param.debug_bison = env_get_debug_bison(conn->env);

    if (StringLength1 == SQL_NTS) StringLength1 = strlen((const char *) InConnectionString);
//    int r = parser_parse((const char *) InConnectionString, StringLength1, &param);
    SQLRETURN sr = SQL_SUCCESS;
//
//        if (r) {
//            conn_append_err_format(
//                    conn, "HY000", 0,
//                    "General error:bad syntax for connection string:[%.*s][(%d,%d)->(%d,%d):%s]",
//                    StringLength1, InConnectionString,
//                    param.row0, param.col0, param.row1, param.col1,
//                    param.errmsg);
//
//            break;
//        }

    int r = parse_conn_str(InConnectionString, ";", &conn->cfg);
    if (r <= 0) {
        conn_append_err(conn, "HY000", 0, "Can not parse connection string");
        return SQL_ERROR;
    }
    sr = _do_conn_connect(conn);
    if (!(sr == SQL_SUCCESS || sr == SQL_SUCCESS_WITH_INFO)) return SQL_ERROR;

    _conn_fill_out_connection_str(conn, OutConnectionString, BufferLength, StringLength2Ptr);

//        parser_param_release(&param);
    return SQL_SUCCESS;

}

void conn_disconnect(conn_t *conn) {
    int stmts = atomic_load(&conn->stmts);
    assert(stmts == 0);

    if (conn->taos) {
        taos_close(conn->taos);
        conn->taos = NULL;
    }
    connection_cfg_release(&conn->cfg);
}

static void _conn_get_dbms_name(conn_t *conn, const char **name) {
    assert(conn->taos);
    const char *server = taos_get_server_info(conn->taos);
    if (name) *name = server;
}

static void _get_driver_name(const char **name) {
    const char *client = taos_get_client_info();
    if (name) *name = client;
}

static SQLRETURN _conn_commit(conn_t *conn) {
    int outstandings = atomic_load(&conn->outstandings);
    if (outstandings == 0) {
        conn_append_err_format(conn, "01000", 0, "General warning:no outstanding connection");
        return SQL_SUCCESS_WITH_INFO;
    }

    conn_append_err_format(conn, "25S02", 0, "Transaction is still active");
    return SQL_ERROR;
}

static int _conn_rollback(conn_t *conn) {
    int outstandings = atomic_load(&conn->outstandings);
    if (outstandings == 0) {
        conn_append_err_format(conn, "01000", 0, "General warning:no outstandings");
        return SQL_SUCCESS_WITH_INFO;
    }

    conn_append_err_format(conn, "25S01", 0, "Transaction state unknown");
    return SQL_ERROR;
}

SQLRETURN conn_get_diag_rec(
        conn_t *conn,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr) {
    return errs_get_diag_rec(&conn->errs, RecNumber, SQLState, NativeErrorPtr, MessageText, BufferLength,
                             TextLengthPtr);
}

SQLRETURN conn_alloc_stmt(conn_t *conn, SQLHANDLE *OutputHandle) {
    *OutputHandle = SQL_NULL_HANDLE;

    stmt_t *stmt = stmt_create(conn);
    if (stmt == NULL) return SQL_ERROR;

    *OutputHandle = (SQLHANDLE) stmt;
    return SQL_SUCCESS;
}

SQLRETURN conn_alloc_desc(conn_t *conn, SQLHANDLE *OutputHandle) {
    desc_t *desc = desc_create(conn);
    if (!desc) return SQL_ERROR;

    *OutputHandle = (SQLHANDLE) desc;
    return SQL_SUCCESS;
}

static SQLRETURN _conn_connect(conn_t *conn) {
    char buf[1024];
    buf[0] = '\0';

    int r;
    r = SQLGetPrivateProfileString((LPCSTR) conn->cfg.dsn, "UNSIGNED_PROMOTION", (LPCSTR) "0", (LPSTR) buf, sizeof(buf),
                                   NULL);
    if (r == 1 && buf[0] == '1') conn->cfg.unsigned_promotion = 1;

    if (!conn->cfg.pwd) {
        buf[0] = '\0';
        r = SQLGetPrivateProfileString((LPCSTR) conn->cfg.dsn, "PWD", (LPCSTR) "", (LPSTR) buf, sizeof(buf), NULL);
        if (buf[0]) {
            conn->cfg.pwd = strdup(buf);
            if (!conn->cfg.pwd) {
                conn_oom(conn);
                return SQL_ERROR;
            }
        } else {
            conn->cfg.pwd = NULL;
        }
    }

    if (!conn->cfg.uid) {
        buf[0] = '\0';
        r = SQLGetPrivateProfileString((LPCSTR) conn->cfg.dsn, "UID", (LPCSTR) "", (LPSTR) buf, sizeof(buf), NULL);
        if (buf[0]) {
            conn->cfg.uid = strdup(buf);
            if (!conn->cfg.uid) {
                conn_oom(conn);
                return SQL_ERROR;
            }
        } else {
            conn->cfg.uid = NULL;
        }
    }
    if (!conn->cfg.db) {
        conn->cfg.db = NULL;
    }
    if (!conn->cfg.port) {
        conn->cfg.port = 6030;
    }
    return _do_conn_connect(conn);
}

SQLRETURN conn_connect(
        conn_t *conn,
        SQLCHAR *ServerName,
        SQLSMALLINT NameLength1,
        SQLCHAR *UserName,
        SQLSMALLINT NameLength2,
        SQLCHAR *Authentication,
        SQLSMALLINT NameLength3) {
    if (conn->taos) {
        conn_append_err(conn, "HY000", 0, "General error:Already connected");
        return SQL_ERROR;
    }

    if (NameLength1 == SQL_NTS) NameLength1 = ServerName ? strlen((const char *) ServerName) : 0;
    if (NameLength2 == SQL_NTS) NameLength2 = UserName ? strlen((const char *) UserName) : 0;
    if (NameLength3 == SQL_NTS) NameLength3 = Authentication ? strlen((const char *) Authentication) : 0;

    connection_cfg_release(&conn->cfg);
    if (ServerName) {
        conn->cfg.ip = strndup((const char *) ServerName, NameLength1);
        if (!conn->cfg.ip) {
            conn_oom(conn);
            return SQL_ERROR;
        }
    }
    if (UserName) {
        conn->cfg.uid = strndup((const char *) UserName, NameLength2);
        if (!conn->cfg.uid) {
            conn_oom(conn);
            return SQL_ERROR;
        }
    }
    if (Authentication) {
        conn->cfg.pwd = strndup((const char *) Authentication, NameLength3);
        if (!conn->cfg.pwd) {
            conn_oom(conn);
            return SQL_ERROR;
        }
    }

    SQLRETURN sr = _conn_connect(conn);
    return sr;
}

static SQLRETURN _conn_get_info_dbms_name(
        conn_t *conn,
        char *buf,
        size_t sz,
        SQLSMALLINT *StringLengthPtr) {
    const char *name;
    _conn_get_dbms_name(conn, &name);

    int n = snprintf(buf, sz, "%s", name);
    if (StringLengthPtr) *StringLengthPtr = n;

    return SQL_SUCCESS;
}

static SQLRETURN _conn_get_info_driver_name(
        conn_t *conn,
        char *buf,
        size_t sz,
        SQLSMALLINT *StringLengthPtr) {
    (void) conn;
    const char *name;
    _get_driver_name(&name);

    int n = snprintf(buf, sz, "%s", name);
    if (StringLengthPtr) *StringLengthPtr = n;

    return SQL_SUCCESS;
}

SQLRETURN conn_get_info(
        conn_t *conn,
        SQLUSMALLINT InfoType,
        SQLPOINTER InfoValuePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr) {
    switch (InfoType) {
        case SQL_DBMS_NAME:
            return _conn_get_info_dbms_name(conn, (char *) InfoValuePtr, (size_t) BufferLength, StringLengthPtr);
        case SQL_DRIVER_NAME:
            return _conn_get_info_driver_name(conn, (char *) InfoValuePtr, (size_t) BufferLength, StringLengthPtr);
        case SQL_CURSOR_COMMIT_BEHAVIOR:
            *(SQLUSMALLINT *) InfoValuePtr = 0; // TODO:
            return SQL_ERROR;
        case SQL_TXN_ISOLATION_OPTION:
            *(SQLUINTEGER *) InfoValuePtr = 0; // TODO:
            return SQL_SUCCESS;
        case SQL_GETDATA_EXTENSIONS:
            *(SQLUINTEGER *) InfoValuePtr = SQL_GD_ANY_COLUMN | SQL_GD_ANY_ORDER /* | SQL_GD_BLOCK */ |
                                            SQL_GD_BOUND /* | SQL_GD_OUTPUT_PARAMS */;
            return SQL_SUCCESS;
        case SQL_MAX_COLUMN_NAME_LEN:
            *(SQLUSMALLINT *) InfoValuePtr = MAX_COLUMN_NAME_LEN;
            return SQL_SUCCESS;
        case SQL_MAX_CATALOG_NAME_LEN:
            *(SQLUSMALLINT *) InfoValuePtr = MAX_CATALOG_NAME_LEN;
            return SQL_SUCCESS;
        case SQL_MAX_SCHEMA_NAME_LEN:
            *(SQLUSMALLINT *) InfoValuePtr = MAX_SCHEMA_NAME_LEN;
            return SQL_SUCCESS;
        case SQL_MAX_TABLE_NAME_LEN:
            *(SQLUSMALLINT *) InfoValuePtr = MAX_TABLE_NAME_LEN;
            return SQL_SUCCESS;
        default: conn_append_err_format(conn, "HY000", 0, "General error:`%s[%d/0x%x]` not implemented yet",
                                        sql_info_type(InfoType), InfoType, InfoType);
            return SQL_ERROR;
    }
}

SQLRETURN conn_end_tran(
        conn_t *conn,
        SQLSMALLINT CompletionType) {
    switch (CompletionType) {
        case SQL_COMMIT:
            return _conn_commit(conn);
        case SQL_ROLLBACK:
            return _conn_rollback(conn);
        default: conn_append_err_format(conn, "HY000", 0, "General error:[DM]logic error");
            return SQL_ERROR;
    }
}

SQLRETURN conn_set_attr(
        conn_t *conn,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength) {
    (void) StringLength;

    switch (Attribute) {
        case SQL_ATTR_CONNECTION_TIMEOUT:
            if (0 == (SQLUINTEGER) (uintptr_t) ValuePtr) return SQL_SUCCESS;
            conn_append_err_format(conn, "01S02", 0,
                                   "Option value changed:`%u` for `SQL_ATTR_CONNECTION_TIMEOUT` is substituted by `0`",
                                   (SQLUINTEGER) (uintptr_t) ValuePtr);
            return SQL_SUCCESS_WITH_INFO;
        case SQL_ATTR_LOGIN_TIMEOUT:
            if (0 == (SQLUINTEGER) (uintptr_t) ValuePtr) return SQL_SUCCESS;
            conn_append_err_format(conn, "01S02", 0,
                                   "Option value changed:`%u` for `SQL_ATTR_LOGIN_TIMEOUT` is substituted by `0`",
                                   (SQLUINTEGER) (uintptr_t) ValuePtr);
            return SQL_SUCCESS_WITH_INFO;
        default: conn_append_err_format(conn, "HY000", 0, "General error:`%s[0x%x/%d]` not supported yet",
                                        sql_conn_attr(Attribute), Attribute, Attribute);
            return SQL_ERROR;
    }
}

void conn_clr_errs(conn_t *conn) {
    errs_clr(&conn->errs);
}

