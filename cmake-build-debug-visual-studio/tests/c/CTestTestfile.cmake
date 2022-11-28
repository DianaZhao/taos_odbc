# CMake generated Testfile for 
# Source directory: D:/workspaces/taos_odbc/tests/c
# Build directory: D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(conformance_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Debug/conformance_test.exe")
  set_tests_properties(conformance_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;34;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(conformance_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Release/conformance_test.exe")
  set_tests_properties(conformance_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;34;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(conformance_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/MinSizeRel/conformance_test.exe")
  set_tests_properties(conformance_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;34;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(conformance_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/RelWithDebInfo/conformance_test.exe")
  set_tests_properties(conformance_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;34;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
else()
  add_test(conformance_test NOT_AVAILABLE)
endif()
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(api_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Debug/api_test.exe")
  set_tests_properties(api_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;66;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(api_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Release/api_test.exe")
  set_tests_properties(api_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;66;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(api_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/MinSizeRel/api_test.exe")
  set_tests_properties(api_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;66;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(api_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/RelWithDebInfo/api_test.exe")
  set_tests_properties(api_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;66;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
else()
  add_test(api_test NOT_AVAILABLE)
endif()
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(edge_cases_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Debug/edge_cases_test.exe")
  set_tests_properties(edge_cases_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;80;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(edge_cases_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Release/edge_cases_test.exe")
  set_tests_properties(edge_cases_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;80;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(edge_cases_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/MinSizeRel/edge_cases_test.exe")
  set_tests_properties(edge_cases_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;80;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(edge_cases_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/RelWithDebInfo/edge_cases_test.exe")
  set_tests_properties(edge_cases_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;80;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
else()
  add_test(edge_cases_test NOT_AVAILABLE)
endif()
if(CTEST_CONFIGURATION_TYPE MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
  add_test(odbc_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Debug/odbc_test.exe")
  set_tests_properties(odbc_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;97;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
  add_test(odbc_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/Release/odbc_test.exe")
  set_tests_properties(odbc_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;97;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
  add_test(odbc_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/MinSizeRel/odbc_test.exe")
  set_tests_properties(odbc_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;97;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
elseif(CTEST_CONFIGURATION_TYPE MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
  add_test(odbc_test "D:/workspaces/taos_odbc/cmake-build-debug-visual-studio/tests/c/RelWithDebInfo/odbc_test.exe")
  set_tests_properties(odbc_test PROPERTIES  _BACKTRACE_TRIPLES "D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;97;add_test;D:/workspaces/taos_odbc/tests/c/CMakeLists.txt;0;")
else()
  add_test(odbc_test NOT_AVAILABLE)
endif()
