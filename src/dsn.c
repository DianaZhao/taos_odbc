/*
 * MIT License
 *
 * Copyright (c) 2022 Diana Zhao <dianaz@outlook.com>
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

#include <windows.h>
#include <odbcinst.h>

#define POST_INSTALLER_ERROR(hwndParent, code, fmt, ...)             \
do {                                                                 \
  char buf[4096];                                                    \
  snprintf(buf, sizeof(buf), "%s[%d]%s():" fmt "",                   \
           basename((char*)__FILE__), __LINE__, __func__,            \
           ##__VA_ARGS__);                                           \
  SQLPostInstallerError(code, buf);                                  \
  if (hwndParent) {                                                  \
    MessageBox(hwndParent, buf, "Error", MB_OK|MB_ICONEXCLAMATION);  \
  }                                                                  \
} while (0)

typedef struct kv_s kv_t;
struct kv_s {
    char *line;
    size_t val;
};

static BOOL get_driver_dll_path(HWND hwndParent, char *buf, size_t len) {
    HMODULE hm = NULL;

    if (GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                          (LPCSTR) &ConfigDSN, &hm) == 0) {
        int ret = GetLastError();
        POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_REQUEST_FAILED, "GetModuleHandle failed, error = %d\n", ret);
        return FALSE;
    }
    if (GetModuleFileName(hm, buf, (DWORD) len) == 0) {
        int ret = GetLastError();
        POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_REQUEST_FAILED, "GetModuleFileName failed, error = %d\n", ret);
        return FALSE;
    }
    return TRUE;
}

static BOOL doDSNAdd(HWND hwndParent, LPCSTR lpszDriver, LPCSTR lpszAttributes) {
    BOOL r = TRUE;

    kv_t *kvs = NULL;

    kv_t dsn = {0};
    char *line = NULL;

    do {
        char driver_dll[MAX_PATH + 1];
        r = get_driver_dll_path(hwndParent, driver_dll, sizeof(driver_dll));
        if (!r) break;

        dsn.line = strdup("DSN=TAOS_DEMO");
        if (!dsn.line) {
            r = FALSE;
            break;
        }

        const char *p = lpszAttributes;
        int ikvs = 0;
        while (p && *p) {
            line = strdup(p);
            if (!line) {
                r = FALSE;
                break;
            }
            char *v = strchr(line, '=');
            if (!v) {
                r = FALSE;
                break;
            }

            if (strstr(line, "DSN") == line) {
                if (dsn.line) {
                    free(dsn.line);
                    dsn.line = NULL;
                    dsn.val = 0;
                }
                dsn.line = line;
                line = NULL;
            } else {
                kv_t *t = (kv_t *) realloc(kvs, (ikvs + 1) * sizeof(*t));
                if (!t) {
                    r = FALSE;
                    free(line);
                    break;
                }
                t[ikvs].line = line;
                *v = '\0';
                if (v) t[ikvs].val = v - line + 1;
                line = NULL;

                kvs = t;
                ++ikvs;
            }

            p += strlen(p) + 1;
        }

        if (hwndParent) {
            MessageBox(hwndParent, "Please use odbcconf to add DSN for TAOS ODBC Driver", "Warning!",
                       MB_OK | MB_ICONEXCLAMATION);
        }
        if (!r) break;

        char *v = NULL;
        v = strchr(dsn.line, '=');
        if (!v) {
            r = FALSE;
            break;
        }
        *v = '\0';
        dsn.val = v - dsn.line + 1;

        if ((!dsn.line)) {
            if (!r) POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_REQUEST_FAILED, "lack of either DSN or Driver");
        } else {
            if (r) r = SQLWritePrivateProfileString("ODBC Data Sources", dsn.line + dsn.val, lpszDriver, "Odbc.ini");
            if (r) r = SQLWritePrivateProfileString(dsn.line + dsn.val, "Driver", driver_dll, "Odbc.ini");
        }

        for (int i = 0; r && i < ikvs; ++i) {
            const char *k = kvs[i].line;
            const char *v = NULL;
            if (kvs[i].val) v = kvs[i].line + kvs[i].val;
            r = SQLWritePrivateProfileString(dsn.line + dsn.val, k, v, "Odbc.ini");
        }
    } while (0);

    if (dsn.line) free(dsn.line);
    if (line) free(line);

    return r;
}

static BOOL doDSNConfig(HWND hwndParent, LPCSTR lpszDriver, LPCSTR lpszAttributes) {
    const char *p = lpszAttributes;
    while (p && *p) {
        p += strlen(p) + 1;
    }
    return FALSE;
}

static BOOL doDSNRemove(HWND hwndParent, LPCSTR lpszDriver, LPCSTR lpszAttributes) {
    BOOL r = TRUE;

    kv_t dsn = {0};
    char *line = NULL;

    do {
        const char *p = lpszAttributes;
        int ikvs = 0;
        while (p && *p) {
            line = strdup(p);
            if (!line) {
                r = FALSE;
                break;
            }
            char *v = strchr(line, '=');
            if (!v) {
                r = FALSE;
                break;
            }
            *v = '\0';

            if (strstr(line, "DSN") == line) {
                if (dsn.line) {
                    free(dsn.line);
                    dsn.line = NULL;
                    dsn.val = 0;
                }
                dsn.line = line;
                dsn.val = v - line + 1;
                line = NULL;
                break;
            } else {
                free(line);
                line = NULL;
            }

            p += strlen(p) + 1;
        }

        if (!r) break;

        if (!dsn.line) {
            POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_REQUEST_FAILED, "lack of DSN");
            r = FALSE;
            break;
        }

        r = SQLWritePrivateProfileString("ODBC Data Sources", dsn.line + dsn.val, NULL, "Odbc.ini");
        if (!r) break;

        char buf[8192];
        r = SQLGetPrivateProfileString(dsn.line + dsn.val, NULL, "null", buf, sizeof(buf), "Odbc.ini");
        if (!r) break;

        int n = 0;
        char *s = buf;
        while (s && *s && n++ < 10) {
            SQLWritePrivateProfileString(dsn.line + dsn.val, s, NULL, "Odbc.ini");
            s += strlen(s) + 1;
        }
    } while (0);

    if (dsn.line) free(dsn.line);
    if (line) free(line);
    return r;
}

static BOOL doConfigDSN(HWND hwndParent, WORD fRequest, LPCSTR lpszDriver, LPCSTR lpszAttributes) {
    BOOL r = FALSE;
    const char *sReq = NULL;
    switch (fRequest) {
        case ODBC_ADD_DSN:
            sReq = "ODBC_ADD_DSN";
            break;
        case ODBC_CONFIG_DSN:
            sReq = "ODBC_CONFIG_DSN";
            break;
        case ODBC_REMOVE_DSN:
            sReq = "ODBC_REMOVE_DSN";
            break;
        default:
            sReq = "UNKNOWN";
            break;
    }
    switch (fRequest) {
        case ODBC_ADD_DSN: {
            r = doDSNAdd(hwndParent, lpszDriver, lpszAttributes);
        }
            break;
        case ODBC_CONFIG_DSN: {
            r = doDSNConfig(hwndParent, lpszDriver, lpszAttributes);
        }
            break;
        case ODBC_REMOVE_DSN: {
            r = doDSNRemove(hwndParent, lpszDriver, lpszAttributes);
        }
            break;
        default: {
            POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_GENERAL_ERR, "not implemented yet");
            r = FALSE;
        }
            break;
    }
    return r;
}

BOOL INSTAPI
ConfigDSN(HWND
          hwndParent,
          WORD fRequest, LPCSTR
          lpszDriver,
          LPCSTR lpszAttributes
) {
    BOOL r;
    r = doConfigDSN(hwndParent, fRequest, lpszDriver, lpszAttributes);
    return
            r;
}

BOOL INSTAPI
ConfigTranslator(HWND
                 hwndParent,
                 DWORD *pvOption
) {
    POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_GENERAL_ERR, "not implemented yet");
    return
            FALSE;
}

BOOL INSTAPI
ConfigDriver(HWND
             hwndParent,
             WORD fRequest, LPCSTR
             lpszDriver,
             LPCSTR lpszArgs,
             LPSTR
             lpszMsg,
             WORD cbMsgMax, WORD
             *pcbMsgOut) {
    POST_INSTALLER_ERROR(hwndParent, ODBC_ERROR_GENERAL_ERR, "not implemented yet");
    return
            FALSE;
}
