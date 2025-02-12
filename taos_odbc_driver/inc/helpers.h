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

#ifndef _helpers_h_
#define _helpers_h_

#include "macros.h"

#include <inttypes.h>
//#include <libgen.h>
#include <stdio.h>
#include <time.h>

// NOTE: you can use you own `LOG_IMPL` implementation by defining it before including this header file `helpers.h`
#ifndef LOG_IMPL
#define LOG_IMPL(...)          fprintf(stderr, ##__VA_ARGS__)
#endif

#ifdef __cplusplus         /* { */
#define ABORT_OR_THROW throw int(1)
#else                      /* }{ */
#include <stdlib.h>
#define ABORT_OR_THROW abort()
#endif                     /* } */

EXTERN_C_BEGIN

static inline const char* color_red(void)
{
  return "\033[1;31m";
}

static inline const char* color_green(void)
{
  return "\033[1;32m";
}

static inline const char* color_yellow(void)
{
  return "\033[1;33m";
}

static inline const char* color_reset(void)
{
  return "\033[0m";
}

#define LOGI(_file, _line, _func, _fmt, ...)           \
  LOG_IMPL("I:%s[%d]:%s(): " _fmt "\n",                \
      basename((char*)_file), _line, _func,            \
      ##__VA_ARGS__)

#define LOG LOGI

#define LOGD(_file, _line, _func, _fmt, ...)           \
  LOG_IMPL("D:%s[%d]:%s(): " _fmt "\n",                \
      basename((char*)_file), _line, _func,            \
      ##__VA_ARGS__)

#define LOGW(_file, _line, _func, _fmt, ...)           \
  LOG_IMPL("%sW%s:%s[%d]:%s(): " _fmt "\n",            \
      color_yellow(), color_reset(),                   \
      basename((char*)_file), _line, _func,            \
      ##__VA_ARGS__)

#define LOGE(_file, _line, _func, _fmt, ...)           \
  LOG_IMPL("%sE%s:%s[%d]:%s(): " _fmt "\n",            \
      color_red(), color_reset(),                      \
      basename((char*)_file), _line, _func,            \
      ##__VA_ARGS__)

#define LOGA(_file, _line, _func, _fmt, ...)           \
  LOG_IMPL("%sA%s:%s[%d]:%s(): " _fmt "\n",            \
      color_red(), color_reset(),                      \
      basename((char*)_file), _line, _func,            \
      ##__VA_ARGS__)

#define D(_fmt, ...) LOGD(__FILE__, __LINE__, __func__, _fmt, ##__VA_ARGS__)
#define W(_fmt, ...) do {                                     \
  LOGW(__FILE__, __LINE__, __func__,                          \
      "%s" _fmt "%s",                                         \
      color_yellow(), ##__VA_ARGS__, color_reset());          \
} while (0)
#define E(_fmt, ...) do {                                     \
  LOGE(__FILE__, __LINE__, __func__,                          \
      "%s" _fmt "%s",                                         \
      color_red(), ##__VA_ARGS__, color_reset());             \
} while (0)

#define A(_statement, _fmt, ...)                                        \
  do {                                                                  \
    if (!(_statement)) {                                                \
      LOGA(__FILE__, __LINE__, __func__,                                \
          "%sassertion failed%s: [%s]" _fmt "",                         \
          color_red(), color_reset(), #_statement, ##__VA_ARGS__);      \
      ABORT_OR_THROW;                                                   \
    }                                                                   \
  } while (0)

#define CHECK(_statement) do {                                          \
  D("%s ...", #_statement);                                             \
  if (_statement) {                                                     \
    E("%s => %sfailure%s", #_statement, color_red(), color_reset());    \
    return 1;                                                           \
  }                                                                     \
  D("%s => %ssuccess%s", #_statement, color_green(), color_reset());    \
} while (0)

#define LOG_CALL(fmt, ...)        D("" fmt " ...", ##__VA_ARGS__)
#define LOG_FINI(r, fmt, ...) do {                                             \
  if (r) {                                                                     \
    D("" fmt " => %sfailure%s", ##__VA_ARGS__, color_red(), color_reset());    \
  } else {                                                                     \
    D("" fmt " => %ssuccess%s", ##__VA_ARGS__, color_green(), color_reset());  \
  }                                                                            \
} while (0)

char *tod_strptime(const char *s, const char *format, struct tm *tm);

EXTERN_C_END

#endif // _helpers_h_

