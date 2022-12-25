# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/workspaces/taos_odbc/contrib/wcwidth"
  "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/src/wcwidth-build"
  "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix"
  "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/tmp"
  "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/src/wcwidth-stamp"
  "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/src"
  "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/src/wcwidth-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/src/wcwidth-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/workspaces/taos_odbc/contrib/deps-download/wcwidth-prefix/src/wcwidth-stamp${cfgdir}") # cfgdir has leading slash
endif()
