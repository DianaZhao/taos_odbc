
#include "td_dsn.h"

#define DSNKEY_OPTIONS_INDEX   3
#define DSNKEY_OPTION_INDEX    4
#define DSNKEY_NAMEDPIPE_INDEX 5
#define DSNKEY_TCPIP_INDEX     6
#define DSNKEY_SERVER_INDEX    7
#define DSNKEY_UID_INDEX       8
#define DSNKEY_PWD_INDEX       9
#define DSNKEY_DATABASE_INDEX 10
#define DSNKEY_FP_INDEX       25
#define DSNKEY_FPLIST_INDEX   26
#define DSNKEY_STREAMRS_INDEX 43


/* If adding new connstring option, one should add them to the end of the array section before aliases, because some arrays defining program logic
   use indexes of an element in this array. In particular, in the setup lib there is array mapping dsn field(i.e. this array element) to parent window
   and resource id of the contorl in the dialog. Thus if you still think it should go somewhere in the middle, mind also change that array and at least
   DsnKeysSwitch below in this file, and index defines in this file, accordingly */
TAOS_DsnKey DsnKeys[] =
        {
                {"DRIVER",      offsetof(TAOS_Dsn, Driver),      DSN_TYPE_STRING, 0, 0},
                {"DSN",         offsetof(TAOS_Dsn, DSNName),     DSN_TYPE_STRING, 0, 0}, /* 0 */
                {"DESCRIPTION", offsetof(TAOS_Dsn, Description), DSN_TYPE_STRING, 0, 0},
//                /* OPTIONS should go above all DSN_TYPE_OPTION. They are not saved in DSN separately, and then DSN is read, corresponding
//                   properties are filled from OPTIONS. Also putting its alias here - it should not appear on Windows(unless somebody edits
//                   registry manually), but on *nix we can expect everything. Array index used in some places to decide if the key is OPTIONS */
//                {"OPTIONS",        offsetof(TAOS_Dsn,
//                                            Options),               DSN_TYPE_INT,    0,                            0}, /* DSNKEY_OPTIONS_INDEX */
//                {"OPTION",         DSNKEY_OPTIONS_INDEX,            DSN_TYPE_INT,    0,                            1}, /* DSNKEY_OPTION_INDEX  */
//
                {"SERVER",      offsetof(TAOS_Dsn, ServerName),  DSN_TYPE_STRING, 0, 0}, /* DSNKEY_SERVER_INDEX     */
                {"UID",         offsetof(TAOS_Dsn, UserName),    DSN_TYPE_STRING, 0, 0}, /* DSNKEY_UID_INDEX        */
                {"PWD",         offsetof(TAOS_Dsn, Password),    DSN_TYPE_STRING, 0, 0}, /* DSNKEY_PWD_INDEX        */
                {"DATABASE",    offsetof(TAOS_Dsn, Database),    DSN_TYPE_COMBO,  0, 0}, /* 10 DSNKEY_DATABASE_INDEX */
                {"PORT",        offsetof(TAOS_Dsn, Port),        DSN_TYPE_INT,    0, 0},
//                {"CONN_TIMEOUT",   offsetof(TAOS_Dsn,
//                                            ConnectionTimeout),     DSN_TYPE_INT,    0,                            0},
//                {"AUTO_RECONNECT", offsetof(TAOS_Dsn,
//                                            Reconnect),             DSN_TYPE_OPTION, TAOS_OPT_FLAG_AUTO_RECONNECT, 0},
//                /* SSL */
//                {"SAVEFILE",       offsetof(TAOS_Dsn,
//                                            SaveFile),              DSN_TYPE_STRING, 0,                            0}, /* 30 */
//                {"READ_TIMEOUT",   offsetof(TAOS_Dsn,
//                                            ReadTimeout),           DSN_TYPE_INT,    0,                            0},
//                {"WRITE_TIMEOUT",  offsetof(TAOS_Dsn,
//                                            WriteTimeout),          DSN_TYPE_INT,    0,                            0}, /* 40 */
//                /* Aliases. Here offset is index of aliased key */
//                {"SERVERNAME",     DSNKEY_SERVER_INDEX,             DSN_TYPE_STRING, 0,                            1},
//                {"USER",           DSNKEY_UID_INDEX,                DSN_TYPE_STRING, 0,                            1},
//                {"PASSWORD",       DSNKEY_PWD_INDEX,                DSN_TYPE_STRING, 0,                            1},
//                {"DB",             DSNKEY_DATABASE_INDEX,           DSN_TYPE_COMBO,  0,                            1},
//                {"SSLFP",          DSNKEY_FP_INDEX,                 DSN_TYPE_STRING, 0,                            1},
//                {"SSLFPLIST",      DSNKEY_FPLIST_INDEX,             DSN_TYPE_STRING, 0,                            1},
//                {"NO_CACHE",       DSNKEY_STREAMRS_INDEX,           DSN_TYPE_BOOL,   0,                            1},

                /* Terminating Null */
                {NULL, 0,                                        DSN_TYPE_BOOL,   0, 0}
        };

#define IS_OPTIONS_BITMAP(key_index) (key_index == DSNKEY_OPTIONS_INDEX)

typedef struct {
    unsigned int Key;
    unsigned int Dependent;
    BOOL Same;      /* Should dependent be switched same way, or in reverse */
} TAOS_DsnKeyDep;

/* Define pairs of keys that are switches, i.e. setting one should reset the other.
   Transitive dependencies have to be defined as direct dependencies here as well */
const TAOS_DsnKeyDep DsnKeysSwitch[] =
        {
                {DSNKEY_NAMEDPIPE_INDEX, DSNKEY_TCPIP_INDEX,     0},
                {DSNKEY_TCPIP_INDEX,     DSNKEY_NAMEDPIPE_INDEX, 0}
        };

const SQLCHAR TlsVersionName[][8] = {"TLSv1.1", "TLSv1.2", "TLSv1.3"};
const SQLCHAR TlsVersionBits[] = {TAOS_TLSV11, TAOS_TLSV12, TAOS_TLSV13};

/* {{{ ltrim */
SQLCHAR *ltrim(SQLCHAR *Str) {
    if (Str) {
        while (*Str && isspace(Str[0]))
            ++Str;
    }
    return Str;
}
/* }}} */

/* {{{ trim */
SQLCHAR *trim(SQLCHAR *Str) {
    SQLCHAR *end;

    Str = ltrim(Str);

    end = Str + strlen(Str) - 1;
    while (isspace(*end))
        *end-- = 0;
    return Str;
}
/* }}} */


/* {{{ strcasestr() */
SQLCHAR *strcasestr(const SQLCHAR *HayStack, const SQLCHAR *Needle) {
    return StrStrIA(HayStack, Needle);
}
/* }}} */


/* {{{ TAOS_Dsn_Init() */
TAOS_Dsn *TAOS_DSN_Init() {
    TAOS_Dsn *Dsn;

    if ((Dsn = (TAOS_Dsn *) malloc(sizeof(TAOS_Dsn)))) {
        Dsn->Keys = (TAOS_DsnKey *) &DsnKeys;
        /*Dsn->NullSchemaMeansCurrent = '\1';*/
    }
    return Dsn;
}
/* }}} */

/* {{{ TAOS_Dsn_Free */
void TAOS_DSN_Free(TAOS_Dsn *Dsn) {
    if (!Dsn)
        return;

    TAOS_FREE(Dsn->DSNName);
    TAOS_FREE(Dsn->Driver);
    TAOS_FREE(Dsn->Description);
    TAOS_FREE(Dsn->ServerName);
    TAOS_FREE(Dsn->UserName);
    TAOS_FREE(Dsn->Password);
    TAOS_FREE(Dsn->Database);
    TAOS_FREE(Dsn->SaveFile);
    TAOS_FREE(Dsn);
}

/* }}} */


void TAOS_SetOptionValue(TAOS_Dsn *Dsn, TAOS_DsnKey *DsnKey, BOOL value) {
    *GET_FIELD_PTR(Dsn, DsnKey, BOOL) = value;
    if (value) {
        Dsn->Options |= DsnKey->FlagValue;
    } else {
        Dsn->Options &= ~DsnKey->FlagValue;
    }
}


/* {{{ TAOS_DsnSwitchDependents */
/* If TCPIP selected, we have to reset NAMEDPIPE */
BOOL TAOS_DsnSwitchDependents(TAOS_Dsn *Dsn, unsigned int Changed) {
    int i;

    for (i = 0; i < sizeof(DsnKeysSwitch) / sizeof(TAOS_DsnKeyDep); ++i) {
        if (DsnKeysSwitch[i].Key == Changed) {
            BOOL KeySet;

            switch (DsnKeys[Changed].Type) {
                case DSN_TYPE_STRING:
                case DSN_TYPE_COMBO: {
                    SQLCHAR *str = *GET_FIELD_PTR(Dsn, &DsnKeys[Changed], char*);
                    KeySet = str && *str;
                }
                    break;
                case DSN_TYPE_OPTION:
                case DSN_TYPE_BOOL:
                case DSN_TYPE_CBOXGROUP: {
                    KeySet = *GET_FIELD_PTR(Dsn, &DsnKeys[Changed], BOOL);
                }
                    break;
                case DSN_TYPE_INT: {
                    KeySet = *GET_FIELD_PTR(Dsn, &DsnKeys[Changed], int) != 0;
                }
            }

            /* No problem to deal with aliases here as well, but let's keep things simple */
            if (DsnKeys[DsnKeysSwitch[i].Dependent].IsAlias != 0) {
                return FALSE;
            }

            switch (DsnKeys[DsnKeysSwitch[i].Dependent].Type) {
                case DSN_TYPE_BOOL:
                    *GET_FIELD_PTR(Dsn, &DsnKeys[DsnKeysSwitch[i].Dependent], BOOL) =
                            DsnKeysSwitch[i].Same == KeySet ? 1 : 0;
                    break;
                case DSN_TYPE_OPTION:
                    TAOS_SetOptionValue(Dsn, &DsnKeys[DsnKeysSwitch[i].Dependent],
                                        DsnKeysSwitch[i].Same == KeySet ? 1 : 0);
                    break;
                default:
                    return FALSE; /* Only boolean fields are supported as dependent atm */
            }
        }
    }

    return TRUE;
}
/* }}} */


/* {{{ TAOS_DsnStoreValue */
BOOL TAOS_DsnStoreValue(TAOS_Dsn *Dsn, unsigned int DsnKeyIdx, SQLCHAR *Value, BOOL OverWrite) {
    TAOS_DsnKey *DsnKey = &DsnKeys[DsnKeyIdx];
    if (!Dsn || DsnKey->IsAlias)
        return FALSE;
    switch (DsnKey->Type) {
        case DSN_TYPE_STRING:
        case DSN_TYPE_COMBO: {
            SQLCHAR **p = GET_FIELD_PTR(Dsn, DsnKey, char*);
            if (*p && OverWrite == FALSE)
                break;
            /* For the case of making copy of currently stored values */
            TAOS_RESET(*p, Value);
        }
            break;
        case DSN_TYPE_BOOL:
            /* If value is not set or we may overwrite it */
            if (!(*GET_FIELD_PTR(Dsn, DsnKey, BOOL) && OverWrite == FALSE)) {
                *GET_FIELD_PTR(Dsn, DsnKey, BOOL) = atoi(Value);
            }
            break;
        case DSN_TYPE_CBOXGROUP:
            /* If value is not set or we may overwrite it */
            if (!(*GET_FIELD_PTR(Dsn, DsnKey, char) && OverWrite == FALSE)) {
                SQLCHAR IntValue = atoi(Value);

                /* Atm we have only one DSN_TYPE_CBOXGROUP!!!, and following works only for it. If sometime another such field is added,
                   we will need another data structure array, that will bind DSN field with string values and bits for this field.
                   So far we use hardcoded arrays for the singe such field we have atm */
                if (IntValue == '\0') {
                    unsigned int i;

                    IntValue = 0;
                    for (i = 0; i < sizeof(TlsVersionBits); ++i) {
                        if (strcasestr(Value, TlsVersionName[i]) != NULL) {
                            IntValue |= TlsVersionBits[i];
                        }
                    }
                }
                *GET_FIELD_PTR(Dsn, DsnKey, char) = IntValue;
            }
            break;
        case DSN_TYPE_INT:
            if (*GET_FIELD_PTR(Dsn, DsnKey, int) && OverWrite == FALSE)
                break;
            *GET_FIELD_PTR(Dsn, DsnKey, int) = strtoul(Value, NULL, 10);
            break;
        case DSN_TYPE_OPTION:
            if (*GET_FIELD_PTR(Dsn, DsnKey, BOOL) && OverWrite == FALSE)
                break;
            TAOS_SetOptionValue(Dsn, DsnKey, strtoul(Value, NULL, 10) != 0 ? 1 : 0);

            break;
    }
    return TAOS_DsnSwitchDependents(Dsn, DsnKeyIdx);
}
/* }}} */

/* {{{ TAOS_DsnUpdateOptionsFields */
void TAOS_DsnUpdateOptionsFields(TAOS_Dsn *Dsn) {
    int i = 0;

    while (DsnKeys[i].DsnKey != NULL) {
        if (DsnKeys[i].IsAlias == 0) {
            if (DsnKeys[i].Type == DSN_TYPE_OPTION) {
                *GET_FIELD_PTR(Dsn, &DsnKeys[i], BOOL) = (BOOL) (DSN_OPTION(Dsn, DsnKeys[i].FlagValue) ? 1 : 0);
                TAOS_DsnSwitchDependents(Dsn, i);
            }
        }
        ++i;
    }
}
/* }}} */

/* {{{ TAOS_ReadDSN */
BOOL TAOS_ReadDSN(TAOS_Dsn *Dsn, const SQLCHAR *KeyValue, BOOL OverWrite) {
    SQLCHAR *Value;
    /* if no key/value pair was specified, we will try to read Dsn->DSNName */
    if (!KeyValue) {
        Value = Dsn->DSNName;
    } else {
        if ((Value = strchr(KeyValue, '=')) != NULL) {
            ++Value;
            TAOS_RESET(Dsn->DSNName, Value);
        }
    }

    if (Value) {
        int i = 1;
        SQLCHAR KeyVal[1024];

        while (DsnKeys[i].DsnKey) {
            unsigned int KeyIdx = DsnKeys[i].IsAlias ? DsnKeys[i].DsnOffset : i;

            if (SQLGetPrivateProfileString(Dsn->DSNName, DsnKeys[i].DsnKey, "", KeyVal, 1024, "ODBC.INI") > 0) {
                if (!TAOS_DsnStoreValue(Dsn, KeyIdx, KeyVal, OverWrite))
                    return FALSE;
            } else if (DsnKeys[i].Type == DSN_TYPE_OPTION) {
                *GET_FIELD_PTR(Dsn, &DsnKeys[KeyIdx], BOOL) = (BOOL) (DSN_OPTION(Dsn, DsnKeys[KeyIdx].FlagValue) ? 1
                                                                                                                 : 0);
            }
            ++i;
        }
        return TRUE;
    }
    return FALSE;
}

/* }}} */

BOOL TAOS_DSN_Exists(const SQLCHAR *DsnName) {
    BOOL ret;
    SQLCHAR buffer[1024];
    SQLCHAR *p = "";

    if (!DsnName)
        return FALSE;

    ret = (SQLGetPrivateProfileString(DsnName, NULL, p, buffer, 1024, "ODBC.INI") > 0);
    return ret;
}

/* {{{ TAOS_SaveDSN */
BOOL TAOS_SaveDSN(TAOS_Dsn *Dsn) {
    int i = 1;
    SQLCHAR Value[32];
    BOOL ret;
    DWORD ErrNum;

    if (!SQLValidDSN(Dsn->DSNName)) {
        strcpy_s(Dsn->ErrorMsg, SQL_MAX_MESSAGE_LENGTH, "Invalid Data Source Name");
        return FALSE;
    }

    if (!SQLRemoveDSNFromIni(Dsn->DSNName)) {
        SQLInstallerError(1, &ErrNum, Dsn->ErrorMsg, SQL_MAX_MESSAGE_LENGTH, NULL);
        return FALSE;
    }
    if (!SQLWriteDSNToIni(Dsn->DSNName, Dsn->Driver)) {
        SQLInstallerError(1, &ErrNum, Dsn->ErrorMsg, SQL_MAX_MESSAGE_LENGTH, NULL);
        return FALSE;
    }

    while (DsnKeys[i].DsnKey) {
        /* Skipping aliases - options are saved by primary name only */
        if (!DsnKeys[i].IsAlias) {
            ret = TRUE;
            printf("key: %s, idx: %d\n", DsnKeys[i].DsnKey, i);
            /* We do not save DSN_TYPE_OPTION - they are saved as OPTIONS bits */
            switch (DsnKeys[i].Type) {
                case DSN_TYPE_BOOL:
                    printf("Val %s \n", *GET_FIELD_PTR(Dsn, &DsnKeys[i], BOOL) ? "1" : "0");
                    ret = SQLWritePrivateProfileString(Dsn->DSNName, DsnKeys[i].DsnKey,
                                                       *GET_FIELD_PTR(Dsn, &DsnKeys[i], BOOL) ? "1" : "0", "ODBC.INI");
                    break;
                case DSN_TYPE_INT: {
                    snprintf(Value, 32, "%d", *(int *) ((SQLCHAR *) Dsn + DsnKeys[i].DsnOffset));
                    printf("Val %s \n", Value);
                    ret = SQLWritePrivateProfileString(Dsn->DSNName, DsnKeys[i].DsnKey, Value, "ODBC.INI");
                }
                    break;
                case DSN_TYPE_CBOXGROUP: {
                    snprintf(Value, 32, "%hu", (short) *GET_FIELD_PTR(Dsn, &DsnKeys[i], char));
                    printf("Val %s \n", Value);
                    ret = SQLWritePrivateProfileString(Dsn->DSNName, DsnKeys[i].DsnKey, Value, "ODBC.INI");
                }
                    break;
                case DSN_TYPE_STRING:
                case DSN_TYPE_COMBO: {
                    SQLCHAR *Val = *GET_FIELD_PTR(Dsn, &DsnKeys[i], char*);
                    printf("Val %s \n", Val);
                    if (Val && Val[0]) {
                        ret = SQLWritePrivateProfileString(Dsn->DSNName, DsnKeys[i].DsnKey, Val, "ODBC.INI");
                    }
                }
                default:
                    /* To avoid warning with some compilers */
                    break;
            }  /* switch */

            if (!ret) {
                SQLInstallerError(1, &ErrNum, Dsn->ErrorMsg, SQL_MAX_MESSAGE_LENGTH, NULL);
                printf("Key Err %s\n", Dsn->ErrorMsg);
                return FALSE;
            } else {
                printf("Key %s saved\n", DsnKeys[i].DsnKey);
            }
        }
        i++;
    }
    /* Save Options */
//    snprintf(Value, 32, "%d", Dsn->Options);
//    if (!(ret = SQLWritePrivateProfileString(Dsn->DSNName, "OPTIONS", Value, "ODBC.INI"))) {
//        SQLInstallerError(1, &ErrNum, Dsn->ErrorMsg, SQL_MAX_MESSAGE_LENGTH, NULL);
//        return FALSE;
//    }
    return TRUE;
}

/* }}} */


size_t ConnStringLength(const SQLCHAR *String, SQLCHAR *Delimiter) {
    size_t result = strlen(String);
    const SQLCHAR *p = String + result + 1;

    if (Delimiter != "\0") {
        return result;
    }
    /* else - we have string with null terminated key=value pairs with additional NULL after last pair */
    while (*p) {
        p += strlen(p) + 1;
    }

    return p - String /* Length without ending NULL */;
}


/* {{{ TAOS_ParseConnString */
BOOL TAOS_ParseConnString(TAOS_Dsn *Dsn, const SQLCHAR *connStr, size_t Length, const SQLCHAR *Delimiter) {
    SQLCHAR *Buffer;
    BOOL ret = TRUE;

    if (!connStr)
        return FALSE;

    if (Length == SQL_NTS) {
        Length = ConnStringLength(connStr, Delimiter);
    }
    Buffer = malloc(Length + 1);
    memset(Buffer, 0, Length + 1);
    Buffer = memcpy(Buffer, connStr, Length + 1);
    printf("parsing conn str %s, deli %s\n", Buffer, Delimiter);
    int i = 0;
    char *kv = strtok(Buffer, Delimiter);
    while (kv != NULL) {
        char *vs = strchr(kv, '=');
        if (vs != NULL) {
            int ks = vs - kv;
            char *key = malloc(ks + 1);
            memset(key, 0, ks + 1);
            memcpy(key, kv, ks);
            *(key + ks + 1) = '\0';
            for (int j = 0; j < 8; j++) {
                if (_stricmp(key, DsnKeys[j].DsnKey) == 0) {
                    char *val = vs + 1;
                    if (DsnKeys[j].IsAlias) {
                        j = DsnKeys[j].DsnOffset; /* For aliases DsnOffset is index of aliased "main" key */
                    }
                    ret = TAOS_DsnStoreValue(Dsn, j, val, TRUE);
                    i++;
                    break;
                }
            }
        }
        kv = strtok(NULL, Delimiter);
    }
    TAOS_FREE(Buffer);
    return ret;
}
//BOOL TAOS_ParseConnString(TAOS_Dsn *Dsn, const SQLCHAR *String, size_t Length, SQLCHAR Delimiter) {
//    SQLCHAR *Buffer, *Key, *Value, *ValueBuf;
//    BOOL ret = TRUE;
//
//    if (!String)
//        return FALSE;
//
//    if (Length == SQL_NTS) {
//        Length = ConnStringLength(String, Delimiter);
//    }
//
//    Buffer = malloc(Length + 1);
//    Buffer = memcpy(Buffer, String, Length + 1);
//    Key = Buffer;
//    ValueBuf = malloc(Length - 4); /*DSN=<value> - DSN or DRIVER must be in */
//
//    while (Key && Key < ((SQLCHAR *) Buffer + Length)) {
//        int i = 0;
//
//        /* The case of ;; - "empty key/value pair. Probably that shouldn't be allowed. But parser uset to digest this, so leaving this as a feature so far
//           TODO: check and maybe remove for the next version */
//        if (Delimiter != '\0' && *Key == Delimiter) {
//            ++Key;
//            continue;
//        }
//        if (!(Value = strchr(Key, '='))) {
//            ret = FALSE;
//            break;
//        }
//
//        *Value = 0;
//        ++Value;
//        Key = trim(Key);
//
//        while (DsnKeys[i].DsnKey) {
//            if (_stricmp(DsnKeys[i].DsnKey, Key) == 0) {
//                SQLCHAR *p = NULL;
//
//                if (DsnKeys[i].IsAlias) {
//                    i = DsnKeys[i].DsnOffset; /* For aliases DsnOffset is index of aliased "main" key */
//                }
//
//                Value = ltrim(Value);
//
//                if (Value[0] == '{') {
//                    SQLCHAR *valueBufPtr = ValueBuf;
//                    SQLCHAR *prev = ++Value;
//                    *valueBufPtr = '\0';
//                    while ((p = strchr(prev, '}')) != NULL) {
//                        memcpy(valueBufPtr, prev, p - prev);
//                        valueBufPtr += p - prev;
//                        if (*(p + 1) == '}') {
//                            *(valueBufPtr++) = '}';
//                            *valueBufPtr = '\0';
//                            prev = p + 2;
//                        } else {
//                            *valueBufPtr = '\0';
//                            ++p;
//                            break;
//                        }
//                    }
//                    Value = ValueBuf;
//                } else if ((p = strchr(Value, Delimiter))) {
//                    *p = 0;
//                }
//                /* TODO: 3.2 we should not trim enclosed in braces, I think */
//                Value = trim(Value);
//
//                /* Overwriting here - if an option repeated more than once in the string, its last entrance will determine the value */
//                if (!TAOS_DsnStoreValue(Dsn, i, Value, TRUE)) {
//                    ret = FALSE;
//                    goto end;
//                }
//                if (IS_OPTIONS_BITMAP(i)) {
//                    TAOS_DsnUpdateOptionsFields(Dsn);
//                }
//
//                if (p) {
//                    Key = p + 1;
//                } else {
//                    Key = NULL;
//                }
//                break;
//            }
//            ++i;
//        }
//        /* Unknown keyword */
//        if (DsnKeys[i].DsnKey == NULL) {
//            //TODO: shouldn't some error/warning be thrown?
//            Key = strchr(Value, Delimiter);
//            if (Key != NULL) {
//                ++Key;
//            }
//        }
//    }
//
//    end:
//    TAOS_FREE(Buffer);
//    TAOS_FREE(ValueBuf);
//
//    return ret;
//}
/* }}} */

/* {{{ TAOS_ReadConnString */
/* Like ParseConnString, but expands DSN if needed, preserving connection string values precedence.
   Or in other words - it is combination of ReadDsn and ParseConnString */
BOOL TAOS_ReadConnString(TAOS_Dsn *Dsn, const SQLCHAR *String, size_t Length, SQLCHAR Delimiter) {
    /* Basically at this point we need DSN name only */
    if (!TAOS_ParseConnString(Dsn, String, Length, Delimiter)) {
        return FALSE;
    }

    /* "If the connection string contains the DRIVER keyword, the driver cannot retrieve information about the data source
       from the system information." https://msdn.microsoft.com/en-us/library/ms715433%28v=vs.85%29.aspx */
    if (Dsn->DSNName && TAOS_IS_EMPTY(Dsn->Driver)) {
        TAOS_ReadDSN(Dsn, NULL, FALSE);
        /* This redundancy is needed to be able to reset options set in the DSN, e.g. if DSN has Reconnect option selected, and
           connection string has AUTO_RECONNECT=0. Connection string should have precedence */
        TAOS_ParseConnString(Dsn, String, Length, Delimiter);
    }
    return TRUE;
}
/* }}} */

/* {{{ TAOS_DsnToString */
SQLULEN TAOS_DsnToString(TAOS_Dsn *Dsn, SQLCHAR *OutString, SQLULEN OutLength) {
    int i = 0;
    SQLULEN TotalLength = 0;
    SQLCHAR *p = OutString;
    SQLCHAR *Value = NULL;
    SQLCHAR TmpStr[1024] = {'\0'};
    SQLCHAR IntVal[12];
    int CpyLength;

    if (OutLength && OutString)
        OutString[0] = '\0';

    while (DsnKeys[i].DsnKey) {
        Value = NULL;

        if (!DsnKeys[i].IsAlias) {
            switch (DsnKeys[i].Type) {
                case DSN_TYPE_STRING:
                case DSN_TYPE_COMBO:
                    Value = *GET_FIELD_PTR(Dsn, &DsnKeys[i], char*);
                    if (TAOS_IS_EMPTY(Value)) {
                        ++i;
                        continue;
                    }
                    break;
                case DSN_TYPE_INT:
                    if (*GET_FIELD_PTR(Dsn, &DsnKeys[i], int)) {
                        snprintf(IntVal, sizeof(IntVal), "%d", *GET_FIELD_PTR(Dsn, &DsnKeys[i], int));
                        Value = IntVal;
                    }
                    break;
                case DSN_TYPE_BOOL:
                    if (*GET_FIELD_PTR(Dsn, &DsnKeys[i], BOOL)) {
                        Value = "1";
                    }
                    break;
                case DSN_TYPE_CBOXGROUP:
                    if (*GET_FIELD_PTR(Dsn, &DsnKeys[i], char)) {
                        snprintf(IntVal, sizeof(IntVal), "%hu", (short) *GET_FIELD_PTR(Dsn, &DsnKeys[i], char));
                        Value = IntVal;
                    }
                    break;
                default:
                    /* To avoid warning with some compilers */
                    break;
            }
        }

        if (Value) {
            BOOL isSpecial = (strchr(Value, ' ') || strchr(Value, ';') || strchr(Value, '@'));
            CpyLength = snprintf(TmpStr + TotalLength, 1024 - TotalLength, "%s%s=%s%s%s", (TotalLength) ? ";" : "",
                                 DsnKeys[i].DsnKey, isSpecial ? "{" : "", Value, isSpecial ? "}" : "");
            TotalLength += CpyLength;
        }
        ++i;
    }

    if (OutLength && OutString) {
        strncpy_s(OutString, OutLength, TmpStr, TotalLength);
    }
    return TotalLength;
}
/* }}} */
