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
CMAKE_COMMAND = /home/ivb/clion-2017.2.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/ivb/clion-2017.2.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/tokenizer.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/tokenizer.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/tokenizer.dir/flags.make

CMakeFiles/tokenizer.dir/main.cpp.o: CMakeFiles/tokenizer.dir/flags.make
CMakeFiles/tokenizer.dir/main.cpp.o: ../main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/tokenizer.dir/main.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/tokenizer.dir/main.cpp.o -c /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/main.cpp

CMakeFiles/tokenizer.dir/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/tokenizer.dir/main.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/main.cpp > CMakeFiles/tokenizer.dir/main.cpp.i

CMakeFiles/tokenizer.dir/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/tokenizer.dir/main.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/main.cpp -o CMakeFiles/tokenizer.dir/main.cpp.s

CMakeFiles/tokenizer.dir/main.cpp.o.requires:

.PHONY : CMakeFiles/tokenizer.dir/main.cpp.o.requires

CMakeFiles/tokenizer.dir/main.cpp.o.provides: CMakeFiles/tokenizer.dir/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/tokenizer.dir/build.make CMakeFiles/tokenizer.dir/main.cpp.o.provides.build
.PHONY : CMakeFiles/tokenizer.dir/main.cpp.o.provides

CMakeFiles/tokenizer.dir/main.cpp.o.provides.build: CMakeFiles/tokenizer.dir/main.cpp.o


# Object files for target tokenizer
tokenizer_OBJECTS = \
"CMakeFiles/tokenizer.dir/main.cpp.o"

# External object files for target tokenizer
tokenizer_EXTERNAL_OBJECTS =

tokenizer: CMakeFiles/tokenizer.dir/main.cpp.o
tokenizer: CMakeFiles/tokenizer.dir/build.make
tokenizer: CMakeFiles/tokenizer.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable tokenizer"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/tokenizer.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/tokenizer.dir/build: tokenizer

.PHONY : CMakeFiles/tokenizer.dir/build

CMakeFiles/tokenizer.dir/requires: CMakeFiles/tokenizer.dir/main.cpp.o.requires

.PHONY : CMakeFiles/tokenizer.dir/requires

CMakeFiles/tokenizer.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/tokenizer.dir/cmake_clean.cmake
.PHONY : CMakeFiles/tokenizer.dir/clean

CMakeFiles/tokenizer.dir/depend:
	cd /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug /home/ivb/Documents/MIPT/term_6/prog_lang/seminars/tokenizer/cmake-build-debug/CMakeFiles/tokenizer.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/tokenizer.dir/depend

