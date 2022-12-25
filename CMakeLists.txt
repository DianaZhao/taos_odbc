###############################################################################
# MIT License
#
# Copyright (c) 2022 Diana Zhao <dianaz@outlook.com>
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

cmake_minimum_required(VERSION 3.8)

project(taos_odbc)

set(TODBC_VERSION 0.1)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 11)
if (NOT DEFINED TD_SOURCE_DIR)
    set( TD_SOURCE_DIR ${PROJECT_SOURCE_DIR} )
endif()

set(TD_WINDOWS TRUE)
set(TD_SUPPORT_DIR "${TD_SOURCE_DIR}/cmake")
set(TD_CONTRIB_DIR "${TD_SOURCE_DIR}/contrib")
MESSAGE("C Compiler ID: ${CMAKE_C_COMPILER_ID}")

add_subdirectory(contrib)

include(CTest)
include(cmake/macros.cmake)
IF (TD_WINDOWS)
    MESSAGE("${Yellow} set compiler flag for Windows! ${ColourReset}")
    SET(COMMON_FLAGS "/w /D_WIN32 /DWIN32 /Zi /MTd")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /MANIFEST:NO")
    # IF (MSVC AND (MSVC_VERSION GREATER_EQUAL 1900))
    #  SET(COMMON_FLAGS "${COMMON_FLAGS} /Wv:18")
    # ENDIF ()
    IF (CMAKE_DEPFILE_FLAGS_C)
        SET(CMAKE_DEPFILE_FLAGS_C "")
    ENDIF ()
    IF (CMAKE_DEPFILE_FLAGS_CXX)
        SET(CMAKE_DEPFILE_FLAGS_CXX "")
    ENDIF ()
    IF (CMAKE_C_FLAGS_DEBUG)
        SET(CMAKE_C_FLAGS_DEBUG "" CACHE STRING "" FORCE)
    ENDIF ()
    IF (CMAKE_CXX_FLAGS_DEBUG)
        SET(CMAKE_CXX_FLAGS_DEBUG "" CACHE STRING "" FORCE)
    ENDIF ()

    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${COMMON_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${COMMON_FLAGS}")

ENDIF ()

check_requirements()

include_directories(inc)
include_directories("C:\\TDengine\\include")
INCLUDE_DIRECTORIES(${TD_CONTRIB_DIR}/pthread)
INCLUDE_DIRECTORIES(${TD_CONTRIB_DIR}/iconv)

add_subdirectory(common)
add_subdirectory(src)
add_subdirectory(templates)
add_subdirectory(tests)

