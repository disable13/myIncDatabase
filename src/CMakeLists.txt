cmake_minimum_required(VERSION 2.6)
project("myIncDatabase")
##
# This is global config place
##
set(CMAKE_DEBUG_POSTFIX "_d")
if (CMAKE_BUILD_TYPE STREQUAL "")
  # CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
  # differentiation between debug and release builds.
  set(CMAKE_BUILD_TYPE "RelWithDebInfo"
    CACHE STRING "Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel."
     FORCE)
endif()
##
#  Options
##
option(CLIENT_BUILD
    "Compile myIncDatabase client programm."
    ON
)
##
# Sub - project
##
if(CLIENT_BUILD)
    subdirs( app )
endif()
#if(PLUGIN_WIZARD)
#   subdirs( libs\wizard )
#endif()

