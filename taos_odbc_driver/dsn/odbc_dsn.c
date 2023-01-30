

#include <Windows.h>
#include <stdlib.h>
#include <CommCtrl.h>
#include <tchar.h>
#include <windowsx.h>
#include <winuser.h>
#include <shobjidl.h>
#include <taos.h>
#include <sqlext.h>
#include "resource.h"
#include "td_dsn.h"

#pragma comment(lib, "ComCtl32.lib")

#define CONNSTR_BUFFER_SIZE 1024
#define LASTPAGE 2

HINSTANCE hInstance;
unsigned int CurrentPage = 0;
SQLHANDLE Environment = NULL;
BOOL DBFilled = FALSE;
BOOL CSFilled = FALSE;
BOOL ConnectionOK = FALSE;
BOOL notCanceled = TRUE;

SQLCHAR DSNStr[2048];
HWND hwndTab[7], hwndMain;
const int *EffectiveDisabledPages = NULL,
        *EffectiveDisabledControls = NULL;
BOOL OpenCurSelection = TRUE;

/* On Windows we are supposed to have schannel, or we can have openssl */
#if defined(_WIN32) || defined(HAVE_OPENSSL)
# define  SSL_DISABLED 0
#else
# define  SSL_DISABLED 1
#endif

const int DisabledPages[ODBC_PROMPT_REQUIRED + 1][LASTPAGE + 1] = {
        {0, 0, 0},
        {0, 0, 0},
        {1, 0, 1}
};
const int PromptDisabledControls[] = {txtDsnName, 0},
        EmptyDisabledControls[] = {0};
const int *DisabledControls[] = {
        EmptyDisabledControls,
        PromptDisabledControls,
        EmptyDisabledControls
};

TAOS_DsnMap DsnMap[] = {
        {&DsnKeys[1], 0, txtDsnName,        64,  1},
        {&DsnKeys[2], 0, txtDSNDescription, 64,  0},
        {&DsnKeys[3], 1, txtServerName,     128, 0},
        {&DsnKeys[4], 1, txtUserName,       64,  0},
        {&DsnKeys[5], 1, txtPassword,       64,  0},
        {&DsnKeys[6], 1, cbDatabase,        0,   0},
        {&DsnKeys[7], 1, txtPort,           5,   0},
        {NULL,        0, 0,                 0,   0}
};

#define CBGROUP_BIT(MapIdx)             (char)(DsnMap[MapIdx].MaxLength)
#define IS_CB_CHECKED(MapIdx)           GetButtonState(DsnMap[MapIdx].Page, DsnMap[MapIdx].Item)
#define CBGROUP_SETBIT(_Dsn, MapIdx)    *GET_FIELD_PTR(_Dsn, DsnMap[MapIdx].Key, char)|= CBGROUP_BIT(MapIdx)
#define CBGROUP_RESETBIT(_Dsn, MapIdx)  *GET_FIELD_PTR(_Dsn, DsnMap[MapIdx].Key, char)&= ~CBGROUP_BIT(MapIdx)

void GetDialogFields(void);

HBRUSH hbrBg = NULL;


void DsnApplyDefaults(TAOS_Dsn *Dsn) {
    Dsn->Driver = "TAOS_ODBC";
    if (Dsn->Description == NULL) {
        Dsn->Description = "";
    }
    if (Dsn->Database == NULL) {
        Dsn->Database = "";
    }
    if (Dsn->UserName == NULL) {
        Dsn->UserName = "";
    }
    if (Dsn->Password == NULL) {
        Dsn->Password = "";
    }
    if (Dsn->Port == NULL || Dsn->Port == 0) {
        Dsn->Port = 6030;
    }
}


BOOL SetDialogFields() {
    int i = 0;
    TAOS_Dsn *Dsn = (TAOS_Dsn *) GetWindowLongPtr(GetParent(hwndTab[0]), DWLP_USER);

    /* Basically - if dialog does not exist yet */
    if (Dsn == NULL) {
        return TRUE;
    }
    while (DsnMap[i].Key) {
        switch (DsnMap[i].Key->Type) {
            case DSN_TYPE_STRING:
            case DSN_TYPE_COMBO: {
                SQLCHAR *Val = *(SQLCHAR **) ((SQLCHAR *) Dsn + DsnMap[i].Key->DsnOffset);
                if (Val && Val[0])
                    SetDlgItemText(hwndTab[DsnMap[i].Page], DsnMap[i].Item, Val);
                break;
            }
            case DSN_TYPE_INT: {
                int Val = *(int *) ((SQLCHAR *) Dsn + DsnMap[i].Key->DsnOffset);
                SetDlgItemInt(hwndTab[DsnMap[i].Page], DsnMap[i].Item, Val, 0);
            }
                break;
            case DSN_TYPE_BOOL: {
                BOOL Val = *(BOOL *) ((SQLCHAR *) Dsn + DsnMap[i].Key->DsnOffset);
                SendDlgItemMessage(hwndTab[DsnMap[i].Page], DsnMap[i].Item, BM_SETCHECK,
                                   Val ? BST_CHECKED : BST_UNCHECKED, 0);
            }
                break;
            case DSN_TYPE_CBOXGROUP:
                SendDlgItemMessage(hwndTab[DsnMap[i].Page], DsnMap[i].Item, BM_SETCHECK,
                                   (*GET_FIELD_PTR(Dsn, DsnMap[i].Key, char) & CBGROUP_BIT(i)) != '\0' ? BST_CHECKED
                                                                                                       : BST_UNCHECKED,
                                   0);
        }
        i++;
    }
    i = 0;
    return TRUE;

}

SQLCHAR *GetFieldStrVal(int Dialog, int Field, SQLCHAR *(*allocator)(size_t)) {
    int rc;
    int len = Edit_GetTextLength(GetDlgItem(hwndTab[Dialog], Field));
    SQLCHAR *p;

    if (allocator) {
        p = allocator(len * sizeof(char) + 2);
    } else {
        p = (SQLCHAR *) malloc(len * sizeof(char) + 2);
    }

    if (p)
        rc = Edit_GetText(GetDlgItem(hwndTab[Dialog], Field), p, len + 1);
    return p;
}

int GetFieldIntVal(int Dialog, int Field) {
    int rc = 0;
    SQLCHAR *p = GetFieldStrVal(Dialog, Field, NULL);
    if (p) {
        rc = atoi(p);
        free(p);
    }
    return rc;
}

BOOL GetButtonState(int Dialog, int Button) {
    return (BOOL) IsDlgButtonChecked(hwndTab[Dialog], Button);
}


BOOL SaveDSN(HWND hDlg, TAOS_Dsn *Dsn) {
    DsnApplyDefaults(Dsn);
    if (Dsn->SaveFile != NULL || TAOS_SaveDSN(Dsn))
        return TRUE;
    MessageBox(hDlg, Dsn->ErrorMsg, "Error", MB_OK);
    return FALSE;
}


unsigned int GetNextActiveTab(int current_page, int offset) {
    unsigned int result = current_page + offset;

    for (; result >= 0 && result <= LASTPAGE && EffectiveDisabledPages[result] != 0; result += offset) {
        if (offset > 1) {
            offset = 1;
        } else if (offset < -1) {
            offset = -1;
        }
    }

    if (result < 0 || result > LASTPAGE) {
        result = current_page;
    }

    return result;
}


#define DISABLE_CONTROLS(page_map) i= 0;\
  /* Assuming that controls in the #page_map are sorted by tab page */\
  while (page_map[i].Item && page_map[i].Page != CurrentPage && ++i);\
\
  while (page_map[i].Item && page_map[i].Page == CurrentPage)\
  {\
    int j= 0;\
    while (EffectiveDisabledControls[j])\
    {\
      if (EffectiveDisabledControls[j] == page_map[i].Item)\
      {\
        EnableWindow(GetDlgItem(hwndTab[CurrentPage], EffectiveDisabledControls[j]), FALSE);\
      }\
      ++j;\
    }\
    ++i;\
  } 1

void DisableControls(TAOS_Dsn *Dsn) {
    int i;

    DISABLE_CONTROLS(DsnMap);
}


void SetPage(HWND hDlg, int value) {
    TAOS_Dsn *Dsn = (TAOS_Dsn *) GetWindowLongPtr(GetParent(hwndTab[0]), DWLP_USER);
    unsigned int new_page = GetNextActiveTab(CurrentPage, value);

    /* Save if last page or all following pages are disabled */
    if (value > 0 && (CurrentPage == LASTPAGE || new_page == CurrentPage)) {
        GetDialogFields();
        if (Dsn->isPrompt || SaveDSN(hDlg, Dsn)) {
            SendMessage(hwndMain, WM_DESTROY, 0, 0);
        }
        return;
    }

    ShowWindow(hwndTab[CurrentPage != (unsigned int) (-1) ? CurrentPage : 0], SW_HIDE);

    CurrentPage = new_page;

    ShowWindow(hwndTab[CurrentPage], SW_SHOW);

    DisableControls(Dsn);

    /* Disabling prev button if needed*/
    new_page = GetNextActiveTab(CurrentPage, -1);
    EnableWindow(GetDlgItem(hwndTab[CurrentPage], PB_PREV), (CurrentPage != new_page) ? TRUE : FALSE);

    /* Switching caption of the Next/Finish button if needed*/
    new_page = GetNextActiveTab(CurrentPage, 1);
    SendMessage(GetDlgItem(hwndTab[CurrentPage], PB_NEXT), WM_SETTEXT, 0,
                (CurrentPage == new_page) ? (LPARAM) "Finish" : (LPARAM) "Next >");
    SetFocus(hwndTab[CurrentPage]);

    /* If not a prompt - disable finish button in case of empty DS name(for prompt it may be empty/invalid)
       TODO: I think it rather has to check if the name is valid DS name */
    if (Dsn->isPrompt == ODBC_CONFIG && CurrentPage == new_page) {
        EnableWindow(GetDlgItem(hwndTab[CurrentPage], PB_NEXT),
                     Edit_GetTextLength(GetDlgItem(hwndTab[0], txtDsnName)) ? TRUE : FALSE);
    }
}

void GetDialogFields() {
    int i = 0;
    TAOS_Dsn *Dsn = (TAOS_Dsn *) GetWindowLongPtr(GetParent(hwndTab[0]), DWLP_USER);
    while (DsnMap[i].Key) {
        printf("DsnOffset%d\n", DsnMap[i].Key->DsnOffset);
        switch (DsnMap[i].Key->Type) {
            case DSN_TYPE_STRING:
            case DSN_TYPE_COMBO: {
                SQLCHAR **p = (SQLCHAR **) ((SQLCHAR *) Dsn + DsnMap[i].Key->DsnOffset);

                if (Dsn->isPrompt != 0 && Dsn->free != NULL) {
                    Dsn->free(*p);
                }
                *p = GetFieldStrVal(DsnMap[i].Page, DsnMap[i].Item,
                                    Dsn->allocator && Dsn->isPrompt ? Dsn->allocator : NULL);
            }
                break;
            case DSN_TYPE_INT:
                *(int *) ((SQLCHAR *) Dsn + DsnMap[i].Key->DsnOffset) = GetFieldIntVal(DsnMap[i].Page, DsnMap[i].Item);
                break;
            case DSN_TYPE_BOOL:
                *GET_FIELD_PTR(Dsn, DsnMap[i].Key, BOOL) = IS_CB_CHECKED(i);
                break;
            case DSN_TYPE_CBOXGROUP:
                if (IS_CB_CHECKED(i) != '\0') {
                    CBGROUP_SETBIT(Dsn, i);
                } else {
                    CBGROUP_RESETBIT(Dsn, i);
                }
        }
        ++i;
    }
    DsnApplyDefaults(Dsn);
    i = 0;
    Dsn->Options = 0;
}

void DSN_Set_Database(SQLHANDLE Connection) {
    TAOS_STMT *Stmt = NULL;
    SQLRETURN ret = SQL_ERROR;
    SQLCHAR Database[65];
    TAOS_Dsn *Dsn = (TAOS_Dsn *) GetWindowLongPtr(GetParent(hwndTab[0]), DWLP_USER);
    HWND DbCombobox = GetDlgItem(hwndTab[1], cbDatabase);


    if (DBFilled)
        return;

    GetDialogFields();

    if (SQLAllocHandle(SQL_HANDLE_STMT, Connection, (SQLHANDLE *) &Stmt) != SQL_SUCCESS)
        goto end;

    if (SQLExecDirect((SQLHSTMT) Stmt, (SQLCHAR *) "SHOW DATABASES", SQL_NTS) != SQL_SUCCESS)
        goto end;

    SQLBindCol(Stmt, 1, SQL_C_CHAR, Database, 65, 0);
    ComboBox_ResetContent(DbCombobox);
    while (SQLFetch(Stmt) == SQL_SUCCESS) {
        ComboBox_InsertString(DbCombobox, -1, Database);
    }
    if (Dsn->Database) {
        int Idx = ComboBox_FindString(DbCombobox, 0, Dsn->Database);
        ComboBox_SetCurSel(DbCombobox, Idx);
    }
    ComboBox_SetMinVisible(GetDlgItem(hwndTab[1], cbDatabase), 5);
    DBFilled = TRUE;

    end:
    if (Stmt)
        SQLFreeHandle(SQL_HANDLE_STMT, (SQLHANDLE) Stmt);
}


static SQLCHAR *conn_str_varkeys[] = {"dsn", "dsnname", "server", "ip", "addr", "port",
                                      "user", "uid", "password", "pwd", "database", "db"};

static SQLCHAR *conn_str_keys[] = {"dsn", "dsn", "ip", "ip", "ip", "port",
                                   "uid", "uid", "pwd", "pwd", "db", "db"};


static SQLRETURN TestDSN(TAOS_Dsn *Dsn, SQLHANDLE *Conn, SQLCHAR *ConnStrBuffer) {
    SQLHANDLE Connection = NULL;
    SQLRETURN Result = SQL_ERROR;
    SQLCHAR LocalBuffer[CONNSTR_BUFFER_SIZE], *ConnStr = LocalBuffer;
    SQLCHAR *Description = Dsn->Description;
    SQLCHAR Info[1024];
    Dsn->Description = "";

    if (ConnStrBuffer != NULL) {
        ConnStr = ConnStrBuffer;
    }

    DsnApplyDefaults(Dsn);
    /* If defaults has changed actual values - let them be reflected in the dialog(if it exists - SetDialogFields
       cares about that) */
    SetDialogFields();
    TAOS_DsnToString(Dsn, ConnStr, CONNSTR_BUFFER_SIZE);
    _snprintf(Info, sizeof(Info), "dsn: %s, ip: %s, port: %d\n", Dsn->DSNName, Dsn->ServerName, Dsn->Port);
    SQLAllocHandle(SQL_HANDLE_DBC, Environment, (SQLHANDLE *) &Connection);
    if (Connection == NULL) {
        return Result;
    }
    MessageBox(hwndTab[CurrentPage], Info, "Connection test", MB_ICONINFORMATION | MB_OK);
    Result = SQLDriverConnect(Connection, NULL, ConnStr, CONNSTR_BUFFER_SIZE, NULL, 0, NULL, SQL_DRIVER_NOPROMPT);

    Dsn->Description = Description;

    if (Conn != NULL) {
        *Conn = Connection;
    } else {
        SQLDisconnect(Connection);
        SQLFreeHandle(SQL_HANDLE_DBC, Connection);
    }
    return Result;
}


/* Connstr has to be null-terminated */
SQLCHAR *HidePwd(SQLCHAR *ConnStr) {
    SQLCHAR *Ptr = ConnStr;

    while (*Ptr) {
        BOOL IsPwd = FALSE;
        SQLCHAR *KeyValBorder = strchr(Ptr, '=');
        SQLCHAR StopChr = ';';

        Ptr = ltrim(Ptr);

        if (_strnicmp(Ptr, "PWD", 3) == 0 || _strnicmp(Ptr, "PASSWORD", 8) == 0) {
            IsPwd = TRUE;
        }
        if (KeyValBorder != NULL) {
            Ptr = ltrim(KeyValBorder + 1);
        }
        if (*Ptr == '{') {
            StopChr = '}';
        }
        while (*Ptr && *Ptr != StopChr) {
            if (IsPwd) {
                *Ptr = '*';
            }
            ++Ptr;
        }
        ++Ptr;
    }

    return ConnStr;
}


void TAOS_WIN_TestDsn(BOOL ShowSuccess) {
    SQLHANDLE Connection;
    SQLRETURN ret;
    GetDialogFields();
    TAOS_Dsn *Dsn = (TAOS_Dsn *) GetWindowLongPtr(GetParent(hwndTab[0]), DWLP_USER);
    SQLCHAR ConnStr[CONNSTR_BUFFER_SIZE];

    ret = TestDSN(Dsn, &Connection, ConnStr);

    if (ShowSuccess) {
        SQLCHAR Info[1024];

        HidePwd(ConnStr);

        if (SQL_SUCCEEDED(ret)) {
            SQLCHAR DbmsName[16], DbmsVer[16];
            SQLGetInfo(Connection, SQL_DBMS_NAME, DbmsName, sizeof(DbmsName), NULL);
            SQLGetInfo(Connection, SQL_DBMS_VER, DbmsVer, sizeof(DbmsVer), NULL);
            _snprintf(Info, sizeof(Info),
                      "Connection successfully established\n\nServer Information: %s %s\n\nConnection String:\n\n%s",
                      DbmsName, DbmsVer, ConnStr);
            MessageBox(hwndTab[CurrentPage], Info, "Connection test", MB_ICONINFORMATION | MB_OK);
        } else {
            SQLCHAR SqlState[6], ErrMsg[SQL_MAX_MESSAGE_LENGTH];
            SQLGetDiagRec(SQL_HANDLE_DBC, Connection, 1, SqlState, NULL, ErrMsg, SQL_MAX_MESSAGE_LENGTH, NULL);
            _snprintf(Info, sizeof(Info), "Connection failed: [%s] %s\n\nConnection String:\n\n%s", SqlState, ErrMsg,
                      ConnStr);
            MessageBox(hwndTab[CurrentPage], Info, "Connection test", MB_ICONINFORMATION | MB_OK);
        }
    }

    if (SQL_SUCCEEDED(ret)) {
        ConnectionOK = TRUE;
        DSN_Set_Database(Connection);

        SQLDisconnect(Connection);
    }

    SQLFreeHandle(SQL_HANDLE_DBC, Connection);
}


INT_PTR SelectPath(HWND ParentWnd,
                   int BoundEditId,
                   const wchar_t *Caption,
                   BOOL FolderPath,
                   BOOL OpenCurrentSelection) {
    if (SUCCEEDED(CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
        IFileDialog *PathDialog;

        if (SUCCEEDED(CoCreateInstance(&CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
                                       &IID_IFileOpenDialog, (void **) &PathDialog))) {
            HWND BoundEditWnd = GetDlgItem(ParentWnd, BoundEditId);
            DWORD DialogOptions;
            TCHAR Path[MAX_PATH];
            IShellItem *SelectedItem;
            int Length;
            LPWSTR wPath;

            if (FolderPath && SUCCEEDED(PathDialog->lpVtbl->GetOptions(PathDialog, &DialogOptions))) {
                PathDialog->lpVtbl->SetOptions(PathDialog, DialogOptions | FOS_PICKFOLDERS);
            }

            Edit_GetText(BoundEditWnd, Path, sizeof(Path));
            Length = MultiByteToWideChar(GetACP(), 0, Path, -1, NULL, 0);

            wPath = (SQLWCHAR *) malloc(Length * sizeof(SQLWCHAR));
            if (wPath == NULL) {
                return FALSE;
            }

            MultiByteToWideChar(GetACP(), 0, Path, -1, wPath, Length);

            SHCreateItemFromParsingName(wPath, NULL, &IID_IShellItem, (void **) &SelectedItem);
            if (OpenCurrentSelection) {
                PathDialog->lpVtbl->SetFolder(PathDialog, SelectedItem);
            } else {
                PathDialog->lpVtbl->SetDefaultFolder(PathDialog, SelectedItem);
            }
            SelectedItem->lpVtbl->Release(SelectedItem);

            free(wPath);

            PathDialog->lpVtbl->SetTitle(PathDialog, Caption);

            if (SUCCEEDED(PathDialog->lpVtbl->Show(PathDialog, ParentWnd))) {
                if (SUCCEEDED(PathDialog->lpVtbl->GetResult(PathDialog, &SelectedItem))) {
                    LPWSTR SelectedValue;
                    if (SUCCEEDED(
                            SelectedItem->lpVtbl->GetDisplayName(SelectedItem, SIGDN_FILESYSPATH, &SelectedValue))) {
                        BOOL Error;

                        /* TODO: I guess conversions from/to utf8 has to be done when syncing edits with DSN */
                        WideCharToMultiByte(GetACP(), 0, SelectedValue, -1, Path, sizeof(Path), NULL, &Error);
                        Edit_SetText(BoundEditWnd, Path);
                        CoTaskMemFree(SelectedValue);

                        return TRUE;
                    }
                    SelectedItem->lpVtbl->Release(SelectedItem);
                }
            }
            PathDialog->lpVtbl->Release(PathDialog);
        }
        CoUninitialize();
    }

    return FALSE;
}


INT_PTR CALLBACK DialogDSNProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    INT_PTR res;

    switch (uMsg) {
        case WM_CTLCOLORDLG:
            if (!hbrBg)
                hbrBg = CreateSolidBrush(RGB(255, 255, 255));
            return (INT_PTR) hbrBg;
            break;
        case WM_CTLCOLORSTATIC: {
            HDC hdcStatic = (HDC) wParam;
            SetTextColor(hdcStatic, RGB(0, 0, 0));
            SetBkMode(hdcStatic, TRANSPARENT);
            return (INT_PTR) hbrBg;
        }
        case WM_COMMAND:
            switch (LOWORD(wParam)) {
                case IDCANCEL:
                    SendMessage(GetParent(hDlg), WM_CLOSE, 0, 0);
                    notCanceled = FALSE;
                    return TRUE;
                case PB_PREV:
                    SetPage(hDlg, -1);
                    return TRUE;
                case PB_NEXT:
                    SetPage(hDlg, 1);
                    return TRUE;
                case PB_FINISH:
                    TAOS_Dsn *Dsn = (TAOS_Dsn *) GetWindowLongPtr(GetParent(hwndTab[0]), DWLP_USER);
                    GetDialogFields();
                    if (Dsn->isPrompt || SaveDSN(hDlg, Dsn)) {
                        SendMessage(hwndMain, WM_DESTROY, 0, 0);
                    }
                    return TRUE;
                case pbTestDSN:
                    TAOS_WIN_TestDsn(TRUE);
                    return TRUE;
                case cbDatabase:
                case rbTCP:
                case rbPipe:
                    if (HIWORD(wParam) == BN_CLICKED) {
                        SendMessage(GetDlgItem(hwndTab[CurrentPage], lblServerName), WM_SETTEXT, 0,
                                    (LOWORD(wParam) == rbTCP) ? (LPARAM) "Server name:" : (LPARAM) "Named pipe:");
                        ShowWindow(GetDlgItem(hwndTab[CurrentPage], lblPort),
                                   (LOWORD(wParam) == rbTCP) ? SW_SHOW : SW_HIDE);
                        ShowWindow(GetDlgItem(hwndTab[CurrentPage], txtPort),
                                   (LOWORD(wParam) == rbTCP) ? SW_SHOW : SW_HIDE);
                    }
                    return TRUE;

            }
            break;
    }
    return FALSE;
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CTLCOLORDLG:
            if (!hbrBg)
                hbrBg = CreateSolidBrush(RGB(255, 255, 255));
            return (INT_PTR) hbrBg;
            break;
        case WM_CLOSE:
            if (MessageBox(hDlg, TEXT("Close the program?"), TEXT("Close"),
                           MB_ICONQUESTION | MB_YESNO) == IDYES) {
                notCanceled = FALSE;
                DestroyWindow(hDlg);
            }
            return TRUE;

        case WM_DESTROY:
            DestroyWindow(hDlg);
            PostQuitMessage(0);
            return TRUE;
        case WM_INITDIALOG: {
            static int Dialogs[] = {Page_0, Page_1};
            int i;
            RECT rc;
            GetWindowRect(hDlg, &rc);

            for (i = 0; i <= LASTPAGE; ++i) {
                hwndTab[i] = CreateDialog(hInstance, MAKEINTRESOURCE(Dialogs[i]), hDlg, DialogDSNProc);
                SetWindowPos(hwndTab[i], 0, 120, 5, (rc.right - rc.left) - 120, (rc.bottom - rc.top),
                             SWP_NOZORDER | SWP_NOACTIVATE);
                ShowWindow(hwndTab[i], (i == 0) ? SW_SHOW : SW_HIDE);
            }
            i = 0;
            while (DsnMap[i].Key) {
                if (DsnMap[i].MaxLength)
                    TD_WIN_SET_MAXLEN(DsnMap[i].Page, DsnMap[i].Item, DsnMap[i].MaxLength);
                ++i;
            }

            return TRUE;
        }
    }
    return FALSE;
}


BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD ul_reason_for_call,
                      LPVOID lpReserved
) {
    hInstance = hModule;
    return TRUE;
}


void CenterWindow(HWND hwndWindow) {
    RECT rectWindow;

    int nWidth, nHeight, nScreenWidth, nScreenHeight;
    GetWindowRect(hwndWindow, &rectWindow);

    nWidth = rectWindow.right - rectWindow.left;
    nHeight = rectWindow.bottom - rectWindow.top;

    nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    nScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    MoveWindow(hwndWindow, (nScreenWidth - nWidth) / 2, (nScreenHeight - nHeight) / 2,
               nWidth, nHeight, TRUE);

}


BOOL DSNDialog(HWND hwndParent,
               WORD fRequest,
               LPCSTR lpszDriver,
               LPCSTR lpszAttributes,
               TAOS_Dsn *Dsn) {
    MSG msg;
    BOOL ret;
    SQLCHAR *DsnName = NULL;
    BOOL DsnExists = FALSE;
    SQLCHAR Delimiter = ';';

    if (Dsn->isPrompt < 0 || Dsn->isPrompt > ODBC_PROMPT_REQUIRED) {
        Dsn->isPrompt = ODBC_CONFIG;
    }

    if (lpszAttributes) {
        Delimiter = '\0';
        DsnName = strchr((SQLCHAR *) lpszAttributes, '=');
    }

    if (DsnName) {
        ++DsnName;
        /* In case of prompting we are supposed to show dialog even DSN name is incorrect */
        if (!Dsn->isPrompt && !SQLValidDSN(DsnName)) {
            if (hwndParent) {
                MessageBox(hwndParent, "Validation of data source name failed", "Error", MB_ICONERROR | MB_OK);
            }
            return FALSE;
        }
    }

    if (!DsnName && Dsn && Dsn->DSNName) {
        DsnName = Dsn->DSNName;
    } else if (DsnName && Dsn) {
        TAOS_RESET(Dsn->DSNName, DsnName);
    }

    /* Even if DsnName invalid(in case of prompt) - we should not have problem */
    DsnExists = TAOS_DSN_Exists(DsnName);

    SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &Environment);
    SQLSetEnvAttr(Environment, SQL_ATTR_ODBC_VERSION, (SQLPOINTER) SQL_OV_ODBC3, 0);

    switch (fRequest) {
        case ODBC_ADD_DSN:
            if (DsnExists) {
                if (hwndParent) {
                    if (MessageBox(hwndParent, "Data source name already exists, do you want to replace it?",
                                   "Question", MB_ICONQUESTION | MB_YESNO) != IDYES) {
                        return FALSE;
                    }
                }
            }

            TAOS_ParseConnString(Dsn, (SQLCHAR *) lpszAttributes, SQL_NTS, Delimiter);

            /* Need to set driver after connstring parsing, and before saving */
            if (lpszDriver) {
                Dsn->Driver = _strdup(lpszDriver);
            }

            /* If we don't have parent window - we are not supposed to show the dialog, but only
               perform operation, if sufficient data has been provided */
            if (hwndParent == NULL) {
                if (SQL_SUCCEEDED(TestDSN(Dsn, NULL, NULL))) {
                    return TAOS_SaveDSN(Dsn);
                } else {
                    return FALSE;
                }
            }

            break;

        case ODBC_CONFIG_DSN:

            /* i.e. not a prompt */
            if (Dsn->isPrompt == ODBC_CONFIG && Dsn->SaveFile == NULL) {
                if (!DsnExists) {
                    if (hwndParent != NULL) {
                        MessageBox(0, "Data source name not found", "Error", MB_ICONERROR | MB_OK);
                    }
                    return FALSE;
                } else if (!TAOS_ReadConnString(Dsn, (SQLCHAR *) lpszAttributes, SQL_NTS, Delimiter)) {
                    SQLPostInstallerError(ODBC_ERROR_INVALID_DSN, Dsn->ErrorMsg);
                    return FALSE;
                }

                if (hwndParent == NULL) {
                    if (SQL_SUCCEEDED(TestDSN(Dsn, NULL, NULL))) {
                        return TAOS_SaveDSN(Dsn);
                    } else {
                        return FALSE;
                    }
                }
            }
    }

    InitCommonControls();

    EffectiveDisabledPages = DisabledPages[Dsn->isPrompt];
    EffectiveDisabledControls = DisabledControls[Dsn->isPrompt];

    if (fRequest == ODBC_ADD_DSN && Dsn->isPrompt == ODBC_CONFIG && Dsn->DSNName != NULL) {
        EffectiveDisabledControls = PromptDisabledControls;
    }

    notCanceled = TRUE;
    hwndMain = CreateDialogParam(hInstance, MAKEINTRESOURCE(IDD_DIALOG1), 0, DialogProc, 0);
    SetWindowLongPtr(hwndMain, DWLP_USER, (LONG_PTR) Dsn);

    /* Setting first not disabled page */
    CurrentPage = -1;
    SetPage(hwndMain, 1);

    SetDialogFields();
    CenterWindow(hwndMain);
    ShowWindow(hwndMain, SW_SHOW);

    while ((ret = GetMessage(&msg, 0, 0, 0)) != 0) {
        if (ret == -1)
            break;

        if (!IsDialogMessage(hwndTab[CurrentPage], &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    SQLFreeHandle(SQL_HANDLE_ENV, Environment);

    if (notCanceled && fRequest == ODBC_CONFIG_DSN && DsnName != NULL && strcmp(DsnName, Dsn->DSNName) != 0) {
        SQLRemoveDSNFromIni(DsnName);
    }

    return notCanceled;
}

BOOL __stdcall ConfigDSNCmd(const SQLCHAR *connstr) {
    printf("ConfigDSNCmd, connstr=%s", connstr);
    TAOS_Dsn Dsn;
    memset(&Dsn, 0, sizeof(TAOS_Dsn));
    TAOS_ParseConnString(&Dsn, connstr, strlen(connstr), ";");
    DsnApplyDefaults(&Dsn);
    SQLCHAR *cfgStr = malloc(CONNSTR_BUFFER_SIZE * sizeof(char));
    memset(cfgStr, 0, CONNSTR_BUFFER_SIZE * sizeof(char));
    printf("DSN DSNName=%s, ServerName=%s, Driver=%s \n", Dsn.DSNName, Dsn.ServerName, Dsn.Driver);
    if (TAOS_SaveDSN(&Dsn)) {
        TAOS_DsnToString(&Dsn, cfgStr, CONNSTR_BUFFER_SIZE);
        printf("DSN %s saved %s\n", Dsn.DSNName, cfgStr);
        return TRUE;
    } else {
        printf("DSN %s can not be saved\n", Dsn.DSNName);
    }
    free(cfgStr);
}


BOOL INSTAPI ConfigDSN(
        HWND hwndParent,
        WORD fRequest,
        LPCSTR lpszDriver,
        LPCSTR lpszAttributes) {
    TAOS_Dsn Dsn;
    if (!hwndParent && fRequest != ODBC_REMOVE_DSN) {
        /*
        SQLPostInstallerError(ODBC_ERROR_INVALID_HWND, "Invalid Window Handle (NULL)");
        return FALSE; */
    }

    switch (fRequest) {
        case ODBC_ADD_DSN:
        case ODBC_CONFIG_DSN:
            memset(&Dsn, 0, sizeof(TAOS_Dsn));
            return DSNDialog(hwndParent, fRequest, lpszDriver, lpszAttributes, &Dsn);
        case ODBC_REMOVE_DSN: {
            SQLCHAR *Val = strchr((SQLCHAR *) lpszAttributes, '=');
            if (Val) {
                ++Val;
                return SQLRemoveDSNFromIni(Val);
            }
        }
        default:
            return FALSE;
    }
}

BOOL __stdcall DSNPrompt(HWND hwnd, TAOS_Dsn *Dsn) {
    return DSNDialog(hwnd, ODBC_CONFIG_DSN, NULL, NULL, Dsn);
}
