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
include contrib\wingetopt\CMakeFiles\wingetopt.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include contrib\wingetopt\CMakeFiles\wingetopt.dir\compiler_depend.make

# Include the progress variables for this target.
include contrib\wingetopt\CMakeFiles\wingetopt.dir\progress.make

# Include the compile flags for this target's objects.
include contrib\wingetopt\CMakeFiles\wingetopt.dir\flags.make

contrib\wingetopt\CMakeFiles\wingetopt.dir\src\getopt.c.obj: contrib\wingetopt\CMakeFiles\wingetopt.dir\flags.make
contrib\wingetopt\CMakeFiles\wingetopt.dir\src\getopt.c.obj: D:\workspaces\taos_odbc\contrib\wingetopt\src\getopt.c
contrib\wingetopt\CMakeFiles\wingetopt.dir\src\getopt.c.obj: contrib\wingetopt\CMakeFiles\wingetopt.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object contrib/wingetopt/CMakeFiles/wingetopt.dir/src/getopt.c.obj"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\wingetopt.dir\src\getopt.c.obj.d --working-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt --filter-prefix="Note: including file: " -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /showIncludes /FoCMakeFiles\wingetopt.dir\src\getopt.c.obj /FdCMakeFiles\wingetopt.dir\ /FS -c D:\workspaces\taos_odbc\contrib\wingetopt\src\getopt.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

contrib\wingetopt\CMakeFiles\wingetopt.dir\src\getopt.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/wingetopt.dir/src/getopt.c.i"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe > CMakeFiles\wingetopt.dir\src\getopt.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\workspaces\taos_odbc\contrib\wingetopt\src\getopt.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

contrib\wingetopt\CMakeFiles\wingetopt.dir\src\getopt.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/wingetopt.dir/src/getopt.c.s"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\wingetopt.dir\src\getopt.c.s /c D:\workspaces\taos_odbc\contrib\wingetopt\src\getopt.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Object files for target wingetopt
wingetopt_OBJECTS = \
"CMakeFiles\wingetopt.dir\src\getopt.c.obj"

# External object files for target wingetopt
wingetopt_EXTERNAL_OBJECTS =

contrib\wingetopt\wingetopt.dll: contrib\wingetopt\CMakeFiles\wingetopt.dir\src\getopt.c.obj
contrib\wingetopt\wingetopt.dll: contrib\wingetopt\CMakeFiles\wingetopt.dir\build.make
contrib\wingetopt\wingetopt.dll: contrib\wingetopt\CMakeFiles\wingetopt.dir\objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C shared library wingetopt.dll"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt
	C:\App\CMake\bin\cmake.exe -E __create_def CMakeFiles\wingetopt.dir\exports.def CMakeFiles\wingetopt.dir\exports.def.objs
	C:\App\CMake\bin\cmake.exe -E vs_link_dll --intdir=CMakeFiles\wingetopt.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\mt.exe --manifests -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\link.exe /nologo @CMakeFiles\wingetopt.dir\objects1 @<<
 /out:wingetopt.dll /implib:wingetopt.lib /pdb:D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt\wingetopt.pdb /dll /version:0.0 /machine:x64 /debug /INCREMENTAL /DEF:CMakeFiles\wingetopt.dir\exports.def  kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Rule to build all files generated by this target.
contrib\wingetopt\CMakeFiles\wingetopt.dir\build: contrib\wingetopt\wingetopt.dll
.PHONY : contrib\wingetopt\CMakeFiles\wingetopt.dir\build

contrib\wingetopt\CMakeFiles\wingetopt.dir\clean:
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt
	$(CMAKE_COMMAND) -P CMakeFiles\wingetopt.dir\cmake_clean.cmake
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio
.PHONY : contrib\wingetopt\CMakeFiles\wingetopt.dir\clean

contrib\wingetopt\CMakeFiles\wingetopt.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\workspaces\taos_odbc D:\workspaces\taos_odbc\contrib\wingetopt D:\workspaces\taos_odbc\cmake-build-debug-visual-studio D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\contrib\wingetopt\CMakeFiles\wingetopt.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : contrib\wingetopt\CMakeFiles\wingetopt.dir\depend

