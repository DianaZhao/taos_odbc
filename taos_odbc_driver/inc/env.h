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

#ifndef _env_h_
#define _env_h_

#include <windows.h>
#include <sql.h>
#include <sqltypes.h>
#include "stdatomic.h"
#include "internal.h"

env_t *env_create(void);

env_t *env_ref(env_t *env);

env_t *env_unref(env_t *env);

SQLRETURN env_free(env_t *env);

void env_clr_errs(env_t *env);

int env_get_debug(env_t *env);

int env_get_debug_flex(env_t *env);

int env_get_debug_bison(env_t *env);

SQLRETURN env_get_diag_rec(
        env_t *env,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr);

SQLRETURN env_set_attr(
        env_t *env,
        SQLINTEGER Attribute,
        SQLPOINTER ValuePtr,
        SQLINTEGER StringLength);

SQLRETURN env_end_tran(env_t *env, SQLSMALLINT CompletionType);

SQLRETURN env_alloc_conn(env_t *env, SQLHANDLE *OutputHandle);

//EXTERN_C_END

#endif // _env_h_

