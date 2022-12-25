# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/lz4"
  "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/src/lz4-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/src/lz4-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/src/lz4-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/src/lz4-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/lz4-prefix/src/lz4-stamp${cfgdir}") # cfgdir has leading slash
endif()
