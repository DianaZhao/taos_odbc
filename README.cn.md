# TDengine 3.0 TAOS ODBC 驱动 #
[English](README.md) | 简体中文

- **开发中的TDengine 3.0 TAOS ODBC驱动**
- **目前驱动中已经导出的SQLxxx函数如下**:
```
SQLAllocHandle
SQLBindCol
SQLBindParameter
SQLColAttribute
SQLConnect
SQLDescribeCol
SQLDescribeParam
SQLDisconnect
SQLDriverConnect
SQLEndTran
SQLExecDirect
SQLExecute
SQLFetch
SQLFetchScroll
SQLFreeHandle
SQLFreeStmt
SQLGetData
SQLGetDiagField
SQLGetDiagRec
SQLGetInfo
SQLNumParams
SQLNumResultCols
SQLPrepare
SQLRowCount
SQLSetConnectAttr
SQLSetEnvAttr
SQLSetStmtAttr
SQLTables (暂时使用post-filter来补救一下，等待taosc有新的实现后再移除)
```
- **ODBC应用程序将可以利用该驱动实现对TDengine时序数据库的操作，但是目前该驱动只有Windows平台的实现**
- **相信，任何具有ODBC-绑定/插件的编程语言、框架，利用该驱动，也可实现对TDengine时序数据库的操作**
- **持续开发中**...

### (目前)支持的平台
- Windows

### 所需的依赖
- iconv, 应该不需要单独安装了，基本上libc都已经内建了

### 安装TDengine 3.0
- 请参考TDengine官方说明，https://tdengine.com

### 安装必需的依赖项，以Ubuntu 20.04为例
```
sudo apt install flex bison unixodbc unixodbc-dev && echo -=Done=-
```

### 编译及安装, 以Ubuntu 20.04为例
```
rm -rf debug && cmake -B debug -DCMAKE_BUILD_TYPE=Debug && cmake --build debug && sudo cmake --install debug && echo -=Done=-
```

### 测试
```
pushd debug >/dev/null && TAOS_TEST_CASES=$(pwd)/../tests/taos/taos_test.cases ODBC_TEST_CASES=$(pwd)/../tests/c/odbc_test.cases ctest --output-on-failure && echo -=Done=-; popd >/dev/null
```

### 带上TAOS_ODBC_DEBUG环境变量进行测试
当测试程序出现失败的时候，你可能期望看到更多的调试信息，那么你可以这样
```
pushd debug >/dev/null && TAOS_TEST_CASES=$(pwd)/../tests/taos/taos_test.cases ODBC_TEST_CASES=$(pwd)/../tests/c/odbc_test.cases TAOS_ODBC_DEBUG= ctest --output-on-failure && echo -=Done=-; popd >/dev/null
```

### 让每天的生活简单一点
```
export TAOS_TEST_CASES=$(pwd)/tests/taos/taos_test.cases
export ODBC_TEST_CASES=$(pwd)/tests/c/odbc_test.cases
export TAOS_ODBC_DEBUG=
```
或者，你也可以这样
```
pushd debug >/dev/null && ctest --output-on-failure && echo -=Done=-; popd >/dev/null
```

### 小提示
- `cmake --help` or `man cmake`
- `ctest --help` or `man ctest`
- `valgrind --help` or `man valgrind`

### 源代码目录结构
```
<root>
├── cmake
├── common
├── sh
├── taos_odbc_driver
│   ├── core
│   ├── inc
│   └── dsn
├── templates
├── tests
│   ├── c
│   ├── cpp
│   ├── node
│   ├── rust
│   │   └── main
│   │       └── src
│   └── taos
├── tools
└── valgrind
```

## TDengine 引用及出处
- https://tdengine.com
- https://github.com/taosdata/TDengine

## ODBC 引用及出处
- https://learn.microsoft.com/en-us/sql/odbc/reference/introduction-to-odbc?view=sql-server-ver16
- https://learn.microsoft.com/en-us/sql/odbc/reference/syntax/odbc-api-reference?view=sql-server-ver16

