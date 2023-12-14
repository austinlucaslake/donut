# Apache 2.0 License 
# Copyright (c) 2023 Austin Lucas Lake <53884490+austinlucaslake@users.noreply.github.com>
cmake_minimum_required(VERSION 3.23.5 FATAL_ERROR)
project(
    quaternion
    VERSION 1.2.1
    LANGUAGES C
    DESCRIPTION "Quaternion rotations rendered using ASCII characters."
)

cmake_policy(SET CMP0063 NEW)
cmake_policy(SET CMP0091 NEW)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED TRUE)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_VISIBILITY_PRESET hidden)

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_SKIP_BUILD_RPATH FALSE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/install)
set(CMAKE_MODULE_PATH ${CMAKE_BINARY_DIR} ${CMAKE_MODULE_PATH})
set(CMAKE_PREFIX_PATH ${CMAKE_BINARY_DIR} ${CMAKE_PREFIX_PATH})

option(BUILD_TESTING "Build tests" FALSE)
option(BUILD_SHARED_LIBS "Build shared libraries" FALSE)
option(BUILD_WITH_MT "Build libraries as MultiThreaded DLL (Windows Only)" FALSE)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(InstallRequiredSystemLibraries)
include(GenerateExportHeader)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
  include(CTest)
  find_package(Doxygen)
  if(Doxygen_FOUND)
    add_subdirectory(docs)
  else()
    message(STATUS "Doxygen not found, not building docs")
  endif()
endif()

add_library(quaternion src/quaternion.c)
target_include_directories(quaternion PUBLIC include)
set_target_properties(quaternion PROPERTIES PUBLIC_HEADER "include/quaternion.h")

add_subdirectory(src)
add_subdirectory(test)
