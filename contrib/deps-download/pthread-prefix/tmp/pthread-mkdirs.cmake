# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/pthread"
  "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/src/pthread-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/src/pthread-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/src/pthread-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/src/pthread-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/pthread-prefix/src/pthread-stamp${cfgdir}") # cfgdir has leading slash
endif()
