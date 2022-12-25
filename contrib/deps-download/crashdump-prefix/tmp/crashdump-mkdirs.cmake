# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/crashdump"
  "D:/workspaces/taos_odbc/contrib/crashdump"
  "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix/src/crashdump-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix/src/crashdump-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix/src/crashdump-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/crashdump-prefix/src/crashdump-stamp${cfgdir}") # cfgdir has leading slash
endif()
