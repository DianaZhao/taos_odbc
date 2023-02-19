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

#ifndef _internal_h_
#define _internal_h_


#include "list.h"
#include "utils.h"


#include <stdatomic.h>
#include <taos.h>
#include <stdio.h>
#include <sqltypes.h>

#define MAX_CATALOG_NAME_LEN        64
#define MAX_SCHEMA_NAME_LEN         64
#define MAX_TABLE_NAME_LEN         192
#define MAX_COLUMN_NAME_LEN         64

#define conn_data_source(_conn) _conn->cfg.dsn ? _conn->cfg.dsn : (_conn->cfg.driver ? _conn->cfg.driver : "")

typedef struct err_s err_t;
struct err_s {
    int err;
    const char *estr;
    SQLCHAR sql_state[6];

    char buf[1024];

    struct tod_list_head node;
};

typedef struct errs_s errs_t;
struct errs_s {
    struct tod_list_head errs;
    struct tod_list_head frees;
};

void errs_init(errs_t *errs);

void errs_append_x(errs_t *errs, const char *file, int line, const char *func, const char *data_source,
                   const char *sql_state, int e, const char *estr);

void errs_clr_x(errs_t *errs);

void errs_release_x(errs_t *errs);

SQLRETURN errs_get_diag_rec_x(
        errs_t *errs,
        SQLSMALLINT RecNumber,
        SQLCHAR *SQLState,
        SQLINTEGER *NativeErrorPtr,
        SQLCHAR *MessageText,
        SQLSMALLINT BufferLength,
        SQLSMALLINT *TextLengthPtr);

#define errs_append(_errs, _data_source, _sql_state, _e, _estr)                              \
                                                                                           \
    errs_append_x(_errs, __FILE__, __LINE__, __func__, _data_source, _sql_state, _e, _estr); \


#define errs_append_format(_errs, _data_source, _sql_state, _e, _fmt, ...)    \
  {                                                                          \
    char _buf[1024];                                                          \
    snprintf(_buf, sizeof(_buf), "" _fmt "", ##__VA_ARGS__);                  \
    errs_append(_errs, _data_source, _sql_state, _e, _buf);                   \
  }

#define errs_oom(_errs, _data_source) errs_append(_errs, _data_source, "HY001", 0, "Memory allocation error")
#define errs_niy(_errs, _data_source) errs_append(_errs, _data_source, "HY000", 0, "General error:Not implemented yet")

#define errs_clr(_errs) errs_clr_x(_errs)

#define errs_release(_errs) errs_release_x(_errs)

#define errs_get_diag_rec(_errs, _RecNumber, _SQLSTATE, _NativeErrorPtr, _MessageText, _BufferLength, _TextLengthPtr) \
  errs_get_diag_rec_x(_errs, _RecNumber, _SQLSTATE, _NativeErrorPtr, _MessageText, _BufferLength, _TextLengthPtr)

#define env_append_err(_env, _sql_state, _e, _estr) errs_append(&_env->errs, "", _sql_state, _e, _estr)

#define env_append_err_format(_env, _sql_state, _e, _fmt, ...) errs_append_format(&_env->errs, "", _sql_state, _e, _fmt, ##__VA_ARGS__)

#define env_oom(_env)                                 \
  ({                                                  \
    env_t *__env = _env;                              \
    errs_oom(&__env->errs, "");                       \
  })

#define conn_append_err(_conn, _sql_state, _e, _estr)                                \
  {                                                                                 \
    conn_t *__conn = _conn;                                                          \
    errs_append(&__conn->errs, conn_data_source(__conn), _sql_state, _e, _estr);     \
  }

#define conn_append_err_format(_conn, _sql_state, _e, _fmt, ...)                                      \
  {                                                                                                  \
    conn_t *__conn = _conn;                                                                           \
    errs_append_format(&__conn->errs, conn_data_source(__conn), _sql_state, _e, _fmt, ##__VA_ARGS__); \
  }

#define conn_oom(_conn)                                  \
                                                       \
    conn_t *__conn = _conn;                              \
    errs_oom(&__conn->errs, conn_data_source(__conn));   \


#define conn_niy(_conn)                                        \
  ({                                                           \
    conn_t *__conn = _conn;                                    \
    errs_niy(&__conn->errs, conn_data_source(__conn));         \
  })

#define conn_nsy(_conn)                                        \
  ({                                                           \
    conn_t *__conn = _conn;                                    \
    errs_nsy(&__conn->errs, conn_data_source(__conn));         \
  })

#define stmt_append_err(_stmt, _sql_state, _e, _estr)                                \
  {                                                                                 \
    stmt_t *__stmt = _stmt;                                                          \
    conn_t *__conn = __stmt->conn;                                                   \
    errs_append(&__stmt->errs, conn_data_source(__conn), _sql_state, _e, _estr);     \
  }

#define stmt_append_err_format(_stmt, _sql_state, _e, _fmt, ...)                                      \
  {                                                                                                  \
    stmt_t *__stmt = _stmt;                                                                           \
    conn_t *__conn = __stmt->conn;                                                                    \
    errs_append_format(&__stmt->errs, conn_data_source(__conn), _sql_state, _e, _fmt, ##__VA_ARGS__); \
  }

#define stmt_oom(_stmt)                                        \
  {                                                           \
    stmt_t *__stmt = _stmt;                                    \
    errs_oom(&__stmt->errs, conn_data_source(__stmt->conn));   \
  }

#define stmt_niy(_stmt)                                        \
  ({                                                           \
    stmt_t *__stmt = _stmt;                                    \
    errs_niy(&__stmt->errs, conn_data_source(__stmt->conn));   \
  })

#define stmt_nsy(_stmt)                                        \
  ({                                                           \
    stmt_t *__stmt = _stmt;                                    \
    errs_nsy(&__stmt->errs, conn_data_source(__stmt->conn));   \
  })



//#define sql_succeeded(_sr) { SQLRETURN __sr = _sr; (__sr==SQL_SUCCESS || __sr==SQL_SUCCESS_WITH_INFO); }




typedef struct env_s env_t;
struct env_s {
    atomic_int refc;

    atomic_int conns;

    errs_t errs;

    unsigned int debug: 1;
    unsigned int debug_flex: 1;
    unsigned int debug_bison: 1;
};


typedef struct desc_header_s desc_header_t;
struct desc_header_s {
    // header fields settable by SQLSetStmtAttr
    SQLULEN DESC_ARRAY_SIZE;
    SQLUSMALLINT *DESC_ARRAY_STATUS_PTR;
    SQLULEN *DESC_BIND_OFFSET_PTR;
    SQLULEN DESC_BIND_TYPE;
    SQLULEN *DESC_ROWS_PROCESSED_PTR;

    // header fields else
    SQLUSMALLINT DESC_COUNT;
};


typedef struct tsdb_to_sql_c_state_s tsdb_to_sql_c_state_t;
struct tsdb_to_sql_c_state_s {
    TAOS_FIELD *field;
    int i_col;
    int i_row;

    SQLSMALLINT TargetType;
    SQLPOINTER TargetValuePtr;
    SQLLEN BufferLength;
    SQLLEN *StrLen_or_IndPtr;

    const char *data;
    int len;

    buffer_t cache;
};

typedef struct stmt_s stmt_t;

typedef SQLRETURN (*conv_from_tsdb_to_sql_c_f)(stmt_t *stmt, tsdb_to_sql_c_state_t *conv_state);


typedef struct desc_record_s desc_record_t;
typedef struct sql_c_to_tsdb_meta_s sql_c_to_tsdb_meta_t;

struct desc_record_s {
    SQLSMALLINT DESC_TYPE;
    SQLSMALLINT DESC_CONCISE_TYPE;
    SQLULEN DESC_LENGTH;
    SQLSMALLINT DESC_PRECISION;
    SQLSMALLINT DESC_SCALE;
    SQLLEN DESC_OCTET_LENGTH;
    SQLPOINTER DESC_DATA_PTR;
    SQLLEN *DESC_INDICATOR_PTR;
    SQLLEN *DESC_OCTET_LENGTH_PTR;
    SQLSMALLINT DESC_PARAMETER_TYPE;

    conv_from_tsdb_to_sql_c_f conv;

    buf_t data_buffer;
    buf_t len_buffer;
    buf_t ind_buffer;

    SQLRETURN (*create_buffer_array)(stmt_t *stmt, desc_record_t *record, int rows, TAOS_MULTI_BIND *mb);

    SQLRETURN (*create_length_array)(stmt_t *stmt, desc_record_t *record, int rows, TAOS_MULTI_BIND *mb);

    SQLRETURN (*convf)(stmt_t *stmt, sql_c_to_tsdb_meta_t *meta);

    unsigned int bound: 1;
};

struct sql_c_to_tsdb_meta_s {
    char *src_base;
    SQLLEN src_len;
    desc_record_t *IPD_record;
    TAOS_FIELD_E *field;
    char *dst_base;
    int32_t *dst_len;
};

typedef struct descriptor_s descriptor_t;
struct descriptor_s {
    desc_header_t header;

    desc_record_t *records;
    size_t cap;
};

typedef struct connection_cfg_s connection_cfg_t;

struct connection_cfg_s {
    char *driver;
    char *dsn;
    char *uid;
    char *pwd;
    char *ip;
    char *db;
    int port;

    // NOTE: 1.this is to hack node.odbc, which maps SQL_TINYINT to SQL_C_UTINYINT
    //       2.node.odbc does not call SQLGetInfo/SQLColAttribute to get signess of integers
    unsigned int unsigned_promotion: 1;
    unsigned int cache_sql: 1;
};

typedef struct conn_s conn_t;
struct conn_s {
    atomic_int refc;
    atomic_int stmts;
    atomic_int descs;
    atomic_int outstandings;

    env_t *env;

    connection_cfg_t cfg;

    errs_t errs;

    TAOS *taos;

    unsigned int fmt_time: 1;
};

typedef struct desc_s desc_t;
struct desc_s {
    atomic_int refc;

    descriptor_t descriptor;

    conn_t *conn;

    // https://learn.microsoft.com/en-us/sql/odbc/reference/develop-app/types-of-descriptors?view=sql-server-ver16
    // `A row descriptor in one statement can serve as a parameter descriptor in another statement.`
    struct tod_list_head associated_stmts_as_ARD; // struct stmt_s*
    struct tod_list_head associated_stmts_as_APD; // struct stmt_s*
};


typedef struct rowset_s rowset_t;
struct rowset_s {
    int i_row;
};

// NOTE: this exists because of https://github.com/taosdata/TDengine/issues/17890
typedef struct post_filter_s post_filter_t;

typedef SQLRETURN (*post_filter_f)(stmt_t *stmt, int row, void *ctx, int *filter);

typedef void (*post_filter_destroy_f)(stmt_t *stmt, void *ctx);

struct post_filter_s {
    void *ctx;
    post_filter_f post_filter;
    post_filter_destroy_f post_filter_destroy;
};

struct stmt_s {
    atomic_int refc;

    conn_t *conn;

    errs_t errs;

    struct tod_list_head associated_APD_node;
    desc_t *associated_APD;

    struct tod_list_head associated_ARD_node;
    desc_t *associated_ARD;

    descriptor_t APD, IPD;
    descriptor_t ARD, IRD;

    descriptor_t *current_APD;
    descriptor_t *current_ARD;

    tsdb_to_sql_c_state_t current_for_get_data;
    buffer_t cache;

    char *sql;

    TAOS_STMT *stmt;
    // for non-insert-parameterized-statement
    int nr_params;

    // for insert-parameterized-statement
    char *subtbl;
    TAOS_FIELD_E *tag_fields;
    int nr_tag_fields;
    TAOS_FIELD_E *col_fields;
    int nr_col_fields;

    TAOS_MULTI_BIND *mbs;
    size_t cap_mbs;
    size_t nr_mbs;

    post_filter_t post_filter;

    TAOS_RES *res;
    SQLLEN affected_row_count;
    SQLSMALLINT col_count;
    TAOS_FIELD *fields;
    int *lengths;
    int time_precision;

    TAOS_ROW rows;
    int nr_rows;

    rowset_t rowset;

    unsigned int prepared: 1;
    unsigned int is_insert_stmt: 1;
    unsigned int res_is_from_taos_query: 1;
    unsigned int subtbl_required: 1;
};

static inline const char *color_red(void) {
    return "\033[1;31m";
}

static inline const char *color_green(void) {
    return "\033[1;32m";
}

static inline const char *color_yellow(void) {
    return "\033[1;33m";
}

static inline const char *color_reset(void) {
    return "\033[0m";
}

static inline char *basename(char *fileName) {
    char *bn = fileName + strlen(fileName);
    while ((char) (*bn) != "\\" && bn >= fileName) {
        bn--;
    }
    if ((char) (*bn) != "\\") {
        bn++;
    }
    return bn;
}

int tod_get_debug(void);

#define LOG_IMPL(fmt, ...) do {                  \
  if (!tod_get_debug()) break;                   \
  fprintf(stderr, fmt, ##__VA_ARGS__);           \
} while (0)

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


static inline void assert(BOOL _statement, const char *function_name) {
    if (!_statement) {
        fprintf((__acrt_iob_func(2)),
                "%sA%s:%s[%d]:%s(): " "%sassertion failed%s: [%s]" "internal logic error" "" "\n", color_red(),
                color_reset(), basename((char *) "_file_name_"), 48, function_name, color_red(), color_reset(),
                "prev >= 1");
        abort();
    }
}

static inline char *strndup(const char *s1, size_t n) {
    char *copy = (char *) malloc(n + 1);
    memcpy(copy, s1, n);
    copy[n] = 0;
    return copy;
}

#endif // _internal_h_
