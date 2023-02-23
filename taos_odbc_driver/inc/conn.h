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

#ifndef _conn_h_
#define _conn_h_

#include "env.h"
#include "utils.h"

#include <string.h>


static inline void connection_cfg_release(connection_cfg_t *conn_str) {
    if (!conn_str) return;
    TOD_SAFE_FREE(conn_str->dsn);
    printf("connection_cfg_release dsn %lld\n",(unsigned long)conn_str->dsn);
//    TOD_SAFE_FREE(conn_str->db);
//    printf("connection_cfg_release db %lld\n",(unsigned long)conn_str->db);
//    TOD_SAFE_FREE(conn_str->ip);
//    printf("connection_cfg_release ip %lld\n",(unsigned long)conn_str->ip);
//    TOD_SAFE_FREE(conn_str->driver);
//    printf("connection_cfg_release driver %lld\n",(unsigned long)conn_str->driver);
//    TOD_SAFE_FREE(conn_str->uid);
//    TOD_SAFE_FREE(conn_str->pwd);
}

static inline void connection_cfg_transfer(connection_cfg_t *from, connection_cfg_t *to) {
    if (from == to) return;
    connection_cfg_release(to);
    *to = *from;
    memset(from, 0, sizeof(*from));
}

conn_t *conn_create(env_t *env);

conn_t *conn_ref(conn_t *conn);

conn_t *conn_unref(conn_t *conn);

SQLRETURN conn_free(conn_t *conn);

void conn_clr_errs(conn_t *conn);

SQLRETURN conn_driver_connect(
        conn_t *conn,
        SQLHWND WindowHandle,
        SQLCHAR *InConnectionString,
        SQLSMALLINT StringLength1,
        SQLCHAR *OutConnectionString,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLength2Ptr,
        SQLUSMALLINT DriverCompletion);

void conn_disconnect(conn_t *conn);

SQLRETURN conn_get_diag_rec(
        conn_t *conn,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr);

SQLRETURN conn_alloc_stmt(conn_t *conn, SQLHANDLE *OutputHandle);

SQLRETURN conn_alloc_desc(conn_t *conn, SQLHANDLE *OutputHandle);

SQLRETURN conn_connect(
        conn_t *conn,
        SQLCHAR *ServerName,
        SQLSMALLINT NameLength1,
        SQLCHAR *UserName,
        SQLSMALLINT NameLength2,
        SQLCHAR *Authentication,
        SQLSMALLINT NameLength3);

SQLRETURN conn_get_info(
        conn_t *conn,
        SQLUSMALLINT InfoType,
        SQLPOINTER InfoValuePtr,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *StringLengthPtr);

SQLRETURN conn_end_tran(
        conn_t *conn,
        SQLSMALLINT CompletionType);

SQLRETURN conn_set_attr(
        conn_t *conn,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength);


#endif //  _conn_h_

