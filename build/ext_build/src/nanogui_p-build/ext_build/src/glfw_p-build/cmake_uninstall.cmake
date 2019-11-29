
if (NOT EXISTS "/Users/alessiapaccagnella/desktop/nori/build/ext_build/src/nanogui_p-build/ext_build/src/glfw_p-build/install_manifest.txt")
  message(FATAL_ERROR "Cannot find install manifest: \"/Users/alessiapaccagnella/desktop/nori/build/ext_build/src/nanogui_p-build/ext_build/src/glfw_p-build/install_manifest.txt\"")
endif()

file(READ "/Users/alessiapaccagnella/desktop/nori/build/ext_build/src/nanogui_p-build/ext_build/src/glfw_p-build/install_manifest.txt" files)
string(REGEX REPLACE "\n" ";" files "${files}")

foreach (file ${files})
  message(STATUS "Uninstalling \"$ENV{DESTDIR}${file}\"")
  if (EXISTS "$ENV{DESTDIR}${file}")
    exec_program("/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/cmake/data/CMake.app/Contents/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      MESSAGE(FATAL_ERROR "Problem when removing \"$ENV{DESTDIR}${file}\"")
    endif()
  elseif (IS_SYMLINK "$ENV{DESTDIR}${file}")
    EXEC_PROGRAM("/Library/Frameworks/Python.framework/Versions/3.6/lib/python3.6/site-packages/cmake/data/CMake.app/Contents/bin/cmake" ARGS "-E remove \"$ENV{DESTDIR}${file}\""
                 OUTPUT_VARIABLE rm_out
                 RETURN_VALUE rm_retval)
    if (NOT "${rm_retval}" STREQUAL 0)
      message(FATAL_ERROR "Problem when removing symlink \"$ENV{DESTDIR}${file}\"")
    endif()
  else()
    message(STATUS "File \"$ENV{DESTDIR}${file}\" does not exist.")
  endif()
endforeach()

