# CMAKE generated file: DO NOT EDIT!
# Generated by "MinGW Makefiles" Generator, CMake Version 3.7

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

SHELL = cmd.exe

# The CMake executable.
CMAKE_COMMAND = C:\Users\Host\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\171.3780.121\bin\cmake\bin\cmake.exe

# The command to remove a file.
RM = C:\Users\Host\AppData\Local\JetBrains\Toolbox\apps\CLion\ch-0\171.3780.121\bin\cmake\bin\cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = E:\Projekty\stm32

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = E:\Projekty\stm32\cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/stm32.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/stm32.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/stm32.dir/flags.make

CMakeFiles/stm32.dir/main.c.obj: CMakeFiles/stm32.dir/flags.make
CMakeFiles/stm32.dir/main.c.obj: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=E:\Projekty\stm32\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/stm32.dir/main.c.obj"
	C:\DEV\MinGW\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles\stm32.dir\main.c.obj   -c E:\Projekty\stm32\main.c

CMakeFiles/stm32.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/stm32.dir/main.c.i"
	C:\DEV\MinGW\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E E:\Projekty\stm32\main.c > CMakeFiles\stm32.dir\main.c.i

CMakeFiles/stm32.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/stm32.dir/main.c.s"
	C:\DEV\MinGW\mingw64\bin\gcc.exe  $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S E:\Projekty\stm32\main.c -o CMakeFiles\stm32.dir\main.c.s

CMakeFiles/stm32.dir/main.c.obj.requires:

.PHONY : CMakeFiles/stm32.dir/main.c.obj.requires

CMakeFiles/stm32.dir/main.c.obj.provides: CMakeFiles/stm32.dir/main.c.obj.requires
	$(MAKE) -f CMakeFiles\stm32.dir\build.make CMakeFiles/stm32.dir/main.c.obj.provides.build
.PHONY : CMakeFiles/stm32.dir/main.c.obj.provides

CMakeFiles/stm32.dir/main.c.obj.provides.build: CMakeFiles/stm32.dir/main.c.obj


# Object files for target stm32
stm32_OBJECTS = \
"CMakeFiles/stm32.dir/main.c.obj"

# External object files for target stm32
stm32_EXTERNAL_OBJECTS =

stm32.exe: CMakeFiles/stm32.dir/main.c.obj
stm32.exe: CMakeFiles/stm32.dir/build.make
stm32.exe: CMakeFiles/stm32.dir/linklibs.rsp
stm32.exe: CMakeFiles/stm32.dir/objects1.rsp
stm32.exe: CMakeFiles/stm32.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=E:\Projekty\stm32\cmake-build-debug\CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable stm32.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles\stm32.dir\link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/stm32.dir/build: stm32.exe

.PHONY : CMakeFiles/stm32.dir/build

CMakeFiles/stm32.dir/requires: CMakeFiles/stm32.dir/main.c.obj.requires

.PHONY : CMakeFiles/stm32.dir/requires

CMakeFiles/stm32.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles\stm32.dir\cmake_clean.cmake
.PHONY : CMakeFiles/stm32.dir/clean

CMakeFiles/stm32.dir/depend:
	$(CMAKE_COMMAND) -E cmake_depends "MinGW Makefiles" E:\Projekty\stm32 E:\Projekty\stm32 E:\Projekty\stm32\cmake-build-debug E:\Projekty\stm32\cmake-build-debug E:\Projekty\stm32\cmake-build-debug\CMakeFiles\stm32.dir\DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/stm32.dir/depend

