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
include taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\depend.make
# Include any dependencies generated by the compiler for this target.
include taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\compiler_depend.make

# Include the progress variables for this target.
include taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\progress.make

# Include the compile flags for this target's objects.
include taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\flags.make

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\flags.make
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj: D:\workspaces\taos_odbc\taos_odbc_driver\dsn\odbc_dsn.c
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object taos_odbc_driver/CMakeFiles/taos_odbc_setup.dir/dsn/odbc_dsn.c.obj"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj.d --working-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver --filter-prefix="Note: including file: " -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /showIncludes /FoCMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj /FdCMakeFiles\taos_odbc_setup.dir\ /FS -c D:\workspaces\taos_odbc\taos_odbc_driver\dsn\odbc_dsn.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/taos_odbc_setup.dir/dsn/odbc_dsn.c.i"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe > CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\workspaces\taos_odbc\taos_odbc_driver\dsn\odbc_dsn.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/taos_odbc_setup.dir/dsn/odbc_dsn.c.s"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.s /c D:\workspaces\taos_odbc\taos_odbc_driver\dsn\odbc_dsn.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.rc.res: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\flags.make
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.rc.res: D:\workspaces\taos_odbc\taos_odbc_driver\dsn\odbc_dsn.rc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building RC object taos_odbc_driver/CMakeFiles/taos_odbc_setup.dir/dsn/odbc_dsn.rc.res"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe $(RC_DEFINES) $(RC_INCLUDES) $(RC_FLAGS) /fo CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.rc.res D:\workspaces\taos_odbc\taos_odbc_driver\dsn\odbc_dsn.rc
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\flags.make
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj: D:\workspaces\taos_odbc\taos_odbc_driver\dsn\td_dsn.c
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object taos_odbc_driver/CMakeFiles/taos_odbc_setup.dir/dsn/td_dsn.c.obj"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj.d --working-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver --filter-prefix="Note: including file: " -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /showIncludes /FoCMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj /FdCMakeFiles\taos_odbc_setup.dir\ /FS -c D:\workspaces\taos_odbc\taos_odbc_driver\dsn\td_dsn.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/taos_odbc_setup.dir/dsn/td_dsn.c.i"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe > CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\workspaces\taos_odbc\taos_odbc_driver\dsn\td_dsn.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/taos_odbc_setup.dir/dsn/td_dsn.c.s"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.s /c D:\workspaces\taos_odbc\taos_odbc_driver\dsn\td_dsn.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\flags.make
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj: D:\workspaces\taos_odbc\taos_odbc_driver\dsn\platform_win.c
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building C object taos_odbc_driver/CMakeFiles/taos_odbc_setup.dir/dsn/platform_win.c.obj"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	$(CMAKE_COMMAND) -E cmake_cl_compile_depends --dep-file=CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj.d --working-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver --filter-prefix="Note: including file: " -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /showIncludes /FoCMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj /FdCMakeFiles\taos_odbc_setup.dir\ /FS -c D:\workspaces\taos_odbc\taos_odbc_driver\dsn\platform_win.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/taos_odbc_setup.dir/dsn/platform_win.c.i"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe > CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.i @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E D:\workspaces\taos_odbc\taos_odbc_driver\dsn\platform_win.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/taos_odbc_setup.dir/dsn/platform_win.c.s"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\cl.exe @<<
 /nologo $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) /FoNUL /FAs /FaCMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.s /c D:\workspaces\taos_odbc\taos_odbc_driver\dsn\platform_win.c
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Object files for target taos_odbc_setup
taos_odbc_setup_OBJECTS = \
"CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj" \
"CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.rc.res" \
"CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj" \
"CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj"

# External object files for target taos_odbc_setup
taos_odbc_setup_EXTERNAL_OBJECTS =

taos_odbc_driver\taos_odbc_setup.dll: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.c.obj
taos_odbc_driver\taos_odbc_setup.dll: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\odbc_dsn.rc.res
taos_odbc_driver\taos_odbc_setup.dll: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\td_dsn.c.obj
taos_odbc_driver\taos_odbc_setup.dll: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\dsn\platform_win.c.obj
taos_odbc_driver\taos_odbc_setup.dll: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\build.make
taos_odbc_driver\taos_odbc_setup.dll: D:\workspaces\taos_odbc\taos_odbc_driver\dsn\taos_odbc_setup.def
taos_odbc_driver\taos_odbc_setup.dll: taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\objects1
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking C shared library taos_odbc_setup.dll"
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	C:\App\CMake\bin\cmake.exe -E vs_link_dll --intdir=CMakeFiles\taos_odbc_setup.dir --rc=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\rc.exe --mt=C:\PROGRA~2\WI3CF2~1\10\bin\100190~1.0\x86\mt.exe --manifests -- C:\PROGRA~2\MICROS~1\2019\COMMUN~1\VC\Tools\MSVC\1429~1.301\bin\Hostx86\x64\link.exe /nologo @CMakeFiles\taos_odbc_setup.dir\objects1 @<<
 /out:taos_odbc_setup.dll /implib:taos_odbc_setup.lib /pdb:D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver\taos_odbc_setup.pdb /dll /version:0.0 /machine:x64 /debug /INCREMENTAL /DEF:D:\workspaces\taos_odbc\taos_odbc_driver\dsn\taos_odbc_setup.def  comctl32.lib legacy_stdio_definitions.lib Shlwapi.lib odbc32.lib odbccp32.lib kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib  
<<
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio

# Rule to build all files generated by this target.
taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\build: taos_odbc_driver\taos_odbc_setup.dll
.PHONY : taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\build

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\clean:
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver
	$(CMAKE_COMMAND) -P CMakeFiles\taos_odbc_setup.dir\cmake_clean.cmake
	cd D:\workspaces\taos_odbc\cmake-build-debug-visual-studio
.PHONY : taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\clean

taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\depend:
	$(CMAKE_COMMAND) -E cmake_depends "NMake Makefiles" D:\workspaces\taos_odbc D:\workspaces\taos_odbc\taos_odbc_driver D:\workspaces\taos_odbc\cmake-build-debug-visual-studio D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver D:\workspaces\taos_odbc\cmake-build-debug-visual-studio\taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : taos_odbc_driver\CMakeFiles\taos_odbc_setup.dir\depend

