# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/cJson"
  "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/src/cjson-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/src/cjson-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/src/cjson-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/src/cjson-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/cjson-prefix/src/cjson-stamp${cfgdir}") # cfgdir has leading slash
endif()
