# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

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

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/cxs/Sylar

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/cxs/Sylar/cmake

# Include any dependencies generated for this target.
include CMakeFiles/Utility.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/Utility.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/Utility.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Utility.dir/flags.make

CMakeFiles/Utility.dir/Utility/cmutex.cc.o: CMakeFiles/Utility.dir/flags.make
CMakeFiles/Utility.dir/Utility/cmutex.cc.o: ../Utility/cmutex.cc
CMakeFiles/Utility.dir/Utility/cmutex.cc.o: CMakeFiles/Utility.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxs/Sylar/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/Utility.dir/Utility/cmutex.cc.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Utility.dir/Utility/cmutex.cc.o -MF CMakeFiles/Utility.dir/Utility/cmutex.cc.o.d -o CMakeFiles/Utility.dir/Utility/cmutex.cc.o -c /home/cxs/Sylar/Utility/cmutex.cc

CMakeFiles/Utility.dir/Utility/cmutex.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Utility.dir/Utility/cmutex.cc.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxs/Sylar/Utility/cmutex.cc > CMakeFiles/Utility.dir/Utility/cmutex.cc.i

CMakeFiles/Utility.dir/Utility/cmutex.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Utility.dir/Utility/cmutex.cc.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxs/Sylar/Utility/cmutex.cc -o CMakeFiles/Utility.dir/Utility/cmutex.cc.s

CMakeFiles/Utility.dir/Utility/util.cpp.o: CMakeFiles/Utility.dir/flags.make
CMakeFiles/Utility.dir/Utility/util.cpp.o: ../Utility/util.cpp
CMakeFiles/Utility.dir/Utility/util.cpp.o: CMakeFiles/Utility.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/cxs/Sylar/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/Utility.dir/Utility/util.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/Utility.dir/Utility/util.cpp.o -MF CMakeFiles/Utility.dir/Utility/util.cpp.o.d -o CMakeFiles/Utility.dir/Utility/util.cpp.o -c /home/cxs/Sylar/Utility/util.cpp

CMakeFiles/Utility.dir/Utility/util.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/Utility.dir/Utility/util.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/cxs/Sylar/Utility/util.cpp > CMakeFiles/Utility.dir/Utility/util.cpp.i

CMakeFiles/Utility.dir/Utility/util.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/Utility.dir/Utility/util.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/cxs/Sylar/Utility/util.cpp -o CMakeFiles/Utility.dir/Utility/util.cpp.s

# Object files for target Utility
Utility_OBJECTS = \
"CMakeFiles/Utility.dir/Utility/cmutex.cc.o" \
"CMakeFiles/Utility.dir/Utility/util.cpp.o"

# External object files for target Utility
Utility_EXTERNAL_OBJECTS =

libUtility.a: CMakeFiles/Utility.dir/Utility/cmutex.cc.o
libUtility.a: CMakeFiles/Utility.dir/Utility/util.cpp.o
libUtility.a: CMakeFiles/Utility.dir/build.make
libUtility.a: CMakeFiles/Utility.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/cxs/Sylar/cmake/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX static library libUtility.a"
	$(CMAKE_COMMAND) -P CMakeFiles/Utility.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Utility.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Utility.dir/build: libUtility.a
.PHONY : CMakeFiles/Utility.dir/build

CMakeFiles/Utility.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Utility.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Utility.dir/clean

CMakeFiles/Utility.dir/depend:
	cd /home/cxs/Sylar/cmake && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/cxs/Sylar /home/cxs/Sylar /home/cxs/Sylar/cmake /home/cxs/Sylar/cmake /home/cxs/Sylar/cmake/CMakeFiles/Utility.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Utility.dir/depend

