//
// Created by diana on 2/10/2023.
//

#include "platform_win.h"

Client_Charset utf8 = {CP_UTF8, NULL};

char *strndup(const char *s, size_t n) {
    char *res = NULL;

    if (s != NULL) {
        res = (char *) malloc(n + 1);
        if (res != NULL) {
            memcpy(res, s, n);
            res[n] = '\0';
        }
    }

    return res;
}

SQLWCHAR *ConvertToWchar(const char *Ptr, SQLLEN PtrLength, Client_Charset *cc) {
    SQLWCHAR *WStr = NULL;
    int Length;

    if (!Ptr)
        return WStr;

    if (PtrLength == SQL_NTS)
        PtrLength = -1;

    if (!cc || !cc->CodePage)
        cc = &utf8;

    /* TODO: Check this
       In fact MultiByteToWideChar does not copy terminating character by default
       Thus +1 does not make sense
       "If the function succeeds and cchWideChar is 0, the return value is the required size,
        in characters, for the buffer indicated by lpWideCharStr...
        MultiByteToWideChar does not null-terminate an output string if the input string length
        is explicitly specified without a terminating null character. To null-terminate an output
        string for this function, the application should pass in -1 or explicitly count the
        terminating null character for the input string." */
    if ((Length = MultiByteToWideChar(cc->CodePage, 0, Ptr, (int) PtrLength, NULL, 0)))
        if ((WStr = (SQLWCHAR *) calloc(sizeof(SQLWCHAR) * Length + 1, sizeof(char))))
            MultiByteToWideChar(cc->CodePage, 0, Ptr, (int) PtrLength, WStr, Length);
    return WStr;
}

/* {{{ MADB_ConvertFromWChar
Length gets number of written bytes including TN (if WstrCharLen == -1 or SQL_NTS or if WstrCharLen includes
TN in the Wstr) */
char *ConvertFromWChar(const SQLWCHAR *Wstr, SQLINTEGER WstrCharLen, SQLULEN *Length/*Bytes*/, Client_Charset *cc,
                       BOOL *Error) {
    char *AscStr;
    int AscLen, AllocLen;

    if (Error)
        *Error = 0;

    if (cc == NULL || cc->CodePage < 1) {
        cc = &utf8;
    }

    if (WstrCharLen == SQL_NTS)
        WstrCharLen = -1;

    AllocLen = AscLen = WideCharToMultiByte(cc->CodePage, 0, Wstr, WstrCharLen, NULL, 0, NULL, NULL);
    if (WstrCharLen != -1)
        ++AllocLen;

    if (!(AscStr = (char *) calloc(AllocLen, sizeof(char))))
        return NULL;

    AscLen = WideCharToMultiByte(cc->CodePage, 0, Wstr, WstrCharLen, AscStr, AscLen, NULL,
                                 (cc->CodePage != CP_UTF8) ? Error : NULL);
    if (AscLen && WstrCharLen == -1)
        --AscLen;

    if (Length)
        *Length = (SQLINTEGER) AscLen;
    return AscStr;
}
/* }}} */

/* {{{ MADB_ConvertAnsi2Unicode
       @AnsiLength[in]    - number of bytes to copy, negative if AnsiString is Null terminated and the terminating blank has to be copied
       @UnicodeLength[in] - size of output buffer in chars, that effectively mean in SQLWCHAR units
       @LengthIndicator[out] - number of available characters not counting terminating null(unless it was included in AnsiLength, and IsNull
                            is FALSE)
       @IsNull[in]        - whether to copy terminating blank. The value has to be 1 or 0(TRUE/FALSE)
                            If AnsiString is negative, its value is neglected(null is copied)
       @returns 1 in case of error, 0 otherwise */
/* Required Length without or with TN(if IsNull is TRUE, or AnsiLength == -1 or SQL_NTS) is put to LenghtIndicator */
int ConvertAnsi2Unicode(Client_Charset *cc, const char *AnsiString, SQLLEN AnsiLength,
                        SQLWCHAR *UnicodeString, SQLLEN UnicodeLength,
                        SQLLEN *LengthIndicator, BOOL IsNull) {
    SQLLEN RequiredLength;
    SQLWCHAR *Tmp = UnicodeString;
    int rc = 0;

    if (LengthIndicator)
        *LengthIndicator = 0;


    if (!AnsiLength || UnicodeLength < 0) {
        return 1;
    }

    if (AnsiLength == SQL_NTS || AnsiLength == -1)
        IsNull = 1;

    /* calculate required length */
    RequiredLength = MultiByteToWideChar(cc->CodePage, 0, AnsiString, IsNull ? -1 : (int) AnsiLength, NULL, 0);

    /* Set LengthIndicator */
    if (LengthIndicator)
        *LengthIndicator = RequiredLength - IsNull;
    if (!UnicodeLength)
        return 0;

    if (RequiredLength > UnicodeLength) {
        Tmp = (SQLWCHAR *) malloc(RequiredLength * sizeof(SQLWCHAR));
        if (Tmp == NULL) {
            return 1;
        }
    } else {
        /* Otherwise the size of buffer is, as passed to the this function, UnicodeLength */
        RequiredLength = UnicodeLength;
    }

    RequiredLength = MultiByteToWideChar(cc->CodePage, 0, AnsiString, IsNull ? -1 : (int) AnsiLength, Tmp,
                                         (int) RequiredLength);
    if (RequiredLength < 1) {
        rc = 1;
        if (Tmp != UnicodeString)
            free(Tmp);
        return rc;
    }

    /* Truncation */
    if (Tmp != UnicodeString) {
        wcsncpy(UnicodeString, L"", 1);
        wcsncat(UnicodeString, Tmp, UnicodeLength - 1);
    }

    if (Tmp != UnicodeString)
        free(Tmp);
    return rc;
}

/* Returns required length for result string with(if dest and dest length are provided)
   or without terminating NULL(otherwise). If cc is NULL, or not initialized(CodePage is 0),
   then simply SrcLength is returned.
   If Dest is not NULL, and DestLenth is 0, then error */
SQLLEN SetString(Client_Charset *cc, void *Dest, SQLULEN DestLength,
                      const char *Src, SQLLEN SrcLength/*bytes*/) {
    char *p = (char *) Dest;
    SQLLEN Length = 0;

    if (SrcLength == SQL_NTS) {
        if (Src != NULL) {
            SrcLength = strlen(Src);
        } else {
            SrcLength = 0;
        }
    }

    /* Not enough space */
    if (!DestLength || !Dest) {
        if (!cc || !cc->CodePage)
            return SrcLength;
        else {
            Length = MultiByteToWideChar(cc->CodePage, 0, Src, (int) SrcLength, NULL, 0);
            return Length;
        }
    }

    if (!Src || !strlen(Src) || !SrcLength) {
        memset(p, 0, cc && cc->CodePage ? sizeof(SQLWCHAR) : sizeof(SQLCHAR));
        return 0;
    }

    if (!cc || !cc->CodePage) {
        strncpy_s((char *) Dest, DestLength, Src ? Src : "", _TRUNCATE);
        return SrcLength;
    } else {
        return Length;
    }
}


int GetSourceAnsiCs(Client_Charset *cc) {
    cc->CodePage = GetACP();

    /* We don't need cs_info for this */
    return cc->CodePage;
}
