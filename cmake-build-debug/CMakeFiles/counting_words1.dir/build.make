# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /home/romanblahuta/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/193.6015.37/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/romanblahuta/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/193.6015.37/bin/cmake/linux/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/romanblahuta/UCU/Programming/AKC/count_words

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/counting_words1.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/counting_words1.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/counting_words1.dir/flags.make

CMakeFiles/counting_words1.dir/main.cpp.o: CMakeFiles/counting_words1.dir/flags.make
CMakeFiles/counting_words1.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/counting_words1.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/counting_words1.dir/main.cpp.o -c /home/romanblahuta/UCU/Programming/AKC/count_words/main.cpp

CMakeFiles/counting_words1.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/counting_words1.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/romanblahuta/UCU/Programming/AKC/count_words/main.cpp > CMakeFiles/counting_words1.dir/main.cpp.i

CMakeFiles/counting_words1.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/counting_words1.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/romanblahuta/UCU/Programming/AKC/count_words/main.cpp -o CMakeFiles/counting_words1.dir/main.cpp.s

# Object files for target counting_words1
counting_words1_OBJECTS = \
"CMakeFiles/counting_words1.dir/main.cpp.o"

# External object files for target counting_words1
counting_words1_EXTERNAL_OBJECTS =

counting_words1: CMakeFiles/counting_words1.dir/main.cpp.o
counting_words1: CMakeFiles/counting_words1.dir/build.make
counting_words1: /usr/lib/x86_64-linux-gnu/libarchive.so
counting_words1: CMakeFiles/counting_words1.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable counting_words1"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/counting_words1.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/counting_words1.dir/build: counting_words1

.PHONY : CMakeFiles/counting_words1.dir/build

CMakeFiles/counting_words1.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/counting_words1.dir/cmake_clean.cmake
.PHONY : CMakeFiles/counting_words1.dir/clean

CMakeFiles/counting_words1.dir/depend:
	cd /home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/romanblahuta/UCU/Programming/AKC/count_words /home/romanblahuta/UCU/Programming/AKC/count_words /home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug /home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug /home/romanblahuta/UCU/Programming/AKC/count_words/cmake-build-debug/CMakeFiles/counting_words1.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/counting_words1.dir/depend

