# Install script for directory: /Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/dist")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p-build/Imath/libImath.a")
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath.a" AND
     NOT IS_SYMLINK "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath.a")
    execute_process(COMMAND "/Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin/ranlib" "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/libImath.a")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OpenEXR" TYPE FILE FILES
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathBoxAlgo.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathBox.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathColorAlgo.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathColor.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathEuler.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathExc.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathExport.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathForward.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathFrame.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathFrustum.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathFrustumTest.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathFun.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathGL.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathGLU.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathHalfLimits.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathInt64.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathInterval.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathLimits.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathLineAlgo.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathLine.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathMath.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathMatrixAlgo.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathMatrix.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathNamespace.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathPlane.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathPlatform.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathQuat.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathRandom.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathRoots.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathShear.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathSphere.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathVecAlgo.h"
    "/Users/alessiapaccagnella/Desktop/nori/cmake-build-debug/ext_build/src/IlmBase_p/Imath/ImathVec.h"
    )
endif()

