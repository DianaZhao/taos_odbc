# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/libuv"
  "D:/workspaces/taos_odbc/contrib/libuv"
  "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix/src/libuv-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix/src/libuv-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix/src/libuv-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/libuv-prefix/src/libuv-stamp${cfgdir}") # cfgdir has leading slash
endif()
