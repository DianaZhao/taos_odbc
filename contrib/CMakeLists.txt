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

# ================================================================================================
# Build
# ================================================================================================


# pthread
set(CMAKE_BUILD_TYPE debug)
add_definitions(-DPTW32_STATIC_LIB)
add_subdirectory(pthread EXCLUDE_FROM_ALL)
set_target_properties(libpthreadVC3 PROPERTIES OUTPUT_NAME pthread)
add_library(pthread INTERFACE)
target_link_libraries(pthread INTERFACE libpthreadVC3)


# iconv
add_library(iconv STATIC iconv/win_iconv.c)