/************************************************************************************
   Copyright (C) 2013,2022 MariaDB Corporation AB

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with this library; if not see <http://www.gnu.org/licenses>
   or write to the Free Software Foundation, Inc.,
   51 Franklin St., Fifth Floor, Boston, MA 02110, USA
*************************************************************************************/
#ifndef _td_odbc_h_
#define _td_odbc_h_


#ifdef _WIN32
# include "td_platform_win32.h"
#else
# include "td_platform_posix.h"
#endif

#include <stdlib.h>


#include "td_legacy_helpers.h"

#include <sql.h>
#include <sqlext.h>
#include <odbcinst.h>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stddef.h>
#include <assert.h>
#include <time.h>

#define SQLSTATE_LENGTH 5

typedef struct st_td_odbc_connection TDDB_Dbc;
typedef struct st_td_odbc_stmt TDDB_Stmt;

typedef struct st_td_odbc_error
{
	char SqlState[SQL_SQLSTATE_SIZE + 1];
	char SqlStateV2[SQLSTATE_LENGTH + 1];
	char SqlErrorMsg[SQL_MAX_MESSAGE_LENGTH + 1];
	SQLRETURN ReturnValue;
} TDDB_ERROR;

typedef struct
{
	size_t PrefixLen;
	TDDB_ERROR* ErrRecord;
	SQLINTEGER NativeError;
	/* Order number of last requested error record */
	unsigned int ErrorNum;
	char SqlErrorMsg[SQL_MAX_MESSAGE_LENGTH + 1];
	char SqlState[SQLSTATE_LENGTH + 1];
	SQLRETURN ReturnValue;
} TDDB_Error;

typedef struct
{
	SQLPOINTER TargetValuePtr;
	SQLLEN BufferLength;
	SQLLEN Utf8BufferLength;
	SQLLEN* StrLen_or_Ind;
	void* InternalBuffer; /* used for conversion */
	SQLUINTEGER TargetType;
} TDDB_ColBind;

typedef struct
{
	SQLSMALLINT InputOutputType;
	SQLSMALLINT ValueType;
	SQLSMALLINT ParameterType;
	SQLULEN ColumnSize;
	SQLSMALLINT DecimalDigits;
	SQLPOINTER ParameterValuePtr;
	SQLLEN BufferLength;
	SQLLEN* StrLen_or_IndPtr;
	void* InternalBuffer; /* used for conversion */
} TDDB_ParmBind;

typedef struct
{
	/* Header */
	SQLUSMALLINT* ArrayStatusPtr;
	SQLULEN* BindOffsetPtr;
	/* TODO: In IPD this is SQLUINTEGER* field */
	SQLULEN* RowsProcessedPtr;
	SQLULEN       ArraySize;
	SQLULEN       BindType;
	SQLSMALLINT   AllocType;
	SQLSMALLINT   Count;
	/* Header end */
} TDDB_Header;

typedef struct
{
	SQLUSMALLINT* RowOperationPtr;
	SQLULEN* RowOffsetPtr;
	/*TDDB_ColBind *ColumnBind;*/
	  //MYSQL_BIND   *Bind;
	SQLLEN      dummy;
	SQLUINTEGER	BindSize;	/* size of each structure if using * Row-wise Binding */
	SQLSMALLINT	Allocated;
} TDDB_Ard;

typedef struct
{
	SQLUSMALLINT* ParamOperationPtr;
	SQLULEN* ParamOffsetPtr;
	/*TDDB_ParmBind *ParamBind;*/
	//MYSQL_BIND    *Bind;
	SQLLEN      ParamsetSize;
	SQLUINTEGER	ParamBindType;
	SQLSMALLINT	Allocated;
} TDDB_Apd;

typedef struct
{
	TDDB_Stmt* stmt;
	SQLULEN* RowsFetched;
	SQLUSMALLINT* RowStatusArray;
	//MYSQL_FIELD* Fields;
	SQLUINTEGER FieldCount;
	SQLSMALLINT	Allocated;
} TDDB_Ird;

typedef struct
{
	//TDDB_Header Header;
#if (ODBCVER >= 0x0300)
	SQLUINTEGER* ParamProcessedPtr;
#else
	SQLULEN* ParamProcessedPtr; /* SQLParamOptions */
#endif /* ODBCVER */
	SQLUSMALLINT* ParamStatusPtr;
	/*TDDB_ParmBind* Parameters;*/
	SQLSMALLINT Allocated;
} TDDB_Ipd;

typedef struct {
	char* BaseCatalogName;
	char* BaseColumnName;
	char* BaseTableName;
	char* CatalogName;
	char* ColumnName;
	SQLPOINTER  DataPtr;
	SQLLEN* OctetLengthPtr;
	SQLLEN* IndicatorPtr;
	char* Label;
	char* SchemaName;
	char* TableName;
	char* LiteralPrefix;
	char* LiteralSuffix;
	char* LocalTypeName;
	char* TypeName;
	char* InternalBuffer;  /* used for internal conversion */
	char* DefaultValue;
	char* DaeData;
	SQLLEN      DisplaySize;
	SQLULEN     Length;
	SQLLEN      OctetLength;
	SQLULEN     DaeDataLength;    /* Doesn't seem to be used anywhere */
	unsigned long InternalLength; /* This to be used in the MYSQL_BIND. Thus is the type */
	SQLINTEGER  AutoUniqueValue;
	SQLINTEGER  DateTimeIntervalPrecision;
	SQLINTEGER  DescLength;
	SQLINTEGER  CaseSensitive;
	SQLINTEGER  NumPrecRadix;
	SQLSMALLINT ConciseType;
	SQLSMALLINT DateTimeIntervalCode;
	SQLSMALLINT FixedPrecScale;
	SQLSMALLINT Nullable;
	SQLSMALLINT ParameterType;
	SQLSMALLINT Precision;
	SQLSMALLINT RowVer;
	SQLSMALLINT Scale;
	SQLSMALLINT Searchable;
	SQLSMALLINT Type;
	SQLSMALLINT Unnamed;
	SQLSMALLINT Unsigned;
	SQLSMALLINT Updateable;
	char     PutData;
	char     inUse;
	char     TruncError;
} TDDB_DescRecord;

typedef struct
{
	TDDB_Header Header;
	TDDB_DynArray Records;
	TDDB_DynArray Stmts;
	TDDB_Error Error;
	TDDB_List ListItem;        /* To store in the dbc */
	union {
		TDDB_Ard Ard;
		TDDB_Apd Apd;
		TDDB_Ipd Ipd;
		TDDB_Ird Ird;
	} Fields;

	TDDB_Dbc* Dbc;       /* Disconnect must automatically free allocated descriptors. Thus
						   descriptor has to know the connection it is allocated on */
	SQLINTEGER DescType;  /* SQL_ATTR_APP_ROW_DESC or SQL_ATTR_APP_PARAM_DESC */
	char AppType;      /* Allocated by Application ? */
} TDDB_Desc;

struct st_td_desc_fldid
{
	SQLSMALLINT FieldIdentifier;
	SQLSMALLINT Access[4];
};

struct st_td_stmt_methods;

typedef struct
{
	SQLLEN MaxRows;
	SQLLEN MaxLength;
	SQLLEN KeysetSize;
	SQLUINTEGER CursorType;
	SQLUINTEGER	ScrollConcurrency;
	SQLUINTEGER RetrieveData;
	SQLUINTEGER UseBookmarks;
	void* BookmarkPtr;
	SQLLEN BookmarkLength;
	SQLULEN	MetadataId;
	SQLULEN SimulateCursor;
	SQLSMALLINT BookmarkType;
} TDDB_StmtOptions;

/* TODO: To check is it 0 or 1 based? not quite clear from its usage */
typedef struct
{
	char* Name;
	SQLLEN           Position;
	SQLLEN           RowsetSize;
	//MYSQL_ROW_OFFSET Next;
} TDDB_Cursor;

enum TDDB_DaeType { TDDB_DAE_NORMAL = 0, TDDB_DAE_ADD = 1, TDDB_DAE_UPDATE = 2, TDDB_DAE_DELETE = 3 };

#define RESET_DAE_STATUS(Stmt_Hndl) (Stmt_Hndl)->Status=0; (Stmt_Hndl)->PutParam= -1
#define MARK_DAE_DONE(Stmt_Hndl)    (Stmt_Hndl)->Status=0; (Stmt_Hndl)->PutParam= (Stmt_Hndl)->ParamCount

#define PARAM_IS_DAE(Len_Ptr) ((Len_Ptr) && (*(Len_Ptr) == SQL_DATA_AT_EXEC || *(Len_Ptr) <= SQL_LEN_DATA_AT_EXEC_OFFSET))
#define DAE_DONE(Stmt_Hndl) ((Stmt_Hndl)->PutParam >= (Stmt_Hndl)->ParamCount)

enum TDDB_StmtState { TDDB_SS_INITED = 0, TDDB_SS_EMULATED = 1, TDDB_SS_PREPARED = 2, TDDB_SS_EXECUTED = 3, TDDB_SS_OUTPARAMSFETCHED = 4 };

#define STMT_WAS_PREPARED(Stmt_Hndl) ((Stmt_Hndl)->State >= TDDB_SS_EMULATED)
#define STMT_REALLY_PREPARED(Stmt_Hndl) ((Stmt_Hndl)->State > TDDB_SS_EMULATED)
#define STMT_EXECUTED(Stmt_Hndl) ((Stmt_Hndl)->State == TDDB_SS_EXECUTED)
#define RESET_STMT_STATE(Stmt_Hndl) if ((Stmt_Hndl)->State > TDDB_SS_PREPARED) (Stmt_Hndl)->State= TDDB_SS_PREPARED

/* Struct used to define column type when driver has to fix it (in catalog functions + SQLGetTypeInfo) */
typedef struct
{
	SQLSMALLINT SqlType;
	char     Unsigned;
	SQLSMALLINT Nullable;
	SQLLEN      OctetLength;

} TDDB_ShortTypeInfo;

typedef struct
{
	unsigned int  ArraySize;
	char       HasRowsToSkip;

} TDDB_BulkOperationInfo;

/* Stmt struct needs definitions from my_parse.h */
#include "td_parse.h"

#define STMT_STRING(STMT) (STMT)->Query.Original

struct st_td_odbc_stmt
{
	TDDB_StmtOptions          Options;
	TDDB_Error                Error;
	TDDB_Cursor               Cursor;
	TDDB_QUERY                Query;
	TDDB_List                 ListItem;
	long long                 AffectedRows;
	TDDB_Dbc* Connection;
	struct st_td_stmt_methods* Methods;
	struct st_td_stmt_rsstore* RsOps;
	MYSQL_STMT* stmt;
	MYSQL_RES* metadata;
	MYSQL_RES* DefaultsResult;
	TDDB_DescRecord* PutDataRec;
	TDDB_Stmt* DaeStmt;
	TDDB_Stmt* PositionedCursor;
	MYSQL_STMT** MultiStmts;
	SQLLEN                    LastRowFetched;
	MYSQL_BIND* result;
	MYSQL_BIND* params;
	unsigned long* CharOffset;
	unsigned long* Lengths;
	char* TableName;
	char* CatalogName;
	TDDB_ShortTypeInfo* ColsTypeFixArr;
	/* Application Descriptors */
	TDDB_Desc* Apd;
	TDDB_Desc* Ard;
	TDDB_Desc* Ird;
	TDDB_Desc* Ipd;
	/* Internal Descriptors */
	TDDB_Desc* IApd;
	TDDB_Desc* IArd;
	TDDB_Desc* IIrd;
	TDDB_Desc* IIpd;
	unsigned short* UniqueIndex; /* Insdexes of columns that make best available unique identifier */
	SQLSETPOSIROW             DaeRowNumber;
	int                       ArrayOffset;
	int                       Status;
	unsigned int              MultiStmtNr;
	unsigned int              MultiStmtMaxParam;
	int                       PutParam;
	enum TDDB_StmtState       State;
	enum TDDB_DaeType         DataExecutionType;
	SQLSMALLINT               ParamCount;
	TDDB_BulkOperationInfo    Bulk;
	char                   PositionedCommand;
	char                   RebindParams;
	char                   bind_done;
};

enum TDDB_AppType {
	ATypeGeneral = 0,
	ATypeMSAccess = 1
};

/*const size_t stmtsime = sizeof(struct st_td_odbc_stmt);*/
typedef struct st_td_odbc_environment {
	TDDB_Error Error;
	CRITICAL_SECTION cs;
	TDDB_List* Dbcs;
	SQLWCHAR* TraceFile;
	SQLUINTEGER Trace;
	SQLINTEGER OdbcVersion;
	SQLINTEGER OutputNTS;
	enum TDDB_AppType AppType;
} TDDB_Env;

//const size_t sizeOfT = sizeof(TDDB_Env);

#include "td_dsn.h"

typedef struct ma_charset_info_st
{
	unsigned int	nr; /* so far only 1 byte for charset */
	unsigned int  state;
	const char* csname;
	const char* name;
	const char* dir;
	unsigned int codepage;
	const char* encoding;
	unsigned int	char_minlen;
	unsigned int	char_maxlen;
	unsigned int 	(*mb_charlen)(unsigned int c);
	unsigned int 	(*mb_valid)(const char* start, const char* end);
} TDDB_CHARSET_INFO;

typedef struct st_client_charset
{
	unsigned int CodePage;
	TDDB_CHARSET_INFO* cs_info;
} Client_Charset;


struct st_td_odbc_connection
{
	TDDB_Error Error;
	CRITICAL_SECTION cs;           /* mutex for mariadb handle, i.e. for server communications */
	CRITICAL_SECTION ListsCs;      /*       for operations with lists */
	TDDB_List ListItem;
	Client_Charset Charset;
	//MYSQL* mariadb;                /* handle to a mariadb connection */
	TDDB_Env* Environment;         /* global environment */
	TDDB_Dsn* Dsn;
	struct st_td_connection_methods* Methods;

	Client_Charset* ConnOrSrcCharset; /* "Source" here stands for which charset Windows DM was using as source, when converted to unicode.
									We have to use same charset to recode from unicode to get same string as application sent it.
									For Unicode application that is the same as "Charset", or in case of ANSI on Windows - defaulst system
									codepage */
	char* CurrentSchema; /* Used to store current schema if the seesion tracking is possible */
	TDDB_List* Stmts;
	TDDB_List* Descrs;
	/* Attributes */
	char* CatalogName;
	HWND       QuietMode;
	char* TraceFile;
	TDDB_Stmt* Streamer;

	SQLULEN    AsyncEnable;
	SQLPOINTER EnlistInDtc;
	SQLULEN    OdbcCursors;
	unsigned long Options;
	SQLUINTEGER AutoIpd;
	SQLUINTEGER AutoCommit;
	SQLUINTEGER ConnectionDead;
	SQLUINTEGER ConnectionTimeout;
	SQLUINTEGER ReadTimeout;
	SQLUINTEGER WriteTimeout;
	SQLUINTEGER PacketSize;
	SQLINTEGER  AccessMode;
	SQLINTEGER  IsolationLevel;     /* tx_isolation */
	SQLUINTEGER Trace;
	SQLUINTEGER LoginTimeout;
	SQLUINTEGER MetadataId;
	SQLINTEGER  TxnIsolation;
	SQLINTEGER  CursorCount;
	char    ServerCapabilities;
	char IsAnsi;
	char IsMySQL;
};

typedef BOOL(__stdcall* PromptDSN)(HWND hwnd, TDDB_Dsn* Dsn);

typedef struct
{
	void* LibraryHandle;
	PromptDSN Call;
} TDDB_Prompt;

#define TDDB_PROMPT_NOT_SUPPORTED 1
#define TDDB_PROMPT_COULDNT_LOAD  2
int DSNPrompt_Lookup(TDDB_Prompt* prompt, const char* SetupLibName);

int DSNPrompt_Free(TDDB_Prompt* prompt);

int   InitClientCharset(Client_Charset* cc, const char* name);
void  CopyClientCharset(Client_Charset* Src, Client_Charset* Dst);
void  CloseClientCharset(Client_Charset* cc);

/* Default precision of SQL_NUMERIC */
#define TDDB_DEFAULT_PRECISION 38
#define TDDB_MAX_SCALE         TDDB_DEFAULT_PRECISION
#define BINARY_CHARSETNR       63
/* Inexistent param id */
#define TDDB_NOPARAM           -1
/* Macros to guard communications with the server.
   TODO: make it(locking) optional depending on designated connection string option */
#define LOCK_MARIADB(Dbc)   EnterCriticalSection(&(Dbc)->cs)
#define UNLOCK_MARIADB(Dbc) LeaveCriticalSection(&(Dbc)->cs)

   /* Enabling tracing */
#define MAODBC_DEBUG 1
/* Macro checks return of the suplied SQLRETURN function call, checks if it is succeeded, and in case of error pushes error up */
#define RETURN_ERROR_OR_CONTINUE(sqlreturn_func_call) {\
  SQLRETURN rc= (sqlreturn_func_call);\
  if (!SQL_SUCCEEDED(rc)) return rc;\
} while(0)

#define iOdbc() (sizeof(SQLWCHAR)==4)

//#include <td_error.h>
//#include <td_info.h>
//#include <td_environment.h>
//#include <td_connection.h>
//#include <td_debug.h>
//#include <td_desc.h>
//#include <td_statement.h>
//#include <td_string.h>
//#include <td_result.h>
//#include <td_driver.h>
//#include <td_helper.h>
//#include <td_server.h>
//#include <td_typeconv.h>
//#include <td_bulk.h>
//
//#include "td_api_internal.h"

SQLRETURN TDDB_GetBookmark(TDDB_Stmt* StatementHandle,
	SQLSMALLINT TargetType,
	SQLPOINTER  TargetValuePtr,
	SQLLEN      BufferLength,
	SQLLEN* StrLen_or_IndPtr);

SQLRETURN TDDB_StmtColAttr(TDDB_Stmt* Stmt, SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttributePtr,
	SQLSMALLINT BufferLength, SQLSMALLINT* StringLengthPtr, SQLLEN* NumericAttributePtr, char IsWchar);

SQLRETURN TDDB_StmtColAttr(TDDB_Stmt* Stmt, SQLUSMALLINT ColumnNumber, SQLUSMALLINT FieldIdentifier, SQLPOINTER CharacterAttributePtr,
	SQLSMALLINT BufferLength, SQLSMALLINT* StringLengthPtr, SQLLEN* NumericAttributePtr, char IsWchar);
#endif /* _td_odbc_h_ */
