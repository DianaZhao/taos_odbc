# ================================================================================================
# Download
# ================================================================================================
function(cat IN_FILE OUT_FILE)
    file(READ ${IN_FILE} CONTENTS)
    file(APPEND ${OUT_FILE} "${CONTENTS}")
endfunction(cat IN_FILE OUT_FILE)

set(CONTRIB_TMP_FILE "${CMAKE_BINARY_DIR}/deps_tmp_CMakeLists.txt.in")
configure_file("${TD_SUPPORT_DIR}/deps_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})


# pthread
cat("${TD_SUPPORT_DIR}/pthread_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# iconv
cat("${TD_SUPPORT_DIR}/iconv_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# msvc regex
cat("${TD_SUPPORT_DIR}/msvcregex_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# wcwidth
cat("${TD_SUPPORT_DIR}/wcwidth_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# wingetopt
cat("${TD_SUPPORT_DIR}/wingetopt_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# lz4
cat("${TD_SUPPORT_DIR}/lz4_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# zlib
cat("${TD_SUPPORT_DIR}/zlib_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# cJson
cat("${TD_SUPPORT_DIR}/cjson_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})


#libuv
cat("${TD_SUPPORT_DIR}/libuv_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})


# crashdump
cat("${TD_SUPPORT_DIR}/crashdump_CMakeLists.txt.in" ${CONTRIB_TMP_FILE})

# download dependencies
configure_file(${CONTRIB_TMP_FILE} "${TD_CONTRIB_DIR}/deps-download/CMakeLists.txt")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${TD_CONTRIB_DIR}/deps-download")
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${TD_CONTRIB_DIR}/deps-download")

# ================================================================================================
# Build
# ================================================================================================

# cJson
# see https://stackoverflow.com/questions/37582508/silence-cmp0048-warnings-in-vendored-projects
set(CMAKE_PROJECT_INCLUDE_BEFORE "${TD_SUPPORT_DIR}/EnableCMP0048.txt.in")
option(ENABLE_CJSON_TEST "Enable building cJSON test" OFF)
option(CJSON_OVERRIDE_BUILD_SHARED_LIBS "Override BUILD_SHARED_LIBS with CJSON_BUILD_SHARED_LIBS" ON)
option(CJSON_BUILD_SHARED_LIBS "Overrides BUILD_SHARED_LIBS if CJSON_OVERRIDE_BUILD_SHARED_LIBS is enabled" OFF)
add_subdirectory(cJson EXCLUDE_FROM_ALL)
target_include_directories(
    cjson
    # see https://stackoverflow.com/questions/25676277/cmake-target-include-directories-prints-an-error-when-i-try-to-add-the-source
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/cJson>
)
unset(CMAKE_PROJECT_INCLUDE_BEFORE)

# lz4
add_subdirectory(lz4/build/cmake EXCLUDE_FROM_ALL)
target_include_directories(
    lz4_static
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib
)

# zlib
set(CMAKE_PROJECT_INCLUDE_BEFORE "${TD_SUPPORT_DIR}/EnableCMP0048.txt.in")
add_subdirectory(zlib EXCLUDE_FROM_ALL)
target_include_directories(
    zlibstatic
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/zlib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/zlib
)
target_include_directories(
    zlib
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}/zlib
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/zlib
)
unset(CMAKE_PROJECT_INCLUDE_BEFORE)

# pthread
set(CMAKE_BUILD_TYPE debug)
add_definitions(-DPTW32_STATIC_LIB)
add_subdirectory(pthread EXCLUDE_FROM_ALL)
set_target_properties(libpthreadVC3 PROPERTIES OUTPUT_NAME pthread)
add_library(pthread INTERFACE)
target_link_libraries(pthread INTERFACE libpthreadVC3)

# crashdump
add_executable(dumper "crashdump/dumper/dumper.c")
target_link_libraries(dumper User32.lib dbghelp.lib)
file(READ "crashdump/crasher/crasher.c" CRASHDUMP_CONTENT)
string(REPLACE "main(" "main_crashdump(" CRASHDUMP_CONTENT "${CRASHDUMP_CONTENT}")
file(WRITE "crashdump/crasher/crasher.c" "${CRASHDUMP_CONTENT}")
add_library(crashdump STATIC "crashdump/crasher/crasher.c")

# iconv
add_library(iconv STATIC iconv/win_iconv.c)

# wingetopt
add_subdirectory(wingetopt EXCLUDE_FROM_ALL)

# msvcregex
add_library(msvcregex STATIC "")
target_sources(msvcregex
    PRIVATE "msvcregex/regex.c"
)
target_include_directories(msvcregex
    PRIVATE "msvcregex"
)
target_link_libraries(msvcregex
    INTERFACE Shell32
)
SET_TARGET_PROPERTIES(msvcregex PROPERTIES OUTPUT_NAME msvcregex)

# wcwidth
add_library(wcwidth STATIC "")
target_sources(wcwidth
    PRIVATE "wcwidth/wcwidth.c"
)
target_include_directories(wcwidth
    PRIVATE "wcwidth"
)
SET_TARGET_PROPERTIES(wcwidth PROPERTIES OUTPUT_NAME wcwidth)

# LIBUV
if (TD_WINDOWS)
    # There is no GetHostNameW function on win7.
    file(READ "libuv/src/win/util.c" LIBUV_WIN_UTIL_CONTENT)
    string(REPLACE "if (GetHostNameW(buf, UV_MAXHOSTNAMESIZE" "DWORD  nSize = UV_MAXHOSTNAMESIZE;\n  if (GetComputerNameW(buf, &nSize" LIBUV_WIN_UTIL_CONTENT "${LIBUV_WIN_UTIL_CONTENT}")
    file(WRITE "libuv/src/win/util.c" "${LIBUV_WIN_UTIL_CONTENT}")
endif ()
add_subdirectory(libuv EXCLUDE_FROM_ALL)

