/*
 * MIT License
 *
 * Copyright (c) 2022 freemine <freemine@yeah.net>
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

#ifndef _enums_h_
#define _enums_h_

#include <windows.h>
#include <sqlext.h>


const char *sql_c_data_type(SQLSMALLINT v) ;
const char *sql_data_type(SQLSMALLINT v) ;
const char *sql_handle_type(SQLSMALLINT v) ;
const char *sql_driver_completion(SQLUSMALLINT v) ;
const char *sql_odbc_version(SQLINTEGER v) ;
const char *sql_info_type(SQLUSMALLINT v) ;
const char *sql_completion_type(SQLSMALLINT v) ;
const char *sql_conn_attr(SQLINTEGER v) ;
const char *sql_cursor_type(SQLULEN v) ;
const char *sql_free_statement_option(SQLUSMALLINT v) ;
const char *sql_return_type(SQLRETURN v) ;
const char *sql_env_attr(SQLINTEGER v) ;
const char *sql_input_output_type(SQLSMALLINT v) ;
const char *sql_stmt_attr(SQLINTEGER v) ;
const char *sql_nullable(SQLSMALLINT v) ;
const char *sql_fetch_orientation(SQLSMALLINT v) ;


#endif // _enums_h_

