# Builds the libraries for Boost.Nowide
# Automatically detects if build as standalone or as "part of boost" (with boost namespace)
#
# Options:
# BOOST_NOWIDE_INSTALL
# BOOST_NOWIDE_BUILD_TESTS
# BOOST_NOWIDE_SYSTEM_INCLUDE
#
# Created target: Boost::nowide
#
# When not using CMake to link against the shared library on windows define -DBOOST_NOWIDE_DYN_LINK

cmake_minimum_required(VERSION 3.9)
# Version number starts at 10 to avoid conflicts with Boost version
set(_version 10.0.0)
if(BOOST_SUPERPROJECT_SOURCE_DIR)
  set(_version ${BOOST_SUPERPROJECT_VERSION})
endif()
project(boost_nowide VERSION ${_version} LANGUAGES CXX)

if(POLICY CMP0074)
  cmake_policy(SET CMP0074 NEW)
endif()

list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Find out if we are beeing build as standalone or boost version
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/include/nowide)
  set(BOOST_NOWIDE_STANDALONE ON)
else()
  set(BOOST_NOWIDE_STANDALONE OFF)
endif()

if(NOT CMAKE_RUNTIME_OUTPUT_DIRECTORY)
  # Make sure all binarys (especially exe/dll) are in one directory for tests to work
  set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
endif()

include(BoostAddOptions)
include(BoostAddLibrary)
include(BoostAddWarnings)
boost_add_options(nowide)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
  include(CTest)
endif()
if(BOOST_NOWIDE_SYSTEM_INCLUDE)
  set(nowideSystem SYSTEM)
else()
  set(nowideSystem "")
endif()
boost_add_library(nowide ${nowideSystem} LAYOUT ${BOOST_NOWIDE_LAYOUT})
boost_add_warnings(boost_nowide pedantic ${BOOST_NOWIDE_WERROR})

if(BOOST_NOWIDE_STANDALONE)
  target_compile_features(boost_nowide PUBLIC cxx_std_11)
else()
  if(BOOST_SUPERPROJECT_SOURCE_DIR)
    target_link_libraries(boost_nowide PUBLIC Boost::config Boost::smart_ptr Boost::static_assert)
  else()
    # Default boost libs are static on windows and dynamic on linux
    if(WIN32 AND NOT DEFINED Boost_USE_STATIC_LIBS)
      set(Boost_USE_STATIC_LIBS ON)
    endif()
    find_package(Boost 1.56 REQUIRED)
    target_link_libraries(boost_nowide PUBLIC Boost::boost)
  endif()
endif()

# Using glob here is ok as it is only for headers
file(GLOB_RECURSE headers include/*.hpp)
target_sources(boost_nowide PRIVATE src/iostream.cpp ${headers})

if(BUILD_TESTING)
  add_subdirectory(test)
endif()

if(BOOST_NOWIDE_INSTALL)
  include(BoostInstall)
  boost_install(TARGETS boost_nowide LAYOUT ${BOOST_NOWIDE_LAYOUT})
endif()
