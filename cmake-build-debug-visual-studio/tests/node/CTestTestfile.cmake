# CMake generated Testfile for 
# Source directory: D:/workspaces/taos_odbc/tests/node
# Build directory: D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/node
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(node_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/node/node_test.js")
  set_tests_properties(node_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;31;add_test;D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(node_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/node/node_test.js")
  set_tests_properties(node_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;31;add_test;D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(node_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/node/node_test.js")
  set_tests_properties(node_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;31;add_test;D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(node_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/node/node_test.js")
  set_tests_properties(node_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;31;add_test;D:/workspaces/taos_odbc/tests/node/CMakeLists.txt;0;")
else()
  add_test(node_test NOT_AVAILABLE)
endif()
