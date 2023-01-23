// taos_odbc_driver.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#define TAOS_ODBC_API __declspec(dllexport)

#include "taos_odbc_driver/inc/env.h"
#include "taos_odbc_driver/inc/conn.h"
#include "taos_odbc_driver/inc/stmt.h"
#include "taos_odbc_driver/inc/desc.h"

// TODO: Reference additional headers your program requires here.

char * __stdcall test_dll(char *msg);