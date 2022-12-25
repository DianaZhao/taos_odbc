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

# Include any dependencies generated for this target.
include contrib\CMakeFiles\iconv.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include contrib\CMakeFiles\iconv.dir\compiler_depend.make

# Include the progress variables for this target.
include contrib\CMakeFiles\iconv.dir\progress.make

# Include the compile flags for this target's objects.
include contrib\CMakeFiles\iconv.dir\flags.make

contrib\CMakeFiles\iconv.dir\iconv\win_iconv.c.obj: contrib\CMakeFiles\iconv.dir\flags.make
contrib\CMakeFiles\iconv.dir\iconv\win_iconv.c.obj: D:\workspaces\taos_odbc\contrib\iconv\win_iconv.c
contrib\CMakeFiles\iconv.dir\iconv\win_iconv.c.obj: contrib\CMakeFiles\iconv.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object contrib/CMakeFiles/iconv.dir/iconv/win_iconv.c.obj"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\iconv.dir\iconv\win_iconv.c.obj.d --working-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib --filter-prefix="Note: including file: " -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /showIncludes /FoCMakeFiles\iconv.dir\iconv\win_iconv.c.obj /FdCMakeFiles\iconv.dir\iconv.pdb /FS -c D:\workspaces\taos_odbc\contrib\iconv\win_iconv.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

contrib\CMakeFiles\iconv.dir\iconv\win_iconv.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/iconv.dir/iconv/win_iconv.c.i"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe > CMakeFiles\iconv.dir\iconv\win_iconv.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\workspaces\taos_odbc\contrib\iconv\win_iconv.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

contrib\CMakeFiles\iconv.dir\iconv\win_iconv.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/iconv.dir/iconv/win_iconv.c.s"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\iconv.dir\iconv\win_iconv.c.s /c D:\workspaces\taos_odbc\contrib\iconv\win_iconv.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Object files for target iconv
iconv_OBJECTS = \
"CMakeFiles\iconv.dir\iconv\win_iconv.c.obj"

# External object files for target iconv
iconv_EXTERNAL_OBJECTS =

contrib\iconv.lib: contrib\CMakeFiles\iconv.dir\iconv\win_iconv.c.obj
contrib\iconv.lib: contrib\CMakeFiles\iconv.dir\build.make
contrib\iconv.lib: contrib\CMakeFiles\iconv.dir\objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C static library iconv.lib"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib
	$(CMAKE_COMMAND) -P CMakeFiles\iconv.dir\cmake_clean_target.cmake
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\lib.exe /nologo /machine:x64 /out:iconv.lib @CMakeFiles\iconv.dir\objects1 
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Rule to build all files generated by this target.
contrib\CMakeFiles\iconv.dir\build: contrib\iconv.lib
.PHONY : contrib\CMakeFiles\iconv.dir\build

contrib\CMakeFiles\iconv.dir\clean:
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib
	$(CMAKE_COMMAND) -P CMakeFiles\iconv.dir\cmake_clean.cmake
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio
.PHONY : contrib\CMakeFiles\iconv.dir\clean

contrib\CMakeFiles\iconv.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\workspaces\taos_odbc D:\workspaces\taos_odbc\contrib D:\workspaces\taos_odbc\cmake-build-debug-visual-studio D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\CMakeFiles\iconv.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : contrib\CMakeFiles\iconv.dir\depend

