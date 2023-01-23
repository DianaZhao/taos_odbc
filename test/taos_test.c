//
// Created by diana on 1/5/2023.
//

#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <taos.h>

typedef int16_t VarDataLenT;

#define TSDB_NCHAR_SIZE sizeof(int32_t)
#define VARSTR_HEADER_SIZE sizeof(VarDataLenT)

#define GET_FLOAT_VAL(x) (*(float *)(x))
#define GET_DOUBLE_VAL(x) (*(double *)(x))

#define varDataLen(v) ((VarDataLenT *)(v))[0]

int printRow(char *str, TAOS_ROW row, TAOS_FIELD *fields, int numFields) {
    int len = 0;
    char split = ' ';

    for (int i = 0; i < numFields; ++i) {
        if (i > 0) {
            str[len++] = split;
        }

        if (row[i] == NULL) {
            len += sprintf(str + len, "%s", "NULL");
            continue;
        }

        switch (fields[i].type) {
            case TSDB_DATA_TYPE_TINYINT:
                len += sprintf(str + len, "%d", *((int8_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_UTINYINT:
                len += sprintf(str + len, "%u", *((uint8_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_SMALLINT:
                len += sprintf(str + len, "%d", *((int16_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_USMALLINT:
                len += sprintf(str + len, "%u", *((uint16_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_INT:
                len += sprintf(str + len, "%d", *((int32_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_UINT:
                len += sprintf(str + len, "%u", *((uint32_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_BIGINT:
                len += sprintf(str + len, "%ld", *((int64_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_UBIGINT:
                len += sprintf(str + len, "%llu", *((uint64_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_FLOAT: {
                float fv = 0;
                fv = GET_FLOAT_VAL(row[i]);
                len += sprintf(str + len, "%f", fv);
            }
                break;

            case TSDB_DATA_TYPE_DOUBLE: {
                double dv = 0;
                dv = GET_DOUBLE_VAL(row[i]);
                len += sprintf(str + len, "%lf", dv);
            }
                break;

            case TSDB_DATA_TYPE_BINARY:
            case TSDB_DATA_TYPE_NCHAR: {
                int32_t charLen = varDataLen((char *) row[i] - VARSTR_HEADER_SIZE);
                memcpy(str + len, row[i], charLen);
                len += charLen;
            }
                break;

            case TSDB_DATA_TYPE_TIMESTAMP:
                len += sprintf(str + len, "%ld", *((int64_t *) row[i]));
                break;

            case TSDB_DATA_TYPE_BOOL:
                len += sprintf(str + len, "%d", *((int8_t *) row[i]));
            default:
                break;
        }
    }

    return len;
}

/**
 * @brief print column name and values of each row
 *
 * @param res
 * @return int
 */
static int printResult(TAOS_RES *res) {
    int numFields = taos_num_fields(res);
    TAOS_FIELD *fields = taos_fetch_fields(res);
    char header[256] = {0};
    int len = 0;
    for (int i = 0; i < numFields; ++i) {
        len += sprintf(header + len, "%s ", fields[i].name);
    }
    puts(header);

    TAOS_ROW row = NULL;
    while ((row = taos_fetch_row(res))) {
        char temp[256] = {0};
        printRow(temp, row, fields, numFields);
        puts(temp);
    }
}

int insertData(TAOS *taos) {
    struct {
        int64_t ts;
        int8_t b;
        int8_t v1;
        int16_t v2;
        int32_t v4;
        int64_t v8;
        float f4;
        double f8;
        char bin[40];
        char blob[80];
    } v = {0};

    int32_t boolLen = sizeof(int8_t);
    int32_t sintLen = sizeof(int16_t);
    int32_t intLen = sizeof(int32_t);
    int32_t bintLen = sizeof(int64_t);
    int32_t floatLen = sizeof(float);
    int32_t doubleLen = sizeof(double);
    int32_t binLen = sizeof(v.bin);
    int32_t ncharLen = 30;

    TAOS_STMT *stmt = taos_stmt_init(taos);
    TAOS_MULTI_BIND params[10];
    params[0].buffer_type = TSDB_DATA_TYPE_TIMESTAMP;
    params[0].buffer_length = sizeof(v.ts);
    params[0].buffer = &v.ts;
    params[0].length = &bintLen;
    params[0].is_null = NULL;
    params[0].num = 1;

    params[1].buffer_type = TSDB_DATA_TYPE_BOOL;
    params[1].buffer_length = sizeof(v.b);
    params[1].buffer = &v.b;
    params[1].length = &boolLen;
    params[1].is_null = NULL;
    params[1].num = 1;

    params[2].buffer_type = TSDB_DATA_TYPE_TINYINT;
    params[2].buffer_length = sizeof(v.v1);
    params[2].buffer = &v.v1;
    params[2].length = &boolLen;
    params[2].is_null = NULL;
    params[2].num = 1;

    params[3].buffer_type = TSDB_DATA_TYPE_SMALLINT;
    params[3].buffer_length = sizeof(v.v2);
    params[3].buffer = &v.v2;
    params[3].length = &sintLen;
    params[3].is_null = NULL;
    params[3].num = 1;

    params[4].buffer_type = TSDB_DATA_TYPE_INT;
    params[4].buffer_length = sizeof(v.v4);
    params[4].buffer = &v.v4;
    params[4].length = &intLen;
    params[4].is_null = NULL;
    params[4].num = 1;

    params[5].buffer_type = TSDB_DATA_TYPE_BIGINT;
    params[5].buffer_length = sizeof(v.v8);
    params[5].buffer = &v.v8;
    params[5].length = &bintLen;
    params[5].is_null = NULL;
    params[5].num = 1;

    params[6].buffer_type = TSDB_DATA_TYPE_FLOAT;
    params[6].buffer_length = sizeof(v.f4);
    params[6].buffer = &v.f4;
    params[6].length = &floatLen;
    params[6].is_null = NULL;
    params[6].num = 1;

    params[7].buffer_type = TSDB_DATA_TYPE_DOUBLE;
    params[7].buffer_length = sizeof(v.f8);
    params[7].buffer = &v.f8;
    params[7].length = &doubleLen;
    params[7].is_null = NULL;
    params[7].num = 1;

    params[8].buffer_type = TSDB_DATA_TYPE_BINARY;
    params[8].buffer_length = sizeof(v.bin);
    params[8].buffer = v.bin;
    params[8].length = &binLen;
    params[8].is_null = NULL;
    params[8].num = 1;

    strcpy(v.blob, "一二三四五六七八九十");
    params[9].buffer_type = TSDB_DATA_TYPE_NCHAR;
    params[9].buffer_length = sizeof(v.blob);
    params[9].buffer = v.blob;
    params[9].length = &ncharLen;
    params[9].is_null = NULL;
    params[9].num = 1;

    char is_null = 1;

    const char *sql = "insert into m1 values(?,?,?,?,?,?,?,?,?,?)";
    int code = taos_stmt_prepare(stmt, sql, 0);
    if (code != 0) {
        printf("failed to execute taos_stmt_prepare. code:0x%x\n", code);
    }
    v.ts = 1591060628000;
    for (int i = 0; i < 10; ++i) {
        v.ts += 1;
        for (int j = 1; j < 10; ++j) {
            params[j].is_null = ((i == j) ? &is_null : 0);
        }
        v.b = (int8_t) i % 2;
        v.v1 = (int8_t) i;
        v.v2 = (int16_t) (i * 2);
        v.v4 = (int32_t) (i * 4);
        v.v8 = (int64_t) (i * 8);
        v.f4 = (float) (i * 40);
        v.f8 = (double) (i * 80);
        for (int j = 0; j < sizeof(v.bin); ++j) {
            v.bin[j] = (char) (i + '0');
        }

        taos_stmt_bind_param(stmt, params);
        taos_stmt_add_batch(stmt);
    }
    if (taos_stmt_execute(stmt) != 0) {
        printf("failed to execute insert statement.\n");
        return (-1);
    }
    taos_stmt_close(stmt);

    // query the records
    stmt = taos_stmt_init(taos);
    taos_stmt_prepare(stmt, "SELECT * FROM m1 WHERE v1 > ? AND v2 < ?", 0);
    v.v1 = 5;
    v.v2 = 15;
    taos_stmt_bind_param(stmt, params + 2);
    if (taos_stmt_execute(stmt) != 0) {
        printf("failed to execute select statement.\n");
        return (-1);
    }

    TAOS_RES *result = taos_stmt_use_result(stmt);

    TAOS_ROW row;
    int rows = 0;
    int num_fields = taos_num_fields(result);
    TAOS_FIELD *fields = taos_fetch_fields(result);

    // fetch the records row by row
    while ((row = taos_fetch_row(result))) {
        char temp[256] = {0};
        rows++;
        taos_print_row(temp, row, fields, num_fields);
        printf("%s\n", temp);
    }
    if (rows == 2) {
        printf("two rows are fetched as expectation\n");
    } else {
        printf("expect two rows, but %d rows are fetched\n", rows);
    }

    taos_free_result(result);
    taos_stmt_close(stmt);
}

int main(int argc, char *argv[]) {
    //HMODULE m = LoadLibrary("D:\\workspaces\\VSProjects\\taos_odbc_driver\\debug\\taos_odbc_driver\\taos_odbc_driver.dll");
    //if (m != NULL) {

    //}
    const char *ip = "127.0.0.1";
    TAOS *taos = taos_connect(ip, NULL, NULL, "testdb", 6030);
    if (!taos) {
        printf("error connect to %s\n", ip);
        return -1;
    }
    printf("success connect to %s, %lld\n", ip, taos);
    insertData(taos);
    const char *qry = "show databases;";
    TAOS_RES *res = taos_query(taos, qry);
    if (taos_errno(res) == 0) {
        printResult(res);
    } else {
        printf("execute error number: %d\n", taos_errno(res));
    }
    taos_close(taos);
    taos_cleanup();
    return 0;
}