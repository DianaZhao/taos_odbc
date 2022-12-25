# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/zlib"
  "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/src/zlib-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/src/zlib-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/src/zlib-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/src/zlib-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/zlib-prefix/src/zlib-stamp${cfgdir}") # cfgdir has leading slash
endif()
