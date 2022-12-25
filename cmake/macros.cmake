###############################################################################
# MIT License
#
# Copyright (c) 2022 dianazhao <dianaz@outlook.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
###############################################################################

macro(check_requirements)
  if(NOT WIN32)
    string(ASCII 27 Esc)
    set(ColorReset  "${Esc}[m")
    set(ColorBold   "${Esc}[1m")
    set(Red         "${Esc}[31m")
    set(Green       "${Esc}[32m")
    set(Yellow      "${Esc}[33m")
    set(Blue        "${Esc}[34m")
    set(Magenta     "${Esc}[35m")
    set(Cyan        "${Esc}[36m")
    set(White       "${Esc}[37m")
    set(BoldRed     "${Esc}[1;31m")
    set(BoldGreen   "${Esc}[1;32m")
    set(BoldYellow  "${Esc}[1;33m")
    set(BoldBlue    "${Esc}[1;34m")
    set(BoldMagenta "${Esc}[1;35m")
    set(BoldCyan    "${Esc}[1;36m")
    set(BoldWhite   "${Esc}[1;37m")
  endif()

  include(CheckSymbolExists)

  ## prepare `cjson`
  include(ExternalProject)
  ExternalProject_Add(ex_cjson
      GIT_REPOSITORY https://github.com/taosdata-contrib/cJSON.git
      GIT_TAG v1.7.15
      GIT_SHALLOW TRUE
      PREFIX "${TAOS_ODBC_LOCAL_REPO}/cjson"
      CMAKE_ARGS "-DCMAKE_INSTALL_PREFIX=${CJSON_INSTALL_PATH}"
      )

#  find_library(TAOS NAMES taos)
#  message(WARNING "${TAOS}")
#  if(${TAOS} STREQUAL TAOS-NOTFOUND)
#    message(FATAL_ERROR "${Red}`taos.dll` is required but not found, you may refer to https://github.com/taosdata/TDengine${ColorReset}")
#  endif()
#  set(CMAKE_REQUIRED_LIBRARIES taos)
#  check_symbol_exists(taos_query "taos.h" HAVE_TAOS)
#  if(NOT HAVE_TAOS)
#    message(FATAL_ERROR "${Red}`taos.h` is required but not found, you may refer to https://github.com/taosdata/TDengine${ColorReset}")
#  endif()

  ## check `flex`
  find_package(FLEX)
  if(NOT FLEX_FOUND)
    message(FATAL_ERROR "${Red}you need to install `flex` first${ColorReset}")
  endif()
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 5.0.0)
    message(FATAL_ERROR "${Red}gcc 4.8.0 will complain too much about flex-generated code, we just bypass building ODBC driver in such case${ColorReset}")
  endif()

  ## check `bison`
  find_package(BISON)
  if(NOT BISON_FOUND)
    message(FATAL_ERROR "${Red}you need to install `bison` first${ColorReset}")
  endif()
  if(CMAKE_C_COMPILER_ID STREQUAL "GNU" AND CMAKE_C_COMPILER_VERSION VERSION_LESS 5.0.0)
    message(FATAL_ERROR "${Red}gcc 4.8.0 will complain too much about bison-generated code, we just bypass building ODBC driver in such case${ColorReset}")
  endif()

  find_package(ODBC)
  if (NOT ODBC_FOUND)
    message(WARNING "you need to install ODBC first")
  else ()
    message(STATUS "ODBC_INCLUDE_DIRS: ${ODBC_INCLUDE_DIRS}")
    message(STATUS "ODBC_LIBRARIES: ${ODBC_LIBRARIES}")
    message(STATUS "ODBC_CONFIG: ${ODBC_CONFIG}")
  endif ()
  find_package(FLEX)
  if(NOT FLEX_FOUND)
    message(WARNING "you need to install flex first\n"
            "you may go to: https://github.com/lexxmark/winflexbison\n"
            "or download from: https://github.com/lexxmark/winflexbison/releases")
  endif()


  ## check `cargo`
  find_program(CARGO NAMES cargo)
  if(NOT CARGO)
    message(STATUS "${Yellow}`cargo` not found, thus cargo-related-test-cases would be eliminated, you may refer to https://rust-lang.org/${ColorReset}")
  else()
    set(HAVE_CARGO ON)
    execute_process(COMMAND cargo --version ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE CARGO_VERSION)
    message(STATUS "${Green}`cargo` found -- ${CARGO_VERSION}, please be noted, cargo v1.63 and above are expected compatible${ColorReset}")
  endif()

  ## check `mysql`
  if (ENABLE_MYSQL_TEST)
    find_program(HAVE_MYSQL NAMES mysql)
    if(NOT HAVE_MYSQL)
      message(FATAL_ERROR "${Yellow}`mysql-related-test-cases` is requested, but `mysql` is not found, you may refer to https://www.mysql.com/${ColorReset}")
    else()
      execute_process(COMMAND mysql --version ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE MYSQL_VERSION)
      message(STATUS "${Green}`mysql` found -- ${MYSQL_VERSION}, please be noted, mysql v8.0 and above are expected compatible${ColorReset}")
    endif()
  endif()

  ## check `sqlite3`
  if(ENABLE_SQLITE3_TEST)
    find_program(HAVE_SQLITE3 NAMES sqlite3)
    if(NOT HAVE_SQLITE3)
      message(FATAL_ERROR "${Yellow}`sqlite3-related-test-cases` is requested, but `sqlite3` is not found, you may refer to https://www.sqlite.org/${ColorReset}")
    else()
      execute_process(COMMAND sqlite3 --version ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE OUTPUT_VARIABLE SQLITE3_VERSION)
      message(STATUS "${Green}`sqlite3` found -- ${SQLITE3_VERSION}, please be noted, sqlite3 v3.31 and above are expected compatible${ColorReset}")
    endif()
  endif()

endmacro()

macro(parser_gen _name)
    set(_parser          ${_name}_parser)
    set(_scanner         ${_name}_scanner)
    set(_src_path        ${CMAKE_CURRENT_SOURCE_DIR})
    set(_dst_path        ${CMAKE_CURRENT_BINARY_DIR})
    set(_src_name        ${_src_path}/${_name})
    set(_dst_name        ${_dst_path}/${_name})
    set(_y               ${_src_name}.y)
    set(_l               ${_src_name}.l)
    set(_tab_c           ${_src_name}_tab.c)
    set(_dst_tab_c       ${_dst_name}.tab.c)
    set(_dst_lex_c       ${_dst_name}.lex.c)
    set(_dst_lex_h       ${_dst_name}.lex.h)

    BISON_TARGET(${_parser} ${_y} ${_dst_tab_c}
        COMPILE_FLAGS "--warnings=error -Dapi.prefix={${_name}_yy}")
    FLEX_TARGET(${_scanner} ${_l} ${_dst_lex_c}
        COMPILE_FLAGS "--header-file=${_dst_lex_h} --prefix=${_name}_yy")
    ADD_FLEX_BISON_DEPENDENCY(${_scanner} ${_parser})

    set(_bison_outputs ${BISON_${_parser}_OUTPUTS})
    set(_flex_outputs ${FLEX_${_scanner}_OUTPUTS})
    set(_outputs ${_bison_outputs} ${_flex_outputs})

    set_source_files_properties(${_tab_c}
            PROPERTY COMPILE_FLAGS "-I${_dst_path}")
    set_source_files_properties(${_tab_c}
            PROPERTY OBJECT_DEPENDS "${_outputs}")

    unset(_bison_outputs)
    unset(_flex_outputs)
    unset(_outputs)

    unset(_parser)
    unset(_scanner)
    unset(_src_path)
    unset(_dst_path)
    unset(_src_name)
    unset(_dst_name)
    unset(_y)
    unset(_l)
    unset(_tab_c)
    unset(_dst_tab_c)
    unset(_dst_lex_c)
    unset(_dst_lex_h)
endmacro()

