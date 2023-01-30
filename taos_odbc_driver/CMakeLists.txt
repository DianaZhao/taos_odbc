# CMakeList.txt : CMake project for taos_odbc_driver, include source and define
# project specific logic here.
#

INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/inc)
# Add source to this project's executable.
#link_directories(${TD_LIB_DIR})
add_library(taos_odbc_driver SHARED
        "taos_odbc_driver.def"
        "taos_odbc_driver.c"
        "core/env.c"
        "core/conn.c"
        "core/stmt.c"
        "core/enums.c"
        "core/errs.c"
        "core/desc.c"
        "core/utils.c"
        "core/strptime.c"
        "dsn/td_dsn.c")
target_link_libraries(taos_odbc_driver
        pthread
        iconv
        ${TD_LIB_DIR}/taos.lib
        ${PLATFORM_DEPENDENCIES}
        ${ODBC_LIBS}
        ${ODBC_INSTLIBS}
        Shlwapi)

SET(DSN_DIALOG_FILES ${CMAKE_CURRENT_SOURCE_DIR}/dsn/odbc_dsn.c
        ${CMAKE_CURRENT_SOURCE_DIR}/dsn/odbc_dsn.rc
        ${CMAKE_CURRENT_SOURCE_DIR}/dsn/resource.h
        ${CMAKE_CURRENT_SOURCE_DIR}/dsn/td_dsn.c)
ADD_LIBRARY(taos_odbc_setup SHARED ${DSN_DIALOG_FILES} ${CMAKE_CURRENT_SOURCE_DIR}/dsn/taos_odbc_setup.def)
SET_TARGET_PROPERTIES(taos_odbc_setup PROPERTIES LANGUAGE C)
TARGET_LINK_LIBRARIES(taos_odbc_setup comctl32 ${LEGACY_STDIO} Shlwapi ${ODBC_LIBS} ${ODBC_INSTLIBS})


# TODO: Add tests and install targets if needed.
