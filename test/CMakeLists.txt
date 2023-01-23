SET(TD_ODBC_DIR ${PROJECT_SOURCE_DIR}/debug/taos_odbc_driver)
add_executable(taos_test taos_test.c)
target_link_libraries(taos_test ${TD_LIB_DIR}/taos.lib)
add_test(NAME taos_test
        COMMAND taos_test)
add_executable(odbc_test odbc_test.c)
#target_link_libraries(odbc_test ${TD_ODBC_DIR}/taos_odbc_driver.lib)
add_test(NAME odbc_test
        COMMAND odbc_test)

add_executable(dsn_test dsn_test.c)
add_test(NAME dsn_test
        COMMAND dsn_test)
