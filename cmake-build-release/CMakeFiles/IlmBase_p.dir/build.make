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
CMAKE_SOURCE_DIR = /Users/alessiapaccagnella/Desktop/nori

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /Users/alessiapaccagnella/Desktop/nori/cmake-build-release

# Utility rule file for IlmBase_p.

# Include the progress variables for this target.
include CMakeFiles/IlmBase_p.dir/progress.make

CMakeFiles/IlmBase_p: CMakeFiles/IlmBase_p-complete


CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-install
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-mkdir
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-download
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-update
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-patch
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-configure
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-build
CMakeFiles/IlmBase_p-complete: ext_build/src/IlmBase_p-stamp/IlmBase_p-install
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Completed 'IlmBase_p'"
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles/IlmBase_p-complete
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-done

ext_build/src/IlmBase_p-stamp/IlmBase_p-install: ext_build/src/IlmBase_p-stamp/IlmBase_p-build
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Performing install step for 'IlmBase_p'"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build && $(MAKE) install
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-install

ext_build/src/IlmBase_p-stamp/IlmBase_p-mkdir:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Creating directories for 'IlmBase_p'"
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/tmp
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E make_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-mkdir

ext_build/src/IlmBase_p-stamp/IlmBase_p-download: ext_build/src/IlmBase_p-stamp/IlmBase_p-urlinfo.txt
ext_build/src/IlmBase_p-stamp/IlmBase_p-download: ext_build/src/IlmBase_p-stamp/IlmBase_p-mkdir
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Performing download step (DIR copy) for 'IlmBase_p'"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove_directory /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E copy_directory /Users/alessiapaccagnella/Desktop/nori/ext/openexr/IlmBase /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-download

ext_build/src/IlmBase_p-stamp/IlmBase_p-update: ext_build/src/IlmBase_p-stamp/IlmBase_p-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "No update step for 'IlmBase_p'"
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E echo_append
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-update

ext_build/src/IlmBase_p-stamp/IlmBase_p-patch: ext_build/src/IlmBase_p-stamp/IlmBase_p-download
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_6) "No patch step for 'IlmBase_p'"
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E echo_append
	/Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-patch

ext_build/src/IlmBase_p-stamp/IlmBase_p-configure: ext_build/tmp/IlmBase_p-cfgcmd.txt
ext_build/src/IlmBase_p-stamp/IlmBase_p-configure: ext_build/src/IlmBase_p-stamp/IlmBase_p-update
ext_build/src/IlmBase_p-stamp/IlmBase_p-configure: ext_build/src/IlmBase_p-stamp/IlmBase_p-patch
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_7) "Performing configure step for 'IlmBase_p'"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -DCMAKE_BUILD_TYPE=Release "-DCMAKE_GENERATOR=Unix Makefiles" -Wno-dev -DCMAKE_INSTALL_PREFIX=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/dist -DCMAKE_OSX_DEPLOYMENT_TARGET=10.14 -DCMAKE_OSX_SYSROOT=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.14.sdk -DBUILD_SHARED_LIBS=OFF -DNAMESPACE_VERSIONING=NO "-GCodeBlocks - Unix Makefiles" /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-configure

ext_build/src/IlmBase_p-stamp/IlmBase_p-build: ext_build/src/IlmBase_p-stamp/IlmBase_p-configure
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles --progress-num=$(CMAKE_PROGRESS_8) "Performing build step for 'IlmBase_p'"
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build && $(MAKE)
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-build && /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E touch /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/ext_build/src/IlmBase_p-stamp/IlmBase_p-build

IlmBase_p: CMakeFiles/IlmBase_p
IlmBase_p: CMakeFiles/IlmBase_p-complete
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-install
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-mkdir
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-download
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-update
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-patch
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-configure
IlmBase_p: ext_build/src/IlmBase_p-stamp/IlmBase_p-build
IlmBase_p: CMakeFiles/IlmBase_p.dir/build.make

.PHONY : IlmBase_p

# Rule to build all files generated by this target.
CMakeFiles/IlmBase_p.dir/build: IlmBase_p

.PHONY : CMakeFiles/IlmBase_p.dir/build

CMakeFiles/IlmBase_p.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/IlmBase_p.dir/cmake_clean.cmake
.PHONY : CMakeFiles/IlmBase_p.dir/clean

CMakeFiles/IlmBase_p.dir/depend:
	cd /Users/alessiapaccagnella/Desktop/nori/cmake-build-release && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /Users/alessiapaccagnella/Desktop/nori /Users/alessiapaccagnella/Desktop/nori /Users/alessiapaccagnella/Desktop/nori/cmake-build-release /Users/alessiapaccagnella/Desktop/nori/cmake-build-release /Users/alessiapaccagnella/Desktop/nori/cmake-build-release/CMakeFiles/IlmBase_p.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/IlmBase_p.dir/depend

