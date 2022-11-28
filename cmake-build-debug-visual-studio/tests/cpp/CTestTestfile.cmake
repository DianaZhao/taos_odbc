# CMake generated Testfile for 
# Source directory: D:/workspaces/taos_odbc/tests/cpp
# Build directory: D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/cpp
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(SQLBindCol "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/cpp/Debug/SQLBindCol.exe")
  set_tests_properties(SQLBindCol PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;32;add_test;D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(SQLBindCol "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/cpp/Release/SQLBindCol.exe")
  set_tests_properties(SQLBindCol PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;32;add_test;D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(SQLBindCol "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/cpp/MinSizeRel/SQLBindCol.exe")
  set_tests_properties(SQLBindCol PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;32;add_test;D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(SQLBindCol "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/cpp/RelWithDebInfo/SQLBindCol.exe")
  set_tests_properties(SQLBindCol PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;32;add_test;D:/workspaces/taos_odbc/tests/cpp/CMakeLists.txt;0;")
else()
  add_test(SQLBindCol NOT_AVAILABLE)
endif()
