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

#include "env.h"
#include "conn.h"
#include <stdlib.h>
#include <stdio.h>
#include <sqlext.h>
#include "taos.h"
#include "pthread.h"
#include "enums.h"
#include <wtypes.h>

static unsigned int _taos_odbc_debug = 0;
static unsigned int _taos_odbc_debug_flex = 0;
static unsigned int _taos_odbc_debug_bison = 0;
static unsigned int _taos_init_failed = 0;

static void _exit_routine(void) {
    taos_cleanup();
}

static void _init_once(void) {
    if (getenv("TAOS_ODBC_DEBUG")) _taos_odbc_debug = 1;
    if (getenv("TAOS_ODBC_DEBUG_FLEX")) _taos_odbc_debug_flex = 1;
    if (getenv("TAOS_ODBC_DEBUG_BISON")) _taos_odbc_debug_bison = 1;
    if (taos_init()) {
        _taos_init_failed = 1;
        return;
    }
    atexit(_exit_routine);
}

static int _env_init(env_t *env) {
    static pthread_once_t once;
    pthread_once(&once, _init_once);

    // TODO:

    env->debug = _taos_odbc_debug;
    env->debug_flex = _taos_odbc_debug_flex;
    env->debug_bison = _taos_odbc_debug_bison;

    errs_init(&env->errs);

    if (_taos_init_failed) return -1;

    env->refc = 1;

    return 0;
}

static void _env_release(env_t *env) {
    int conns = atomic_load(&env->conns);
    errs_release(&env->errs);
    return;
}

int tod_get_debug(void) {
    return !!_taos_odbc_debug;
}

int env_get_debug(env_t *env) {
    return !!env->debug;
}

int env_get_debug_flex(env_t *env) {
    return !!env->debug_flex;
}

int env_get_debug_bison(env_t *env) {
    return !!env->debug_bison;
}

env_t *env_create(void) {
    env_t *env = (env_t *) calloc(1, sizeof(env_t));
    if (!env) return NULL;

    int r = _env_init(env);
    if (r) {
        _env_release(env);
        free(env);
        return NULL;
    }

    return env;
}

env_t *env_ref(env_t *env) {
    int prev = atomic_fetch_add(&env->refc, 1);
    assert(prev > 0);
    return env;
}

env_t *env_unref(env_t *env) {
    int prev = atomic_fetch_sub(&env->refc, 1);
    if (prev > 1) return env;

    _env_release(env);
    free(env);

    return NULL;
}

SQLRETURN env_free(env_t *env) {
    int conns = atomic_load(&env->conns);
    if (conns) {
//        env_append_err_format(env, "HY000", 0, "General error:%d connections are still connected or allocated", conns);
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "General error:%d connections are still connected or allocated" "", conns);
        return SQL_ERROR;
    }

    env_unref(env);
    return SQL_SUCCESS;
}

static SQLRETURN _env_commit(env_t *env) {
    int conns = atomic_load(&env->conns);
    if (conns == 0) {
//        env_append_err_format(env, "01000", 0, "General warning:no outstanding connection");
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "General warning:no outstanding connection" "");
        errs_append_x(&env->errs, "_file_name_", 141, "_function_name_", "", "01000", 0, _buf);
        return SQL_SUCCESS_WITH_INFO;
    }

//    env_append_err_format(env, "25S02", 0, "Transaction is still active");
    char _buf[1024];
    snprintf(_buf, sizeof(_buf), "" "Transaction is still active" "");
    errs_append_x(&env->errs, "_file_name_", 148, "_function_name_", "", "25S02", 0, _buf);
    return SQL_ERROR;
}

static SQLRETURN _env_rollback(env_t *env) {
    int conns = atomic_load(&env->conns);
    if (conns == 0) {
//        env_append_err_format(env, "01000", 0, "General warning:no outstanding connection");
        char _buf[1024];
        snprintf(_buf, sizeof(_buf), "" "General warning:no outstanding connection" "");
        errs_append_x(&env->errs, "_file_name_", 152, "_function_name_", "", "01000", 0, _buf);
        return SQL_SUCCESS_WITH_INFO;
    }

//    env_append_err_format(env, "25S01", 0, "Transaction state unknown");
    char _buf[1024];
    snprintf(_buf, sizeof(_buf), "" "Transaction state unknown" "");
    errs_append_x(&env->errs, "_file_name_", 165, "_function_name_", "", "25S01", 0, _buf);
    return SQL_ERROR;
}

SQLRETURN env_get_diag_rec(
        env_t *env,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr) {
    return errs_get_diag_rec(&env->errs, RecNumber, SQLState, NativeErrorPtr, MessageText, BufferLength, TextLengthPtr);
}

static SQLRETURN _env_set_odbc_version(env_t *env, SQLINTEGER odbc_version) {
    switch (odbc_version) {
        case SQL_OV_ODBC3:
            return SQL_SUCCESS;
        default:
//            env_append_err_format(env, "01S02", 0,
//                                  "Option value changed:`%s[0x%x/%d]` is substituted by `SQL_OV_ODBC3`",
//                                  sql_odbc_version(odbc_version), odbc_version, odbc_version);
            char _buf[1024];
            snprintf(_buf, sizeof(_buf), "" "Option value changed:`%s[0x%x/%d]` is substituted by `SQL_OV_ODBC3`" "",
                     odbc_version, odbc_version, odbc_version);
            errs_append_x(&env->errs, "_file_name_", 178, "_function_name_", "", "01S02", 0, _buf);
            return SQL_SUCCESS_WITH_INFO;
    }
}

SQLRETURN env_set_attr(
        env_t *env,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength) {
    (void) StringLength;

    switch (Attribute) {
        case SQL_ATTR_ODBC_VERSION:
            return _env_set_odbc_version(env, (SQLINTEGER) (size_t) ValuePtr);

        default:
//            env_append_err_format(env, "01S02", 0, "Optional value changed:`%s[0x%x/%d]` is substituted by default",
//                                  sql_env_attr(Attribute), Attribute, Attribute);
            char _buf[1024];
            snprintf(_buf, sizeof(_buf), "" "Optional value changed:`%s[0x%x/%d]` is substituted by default" "",
                    Attribute, Attribute, Attribute);
//            errs_append_x(&env->errs, "_file_name_", 196, "_function_name_", "", "01S02", 0, _buf);
            return SQL_SUCCESS_WITH_INFO;
    }
}

SQLRETURN env_end_tran(env_t *env, SQLSMALLINT CompletionType) {
    switch (CompletionType) {
        case SQL_COMMIT:
            return _env_commit(env);
        case SQL_ROLLBACK:
            return _env_rollback(env);
        default:
//            env_append_err_format(env, "HY000", 0, "General error:[DM]logic error");
            char _buf[1024];
            snprintf(_buf, sizeof(_buf), "" "General error:[DM]logic error" "");
//            errs_append_x(&env->errs, "_file_name_", 208, "_function_name_", "", "HY000", 0, _buf);
            return SQL_ERROR;
    }
}

SQLRETURN env_alloc_conn(env_t *env, SQLHANDLE *OutputHandle) {
    *OutputHandle = SQL_NULL_HANDLE;

    conn_t *conn = conn_create(env);
    if (conn == NULL) return SQL_ERROR;

    *OutputHandle = (SQLHANDLE) conn;
    return SQL_SUCCESS;
}

void env_clr_errs(env_t *env) {
    errs_clr(&env->errs);
}

