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
CMAKE_SOURCE_DIR = D:\workspaces\taos_odbc

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Utility rule file for ExperimentalSubmit.

# Include any custom commands dependencies for this target.
include contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\compiler_depend.make

# Include the progress variables for this target.
include contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\progress.make

contrib\libuv\CMakeFiles\ExperimentalSubmit:
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\libuv
	C:\App\CMake\bin\ctest.exe -D ExperimentalSubmit
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

ExperimentalSubmit: contrib\libuv\CMakeFiles\ExperimentalSubmit
ExperimentalSubmit: contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\build.make
.PHONY : ExperimentalSubmit

# Rule to build all files generated by this target.
contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\build: ExperimentalSubmit
.PHONY : contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\build

contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\clean:
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\libuv
	$(CMAKE_COMMAND) -P CMakeFiles\ExperimentalSubmit.dir\cmake_clean.cmake
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio
.PHONY : contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\clean

contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\workspaces\taos_odbc D:\workspaces\taos_odbc\contrib\libuv D:\workspaces\taos_odbc\cmake-build-debug-visual-studio D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\libuv D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : contrib\libuv\CMakeFiles\ExperimentalSubmit.dir\depend

