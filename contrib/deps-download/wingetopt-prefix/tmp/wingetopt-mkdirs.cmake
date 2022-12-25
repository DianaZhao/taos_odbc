# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/wingetopt"
  "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/src/wingetopt-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/src/wingetopt-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/src/wingetopt-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/src/wingetopt-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/wingetopt-prefix/src/wingetopt-stamp${cfgdir}") # cfgdir has leading slash
endif()
