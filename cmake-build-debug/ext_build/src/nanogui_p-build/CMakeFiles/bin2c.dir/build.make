# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build

# Include any dependencies generated for this target.
include CMakeFiles/bin2c.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/bin2c.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/bin2c.dir/flags.make

CMakeFiles/bin2c.dir/resources/bin2c.c.o: CMakeFiles/bin2c.dir/flags.make
CMakeFiles/bin2c.dir/resources/bin2c.c.o: /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p/resources/bin2c.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/bin2c.dir/resources/bin2c.c.o"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/bin2c.dir/resources/bin2c.c.o   -c /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p/resources/bin2c.c

CMakeFiles/bin2c.dir/resources/bin2c.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/bin2c.dir/resources/bin2c.c.i"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p/resources/bin2c.c > CMakeFiles/bin2c.dir/resources/bin2c.c.i

CMakeFiles/bin2c.dir/resources/bin2c.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/bin2c.dir/resources/bin2c.c.s"
	/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p/resources/bin2c.c -o CMakeFiles/bin2c.dir/resources/bin2c.c.s

# Object files for target bin2c
bin2c_OBJECTS = \
"CMakeFiles/bin2c.dir/resources/bin2c.c.o"

# External object files for target bin2c
bin2c_EXTERNAL_OBJECTS =

bin2c: CMakeFiles/bin2c.dir/resources/bin2c.c.o
bin2c: CMakeFiles/bin2c.dir/build.make
bin2c: CMakeFiles/bin2c.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable bin2c"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/bin2c.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/bin2c.dir/build: bin2c

.PHONY : CMakeFiles/bin2c.dir/build

CMakeFiles/bin2c.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/bin2c.dir/cmake_clean.cmake
.PHONY : CMakeFiles/bin2c.dir/clean

CMakeFiles/bin2c.dir/depend:
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/nanogui_p-build/CMakeFiles/bin2c.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/bin2c.dir/depend

