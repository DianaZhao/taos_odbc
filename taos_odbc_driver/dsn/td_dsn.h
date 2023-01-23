
#ifndef _td_dsn_h_
#define _td_dsn_h_

#include <stdio.h>
#include <windows.h>
#include <odbcinst.h>
#include <shlwapi.h>

/* MySQL ODBC compatibility options */
#define TAOS_OPT_FLAG_FIELD_LENGTH                      1
#define TAOS_OPT_FLAG_FOUND_ROWS                        2
#define TAOS_OPT_FLAG_DEBUG                             4
#define TAOS_OPT_FLAG_BIG_PACKETS                       8
#define TAOS_OPT_FLAG_NO_PROMPT                        16
#define TAOS_OPT_FLAG_DYNAMIC_CURSOR                   32
#define TAOS_OPT_FLAG_NO_SCHEMA                        64
#define TAOS_OPT_FLAG_NO_DEFAULT_CURSOR               128
#define TAOS_OPT_FLAG_NO_LOCALE                       256
#define TAOS_OPT_FLAG_PAD_SPACE                       512
#define TAOS_OPT_FLAG_FULL_COLUMN_NAMES              1024 /*10*/
#define TAOS_OPT_FLAG_COMPRESSED_PROTO               2048
#define TAOS_OPT_FLAG_IGNORE_SPACE                   4096
#define TAOS_OPT_FLAG_NAMED_PIPE                     8192
#define TAOS_OPT_FLAG_NO_BIGINT                     16384
#define TAOS_OPT_FLAG_NO_CATALOG                    32768
#define TAOS_OPT_FLAG_USE_CNF                       65536
#define TAOS_OPT_FLAG_SAFE                         131072
#define TAOS_OPT_FLAG_NO_TRANSACTIONS              262144
#define TAOS_OPT_FLAG_LOG_QUERY                    524288
#define TAOS_OPT_FLAG_NO_CACHE                    1048576 /*20*/
#define TAOS_OPT_FLAG_FORWARD_CURSOR              2097152
#define TAOS_OPT_FLAG_AUTO_RECONNECT              4194304
#define TAOS_OPT_FLAG_AUTO_IS_NULL                8388608
#define TAOS_OPT_FLAG_ZERO_DATE_TO_MIN           16777216
#define TAOS_OPT_FLAG_MIN_DATE_TO_ZERO           33554432
#define TAOS_OPT_FLAG_MULTI_STATEMENTS           67108864
#define TAOS_OPT_FLAG_COLUMN_SIZE_S32           134217728
#define MADN_OPT_FLAG_NO_BINARY_RESULT          268435456
#define MADN_OPT_FLAG_BIGINT_BIND_STR           536870912
#define MADN_OPT_FLAG_NO_INFORMATION_SCHEMA    1073741824 /*30*/

enum enum_dsn_item_type {
    DSN_TYPE_STRING,
    DSN_TYPE_INT,
    DSN_TYPE_BOOL,
    DSN_TYPE_COMBO,    /* Mainly the same as string, but the field in the dialog is combobox */
    DSN_TYPE_OPTION,   /* Connection string option has correspondent OPTIONS bit */
    DSN_TYPE_CBOXGROUP /* Group of checkboxes each of them represent a bit in the field's value
                        Bitmap size is 1 byte */
};

typedef struct {
    unsigned int Page;
    unsigned long Item;
    unsigned long value;
} TAOS_OptionsMap;

typedef struct {
    char *DsnKey;
    unsigned int DsnOffset;
    enum enum_dsn_item_type Type;
    unsigned long FlagValue;
    BOOL IsAlias;
} TAOS_DsnKey;


typedef struct {
    TAOS_DsnKey *Key;
    int Page;
    int Item;
    int MaxLength;  /* For DSN_TYPE_CBOXGROUP that is the bit it represents */
    BOOL Mandatory;
} TAOS_DsnMap;

#define TD_WIN_SET_VALUE(Page, Field, Value) \
 Edit_SetText(GetDlgItem(hwndTab[(Page)],(Field)), (Value));

#define TD_WIN_SET_MAXLEN(Page, Field, Value) \
 Edit_LimitText(GetDlgItem(hwndTab[(Page)],(Field)), (Value));

/* Definitions to tell setup library via isPrompt field what should it do */
#define ODBC_CONFIG           0
#define ODBC_PROMPT           1
#define ODBC_PROMPT_REQUIRED  2

/* TLS version bits */
#define TAOS_TLSV11 1
#define TAOS_TLSV12 2
#define TAOS_TLSV13 4

extern const char TlsVersionName[3][8];
extern const char TlsVersionBits[3];

typedef struct st_taos_dsn {
    /* TODO: Does it really matter to keep this array in the DSN structure? */
    char ErrorMsg[SQL_MAX_MESSAGE_LENGTH];
    /*** General ***/
    char *DSNName;
    char *Driver;
    char *Description;
    /*** Connection parameters ***/
    char *ServerName;
    char *UserName;
    char *Password;
    char *Database;
    char *SaveFile;
    /* --- Internal --- */
    TAOS_DsnKey *Keys;

    /* Callbacke required for prompt to keep all memory de/allocation operations
       on same side of libraries */
    char *(*allocator)(size_t);

    void (*free)(void *);

    int isPrompt;
    /* Internal - end */
    unsigned int Port;
    /* Options */
    unsigned int Options;
    unsigned int ConnectionTimeout;
    unsigned int ReadTimeout;
    unsigned int WriteTimeout;
    BOOL Reconnect;
} TAOS_Dsn;

/* this structure is used to store and retrieve DSN Information */
extern TAOS_DsnKey DsnKeys[];

#define GET_FIELD_PTR(DSN, DSNKEY, TYPE) ((TYPE *)((char*)(DSN) + (DSNKEY)->DsnOffset))


/*** Function prototypes ***/
TAOS_Dsn *TAOS_DSN_Init(void);

void TAOS_DSN_SetDefaults(TAOS_Dsn *Dsn);

void TAOS_DSN_Free(TAOS_Dsn *Dsn);

BOOL TAOS_ReadDSN(TAOS_Dsn *Dsn, const char *KeyValue, BOOL OverWrite);

BOOL TAOS_SaveDSN(TAOS_Dsn *Dsn);

BOOL TAOS_DSN_Exists(const char *DsnName);

BOOL TAOS_ParseConnString(TAOS_Dsn *Dsn, const char *String, size_t Length, char Delimiter);

BOOL TAOS_ReadConnString(TAOS_Dsn *Dsn, const char *String, size_t Length, char Delimiter);

SQLULEN TAOS_DsnToString(TAOS_Dsn *Dsn, char *OutString, SQLULEN OutLength);

void TAOS_DsnUpdateOptionsFields(TAOS_Dsn *Dsn);

BOOL TAOS_DSN_PossibleConnect(TAOS_Dsn *Dsn);

/*** Helper macros ***/
#define DSN_OPTION(_a, _b)\
  ((_a)->Options & _b)

#define TD_ODBC_CURSOR_DYNAMIC(_a)\
  DSN_OPTION((_a), TAOS_OPT_FLAG_DYNAMIC_CURSOR)

#define TD_ODBC_CURSOR_FORWARD_ONLY(_a)\
  DSN_OPTION((_a), TAOS_OPT_FLAG_FORWARD_CURSOR)

#define TAOS_DSN_SET_STR(dsn, item, value, len)\
  if((value) && (len) != 0)\
  {\
    if ((len) == SQL_NTS)\
      (len)=(SQLSMALLINT)strlen((value));\
    TAOS_FREE((dsn)->item);\
    (dsn)->item= (char *)calloc(len + 1, sizeof(char));\
    memcpy((dsn)->item, (value),(len));\
  }
#define TAOS_FREE(a) do { \
  free((a));\
  (a)= NULL; \
} while(0)

#define TAOS_ALLOC(a) malloc((a))
#define TAOS_CALLOC(a) calloc((a) > 0 ? (a) : 1, sizeof(char))
#define TAOS_REALLOC(a, b) realloc((a),(b))

/* If required to free old memory pointed by current ptr, and set new value */
#define TAOS_RESET(ptr, newptr) do {\
  char *local_new_ptr= (newptr);\
  if (local_new_ptr != ptr) {\
    free((char*)(ptr));\
    if (local_new_ptr != NULL)\
      (ptr)= _strdup(local_new_ptr);\
    else\
      (ptr)= NULL;\
  }\
} while(0)

/* {{{ ltrim */
char *ltrim(char *Str);
/* }}} */

/* {{{ trim */
char *trim(char *Str);
/* }}} */


/* {{{ strcasestr() */
char *strcasestr(const char *HayStack, const char *Needle);
/* }}} */

#define TAOS_IS_EMPTY(STR) ((STR)==NULL || *(STR)=='\0')
#endif /* _td_dsn_h_ */
