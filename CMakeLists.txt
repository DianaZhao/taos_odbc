﻿# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required(VERSION 3.0)

project("taos_odbc_driver" C)

IF (MSVC)
    SET(CONFIG_TYPES "DEBUG" "RELEASE" "RELWITHDEBINFO" "MINSIZEREL")
    FOREACH (BUILD_TYPE ${CONFIG_TYPES})
        FOREACH (COMPILER CXX C)
            SET(COMPILER_FLAGS "${CMAKE_${COMPILER}_FLAGS_${BUILD_TYPE}}")
            IF (NOT COMPILER_FLAGS STREQUAL "")
                STRING(REPLACE "/MD" "/MT" COMPILER_FLAGS ${COMPILER_FLAGS})
                IF (BUILD_TYPE STREQUAL "Debug")
                    SET(COMPILER_FLAGS "${COMPILER_FLAGS} /RTC1 /RTCc")
                    STRING(REPLACE "/Zi" "/ZI" COMPILER_FLAGS ${COMPILER_FLAGS})
                ENDIF ()
                MESSAGE(STATUS "CMAKE_${COMPILER}_FLAGS_${BUILD_TYPE}= ${COMPILER_FLAGS}")
                SET(CMAKE_${COMPILER}_FLAGS_${BUILD_TYPE} ${COMPILER_FLAGS} CACHE
                        STRING "overwritten by taos-odbc" FORCE)
            ENDIF ()
        ENDFOREACH ()
    ENDFOREACH ()
    IF (NOT (MSVC_VERSION LESS 1900))
        MESSAGE(STATUS "Configuring to link connector against legacy_stdio_definitions")
        SET(LEGACY_STDIO legacy_stdio_definitions)
        SET(PLATFORM_DEPENDENCIES ${PLATFORM_DEPENDENCIES} ${LEGACY_STDIO})
    ENDIF ()
ENDIF ()
SET(ODBC_LIBS odbc32)
SET(ODBC_INSTLIBS odbccp32)
set(TD_LIB_DIR "C:\\TDengine\\driver")
include_directories("C:\\TDengine\\include")
set(TD_SUPPORT_DIR "${PROJECT_SOURCE_DIR}/cmake")
set(TD_CONTRIB_DIR "${PROJECT_SOURCE_DIR}/contrib")
add_subdirectory(contrib)
INCLUDE_DIRECTORIES(${TD_CONTRIB_DIR}/pthread)
INCLUDE_DIRECTORIES(${TD_CONTRIB_DIR}/iconv)

# Include sub-projects.
add_subdirectory("taos_odbc_driver")
add_subdirectory("test")
add_subdirectory("wininstall")
