# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/TeamPog_CS3505.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/TeamPog_CS3505.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/TeamPog_CS3505.dir/flags.make

CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.o: CMakeFiles/TeamPog_CS3505.dir/flags.make
CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.o: ../network_server/networking.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.o -c /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/networking.cpp

CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/networking.cpp > CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.i

CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/networking.cpp -o CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.s

CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.o: CMakeFiles/TeamPog_CS3505.dir/flags.make
CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.o: ../network_server/socket_state.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.o -c /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/socket_state.cpp

CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/socket_state.cpp > CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.i

CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/socket_state.cpp -o CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.s

CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.o: CMakeFiles/TeamPog_CS3505.dir/flags.make
CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.o: ../network_server/server_controller.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.o -c /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/server_controller.cpp

CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/server_controller.cpp > CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.i

CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/network_server/server_controller.cpp -o CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.s

CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.o: CMakeFiles/TeamPog_CS3505.dir/flags.make
CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.o: ../spreadsheet/dependency_graph.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.o -c /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/spreadsheet/dependency_graph.cpp

CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/spreadsheet/dependency_graph.cpp > CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.i

CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/spreadsheet/dependency_graph.cpp -o CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.s

CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.o: CMakeFiles/TeamPog_CS3505.dir/flags.make
CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.o: ../Test/test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Building CXX object CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.o"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.o -c /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/Test/test.cpp

CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.i"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/Test/test.cpp > CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.i

CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.s"
	/Library/Developer/CommandLineTools/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/Test/test.cpp -o CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.s

# Object files for target TeamPog_CS3505
TeamPog_CS3505_OBJECTS = \
"CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.o" \
"CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.o" \
"CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.o" \
"CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.o" \
"CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.o"

# External object files for target TeamPog_CS3505
TeamPog_CS3505_EXTERNAL_OBJECTS =

TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/network_server/networking.cpp.o
TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/network_server/socket_state.cpp.o
TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/network_server/server_controller.cpp.o
TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/spreadsheet/dependency_graph.cpp.o
TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/Test/test.cpp.o
TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/build.make
TeamPog_CS3505: CMakeFiles/TeamPog_CS3505.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "Linking CXX executable TeamPog_CS3505"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/TeamPog_CS3505.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/TeamPog_CS3505.dir/build: TeamPog_CS3505

.PHONY : CMakeFiles/TeamPog_CS3505.dir/build

CMakeFiles/TeamPog_CS3505.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/TeamPog_CS3505.dir/cmake_clean.cmake
.PHONY : CMakeFiles/TeamPog_CS3505.dir/clean

CMakeFiles/TeamPog_CS3505.dir/depend:
	cd /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505 /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505 /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug /Users/laurenschwenke/cpp-workspace/cs3505/TeamPog-CS3505/cmake-build-debug/CMakeFiles/TeamPog_CS3505.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/TeamPog_CS3505.dir/depend

