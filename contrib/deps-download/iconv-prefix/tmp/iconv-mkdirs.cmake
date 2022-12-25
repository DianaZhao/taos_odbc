# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/iconv"
  "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/src/iconv-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/src/iconv-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/src/iconv-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/src/iconv-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/iconv-prefix/src/iconv-stamp${cfgdir}") # cfgdir has leading slash
endif()
