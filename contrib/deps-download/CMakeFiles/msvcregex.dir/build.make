﻿# CMAKE generated file: DO NOT EDIT!
# Generated by "NMake Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE
NULL=nul
!ENDIF
SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\App\CMake\bin\cmake.exe

# The command to remove a file.
RM = C:\App\CMake\bin\cmake.exe -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = D:\workspaces\taos_odbc\contrib\deps-download

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\workspaces\taos_odbc\contrib\deps-download

# Utility rule file for msvcregex.

# Include any custom commands dependencies for this target.
include CMakeFiles\msvcregex.dir\compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles\msvcregex.dir\progress.make

CMakeFiles\msvcregex: CMakeFiles\msvcregex-complete

CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-install
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-mkdir
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-download
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-update
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-patch
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-configure
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-build
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-install
CMakeFiles\msvcregex-complete: msvcregex-prefix\src\msvcregex-stamp\msvcregex-test
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'msvcregex'"
	C:\App\CMake\bin\cmake.exe -E make_directory D:/workspaces/taos_odbc/contrib/deps-download/CMakeFiles
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/CMakeFiles/msvcregex-complete
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-done

msvcregex-prefix\src\msvcregex-stamp\msvcregex-build: msvcregex-prefix\src\msvcregex-stamp\msvcregex-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "No build step for 'msvcregex'"
	cd D:\workspaces\taos_odbc\contrib\deps-download\msvcregex-prefix\src\msvcregex-build
	C:\App\CMake\bin\cmake.exe -E echo_append
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-build
	cd D:\workspaces\taos_odbc\contrib\deps-download

msvcregex-prefix\src\msvcregex-stamp\msvcregex-configure: msvcregex-prefix\tmp\msvcregex-cfgcmd.txt
msvcregex-prefix\src\msvcregex-stamp\msvcregex-configure: msvcregex-prefix\src\msvcregex-stamp\msvcregex-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "No configure step for 'msvcregex'"
	cd D:\workspaces\taos_odbc\contrib\deps-download\msvcregex-prefix\src\msvcregex-build
	C:\App\CMake\bin\cmake.exe -E echo_append
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-configure
	cd D:\workspaces\taos_odbc\contrib\deps-download

msvcregex-prefix\src\msvcregex-stamp\msvcregex-download: msvcregex-prefix\src\msvcregex-stamp\msvcregex-gitinfo.txt
msvcregex-prefix\src\msvcregex-stamp\msvcregex-download: msvcregex-prefix\src\msvcregex-stamp\msvcregex-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (git clone) for 'msvcregex'"
	cd D:\workspaces\taos_odbc\contrib
	C:\App\CMake\bin\cmake.exe -P D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/tmp/msvcregex-gitclone.cmake
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-download
	cd D:\workspaces\taos_odbc\contrib\deps-download

msvcregex-prefix\src\msvcregex-stamp\msvcregex-install: msvcregex-prefix\src\msvcregex-stamp\msvcregex-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No install step for 'msvcregex'"
	cd D:\workspaces\taos_odbc\contrib\deps-download\msvcregex-prefix\src\msvcregex-build
	C:\App\CMake\bin\cmake.exe -E echo_append
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-install
	cd D:\workspaces\taos_odbc\contrib\deps-download

msvcregex-prefix\src\msvcregex-stamp\msvcregex-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Creating directories for 'msvcregex'"
	C:\App\CMake\bin\cmake.exe -Dcfgdir= -P D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/tmp/msvcregex-mkdirs.cmake
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-mkdir

msvcregex-prefix\src\msvcregex-stamp\msvcregex-patch: msvcregex-prefix\src\msvcregex-stamp\msvcregex-update
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "No patch step for 'msvcregex'"
	C:\App\CMake\bin\cmake.exe -E echo_append
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-patch

msvcregex-prefix\src\msvcregex-stamp\msvcregex-test: msvcregex-prefix\src\msvcregex-stamp\msvcregex-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "No test step for 'msvcregex'"
	cd D:\workspaces\taos_odbc\contrib\deps-download\msvcregex-prefix\src\msvcregex-build
	C:\App\CMake\bin\cmake.exe -E echo_append
	C:\App\CMake\bin\cmake.exe -E touch D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/src/msvcregex-stamp/msvcregex-test
	cd D:\workspaces\taos_odbc\contrib\deps-download

msvcregex-prefix\src\msvcregex-stamp\msvcregex-update: msvcregex-prefix\src\msvcregex-stamp\msvcregex-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles --progress-num=$(CMAKE_PROGRESS_9) "Performing update step for 'msvcregex'"
	cd D:\workspaces\taos_odbc\contrib\msvcregex
	C:\App\CMake\bin\cmake.exe -P D:/workspaces/taos_odbc/contrib/deps-download/msvcregex-prefix/tmp/msvcregex-gitupdate.cmake
	cd D:\workspaces\taos_odbc\contrib\deps-download

msvcregex: CMakeFiles\msvcregex
msvcregex: CMakeFiles\msvcregex-complete
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-build
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-configure
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-download
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-install
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-mkdir
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-patch
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-test
msvcregex: msvcregex-prefix\src\msvcregex-stamp\msvcregex-update
msvcregex: CMakeFiles\msvcregex.dir\build.make
.PHONY : msvcregex

# Rule to build all files generated by this target.
CMakeFiles\msvcregex.dir\build: msvcregex
.PHONY : CMakeFiles\msvcregex.dir\build

CMakeFiles\msvcregex.dir\clean:
	$(CMAKE_COMMAND) -P CMakeFiles\msvcregex.dir\cmake_clean.cmake
.PHONY : CMakeFiles\msvcregex.dir\clean

CMakeFiles\msvcregex.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\workspaces\taos_odbc\contrib\deps-download D:\workspaces\taos_odbc\contrib\deps-download D:\workspaces\taos_odbc\contrib\deps-download D:\workspaces\taos_odbc\contrib\deps-download D:\workspaces\taos_odbc\contrib\deps-download\CMakeFiles\msvcregex.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles\msvcregex.dir\depend

