//
// Created by diana on 2/10/2023.
//

#ifndef TAOS_ODBC_DRIVER_PLATFORM_WIN_H
#define TAOS_ODBC_DRIVER_PLATFORM_WIN_H

#endif //TAOS_ODBC_DRIVER_PLATFORM_WIN_H

typedef struct charset_info_st
{
    unsigned int	nr; /* so far only 1 byte for charset */
    unsigned int  state;
    const char	*csname;
    const char	*name;
    const char  *dir;
    unsigned int codepage;
    const char  *encoding;
    unsigned int	char_minlen;
    unsigned int	char_maxlen;
    unsigned int 	(*mb_charlen)(unsigned int c);
    unsigned int 	(*mb_valid)(const char *start, const char *end);
} CHARSET_INFO;

typedef struct st_client_charset
{
    unsigned int CodePage;
    CHARSET_INFO *cs_info;
} Client_Charset;

#include <Windows.h>
#include <sql.h>
#include <sqltypes.h>
//#include <WinSock2.h>
#include <Shlwapi.h>

char *ConvertFromWChar(const SQLWCHAR *Ptr, SQLINTEGER PtrLength, SQLULEN *Length, Client_Charset* cc, BOOL *DefaultCharUsed);
int ConvertAnsi2Unicode(Client_Charset* cc, const char *AnsiString, SQLLEN AnsiLength,
                             SQLWCHAR *UnicodeString, SQLLEN UnicodeLength,
                             SQLLEN *LengthIndicator, BOOL IsNull);
SQLWCHAR* ConvertToWchar(const char *Ptr, SQLLEN PtrLength, Client_Charset* cc);
SQLLEN    SetString(Client_Charset* cc, void *Dest, SQLULEN DestLength,
                         const char *Src, SQLLEN SrcLength);

Client_Charset utf8;