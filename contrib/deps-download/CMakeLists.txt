cmake_minimum_required(VERSION 3.8)

project(deps-download NONE)

include(ExternalProject)

# pthread
ExternalProject_Add(pthread
        GIT_REPOSITORY https://github.com/GerHobbelt/pthread-win32
        GIT_TAG v3.0.3.1
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/pthread"
        BINARY_DIR ""
        #BUILD_IN_SOURCE TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)

# iconv
ExternalProject_Add(iconv
        GIT_REPOSITORY https://github.com/win-iconv/win-iconv.git
        GIT_TAG v0.0.8
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/iconv"
        BINARY_DIR ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        )
# msvcregex
ExternalProject_Add(msvcregex
        GIT_REPOSITORY https://gitee.com/l0km/libgnurx-msvc.git
        GIT_TAG master
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/msvcregex"
        BINARY_DIR ""
        #BUILD_IN_SOURCE TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)
# wcwidth
ExternalProject_Add(wcwidth
        GIT_REPOSITORY https://github.com/fumiyas/wcwidth-cjk.git
        GIT_TAG master
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/wcwidth"
        BINARY_DIR ""
        #BUILD_IN_SOURCE TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)
# wingetopt
ExternalProject_Add(wingetopt
        GIT_REPOSITORY https://github.com/alex85k/wingetopt.git
        GIT_TAG master
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/wingetopt"
        BINARY_DIR ""
        #BUILD_IN_SOURCE TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)
# lz4
ExternalProject_Add(lz4
        GIT_REPOSITORY https://github.com/taosdata-contrib/lz4.git
        GIT_TAG v1.9.3
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/lz4"
        BINARY_DIR ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        )
# zlib
ExternalProject_Add(zlib
        GIT_REPOSITORY https://github.com/taosdata-contrib/zlib.git
        GIT_TAG v1.2.11
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/zlib"
        BINARY_DIR ""
        #BUILD_IN_SOURCE TRUE
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
)
# cjson
ExternalProject_Add(cjson
        GIT_REPOSITORY https://github.com/taosdata-contrib/cJSON.git
        GIT_TAG v1.7.15
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/cJson"
        BINARY_DIR ""
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ""
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        )
# libuv
ExternalProject_Add(libuv
        GIT_REPOSITORY https://github.com/libuv/libuv.git
        GIT_TAG v1.44.2
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/libuv"
        BINARY_DIR "D:/workspaces/taos_odbc/contrib/libuv"
        CONFIGURE_COMMAND "" 
        BUILD_COMMAND ""  
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        )

# crashdump
ExternalProject_Add(crashdump
        GIT_REPOSITORY https://github.com/Arnavion/crashdump.git
        GIT_TAG master
        SOURCE_DIR "D:/workspaces/taos_odbc/contrib/crashdump"
        BINARY_DIR "D:/workspaces/taos_odbc/contrib/crashdump"
        CONFIGURE_COMMAND "" 
        BUILD_COMMAND ""  
        INSTALL_COMMAND ""
        TEST_COMMAND ""
        )
