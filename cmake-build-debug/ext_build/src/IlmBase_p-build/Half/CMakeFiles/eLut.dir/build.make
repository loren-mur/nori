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
CMAKE_SOURCE_DIR = /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build

# Include any dependencies generated for this target.
include Half/CMakeFiles/eLut.dir/depend.make

# Include the progress variables for this target.
include Half/CMakeFiles/eLut.dir/progress.make

# Include the compile flags for this target's objects.
include Half/CMakeFiles/eLut.dir/flags.make

Half/CMakeFiles/eLut.dir/eLut.cpp.o: Half/CMakeFiles/eLut.dir/flags.make
Half/CMakeFiles/eLut.dir/eLut.cpp.o: /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Half/eLut.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object Half/CMakeFiles/eLut.dir/eLut.cpp.o"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/eLut.dir/eLut.cpp.o -c /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Half/eLut.cpp

Half/CMakeFiles/eLut.dir/eLut.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eLut.dir/eLut.cpp.i"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Half/eLut.cpp > CMakeFiles/eLut.dir/eLut.cpp.i

Half/CMakeFiles/eLut.dir/eLut.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eLut.dir/eLut.cpp.s"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half && /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Half/eLut.cpp -o CMakeFiles/eLut.dir/eLut.cpp.s

# Object files for target eLut
eLut_OBJECTS = \
"CMakeFiles/eLut.dir/eLut.cpp.o"

# External object files for target eLut
eLut_EXTERNAL_OBJECTS =

Half/eLut: Half/CMakeFiles/eLut.dir/eLut.cpp.o
Half/eLut: Half/CMakeFiles/eLut.dir/build.make
Half/eLut: Half/CMakeFiles/eLut.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable eLut"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/eLut.dir/link.txt --verbose=$(VERBOSE)
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half && /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half/eLut > /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half/eLut.h

# Rule to build all files generated by this target.
Half/CMakeFiles/eLut.dir/build: Half/eLut

.PHONY : Half/CMakeFiles/eLut.dir/build

Half/CMakeFiles/eLut.dir/clean:
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half && $(CMAKE_COMMAND) -P CMakeFiles/eLut.dir/cmake_clean.cmake
.PHONY : Half/CMakeFiles/eLut.dir/clean

Half/CMakeFiles/eLut.dir/depend:
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Half /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Half/CMakeFiles/eLut.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : Half/CMakeFiles/eLut.dir/depend
