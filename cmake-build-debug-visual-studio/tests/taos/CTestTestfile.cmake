# CMake generated Testfile for 
# Source directory: D:/workspaces/taos_odbc/tests/taos
# Build directory: D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/taos
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(taos_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/taos/Debug/taos_test.exe")
  set_tests_properties(taos_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(taos_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/taos/Release/taos_test.exe")
  set_tests_properties(taos_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(taos_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/taos/MinSizeRel/taos_test.exe")
  set_tests_properties(taos_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(taos_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/taos/RelWithDebInfo/taos_test.exe")
  set_tests_properties(taos_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/tests/taos/CMakeLists.txt;0;")
else()
  add_test(taos_test NOT_AVAILABLE)
endif()
