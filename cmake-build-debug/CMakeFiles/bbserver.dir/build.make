# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.8

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /cygdrive/c/Users/Tyler/.CLion2017.2/system/cygwin_cmake/bin/cmake.exe

# The command to remove a file.
RM = /cygdrive/c/Users/Tyler/.CLion2017.2/system/cygwin_cmake/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug"

# Include any dependencies generated for this target.
include CMakeFiles/bbserver.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bbserver.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bbserver.dir/flags.make

CMakeFiles/bbserver.dir/server.c.o: CMakeFiles/bbserver.dir/flags.make
CMakeFiles/bbserver.dir/server.c.o: ../server.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bbserver.dir/server.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bbserver.dir/server.c.o   -c "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/server.c"

CMakeFiles/bbserver.dir/server.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bbserver.dir/server.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/server.c" > CMakeFiles/bbserver.dir/server.c.i

CMakeFiles/bbserver.dir/server.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bbserver.dir/server.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/server.c" -o CMakeFiles/bbserver.dir/server.c.s

CMakeFiles/bbserver.dir/server.c.o.requires:

.PHONY : CMakeFiles/bbserver.dir/server.c.o.requires

CMakeFiles/bbserver.dir/server.c.o.provides: CMakeFiles/bbserver.dir/server.c.o.requires
	$(MAKE) -f CMakeFiles/bbserver.dir/build.make CMakeFiles/bbserver.dir/server.c.o.provides.build
.PHONY : CMakeFiles/bbserver.dir/server.c.o.provides

CMakeFiles/bbserver.dir/server.c.o.provides.build: CMakeFiles/bbserver.dir/server.c.o


# Object files for target bbserver
bbserver_OBJECTS = \
"CMakeFiles/bbserver.dir/server.c.o"

# External object files for target bbserver
bbserver_EXTERNAL_OBJECTS =

bbserver.exe: CMakeFiles/bbserver.dir/server.c.o
bbserver.exe: CMakeFiles/bbserver.dir/build.make
bbserver.exe: CMakeFiles/bbserver.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bbserver.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bbserver.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bbserver.dir/build: bbserver.exe

.PHONY : CMakeFiles/bbserver.dir/build

CMakeFiles/bbserver.dir/requires: CMakeFiles/bbserver.dir/server.c.o.requires

.PHONY : CMakeFiles/bbserver.dir/requires

CMakeFiles/bbserver.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bbserver.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bbserver.dir/clean

CMakeFiles/bbserver.dir/depend:
	cd "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring" "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring" "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug" "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug" "/cygdrive/d/UWF Spring 2018/SysNetII/p2_token_ring/p2_token_ring/cmake-build-debug/CMakeFiles/bbserver.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : CMakeFiles/bbserver.dir/depend

