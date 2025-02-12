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

#ifndef _utils_h_
#define _utils_h_

#include <stdarg.h>
#include <stdlib.h>
#include <string.h>

#define TOD_SAFE_FREE(_p) if (_p) { free(_p); _p = NULL; }

typedef struct fixed_buf_s fixed_buf_t;
struct fixed_buf_s {
  char              *buf;
  size_t             cap;
  size_t             nr;
};

#define fixed_buf_sprintf(_buf, _fmt, ...)            \
  fixed_buf_t *_fixed_buf = _buf;                         \
  char *__buf = _fixed_buf->buf;                          \
  size_t _nn = 0;                                         \
  if (__buf) {                                            \
    __buf += _fixed_buf->nr;                              \
    _nn = _fixed_buf->cap - _fixed_buf->nr;               \
  }                                                       \
  int _n = snprintf(__buf, _nn, _fmt, ##__VA_ARGS__);     \
  if (__buf) {                                            \
    if (_n>0 && (size_t)_n < _nn) {                       \
      _fixed_buf->nr += _n;                               \
    } else {                                              \
      _fixed_buf->nr += _nn - 1;                          \
    }                                                     \
  }                                                       \


typedef struct static_pool_s                   static_pool_t;

static_pool_t* static_pool_create(size_t cap) ;
void static_pool_destroy(static_pool_t *pool) ;
void static_pool_reset(static_pool_t *pool) ;
unsigned char* static_pool_malloc(static_pool_t *pool, size_t sz) ;
unsigned char* static_pool_calloc(static_pool_t *pool, size_t sz) ;
unsigned char* static_pool_malloc_align(static_pool_t *pool, size_t sz, size_t align) ;
unsigned char* static_pool_calloc_align(static_pool_t *pool, size_t sz, size_t align) ;

typedef struct buf_s               buf_t;
struct buf_s {
  char               *base;
  size_t              cap;
};

void buf_release(buf_t *buf) ;
void* buf_realloc(buf_t *buf, size_t sz) ;

typedef struct buffers_s           buffers_t;
struct buffers_s {
  buf_t             **bufs;
  size_t              cap;
};

void buffers_release(buffers_t *buffers) ;
void* buffers_realloc(buffers_t *buffers, size_t idx, size_t sz) ;

typedef struct wildex_s           wildex_t;
int wildcomp(wildex_t **pwild, const char *wildex) ;
int wildexec_n(wildex_t *wild, const char *str, size_t len) ;
static inline int wildexec(wildex_t *wild, const char *str)
{
  return wildexec_n(wild, str, strlen(str));
}

void wildfree(wildex_t *wild) ;

int table_type_parse(const char *table_type, int *table, int *stable) ;

typedef struct buffer_s           buffer_t;
struct buffer_s {
  char        *base;
  size_t       sz;
  size_t       nr;
};

void buffer_reset(buffer_t *str) ;
void buffer_release(buffer_t *str) ;
int buffer_expand(buffer_t *str, size_t nr) ;

int buffer_concat_n(buffer_t *str, const char *s, size_t len) ;
static inline int buffer_concat(buffer_t *str, const char *s)
{
  return buffer_concat_n(str, s, strlen(s));
}
int buffer_vconcat(buffer_t *str, const char *fmt, va_list ap) ;
int buffer_concat_fmt(buffer_t *str, const char *fmt, ...) ;
// replace "'" in s with "''"
int buffer_concat_replacement_n(buffer_t *str, const char *s, size_t len) ;
static inline int buffer_concat_replacement(buffer_t *str, const char *s)
{
  return buffer_concat_replacement_n(str, s, strlen(s));
}

int buffer_copy_n(buffer_t *str, const unsigned char *mem, size_t len) ;
static inline int buffer_copy(buffer_t *str, const char *s)
{
  str->nr = 0;
  return buffer_copy_n(str, (const unsigned char*)s, strlen(s));
}

#endif // _utils_h_

