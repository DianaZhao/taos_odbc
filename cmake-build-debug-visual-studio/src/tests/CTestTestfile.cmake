# CMake generated Testfile for 
# Source directory: D:/workspaces/taos_odbc/src/tests
# Build directory: D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/src/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(basic "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/src/tests/Debug/basic.exe")
  set_tests_properties(basic PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(basic "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/src/tests/Release/basic.exe")
  set_tests_properties(basic PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(basic "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/src/tests/MinSizeRel/basic.exe")
  set_tests_properties(basic PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(basic "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/src/tests/RelWithDebInfo/basic.exe")
  set_tests_properties(basic PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;33;add_test;D:/workspaces/taos_odbc/src/tests/CMakeLists.txt;0;")
else()
  add_test(basic NOT_AVAILABLE)
endif()
