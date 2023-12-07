# Apache-2.0 License 
# Copyright (c) 2023 Austin Lake <53884490+austinlucaslake@users.noreply.github.com>
#
# Main CMake Project file
cmake_minimum_required(VERSION 3.15.0 FATAL_ERROR)

cmake_policy(SET CMP0063 NEW)
cmake_policy(SET CMP0091 NEW)
project(
	donut
	VERSION 1.1.3
	LANGUAGES C
	DESCRIPTION "Quaternion rotations rendered using ASCII characters.")

set(CMAKE_C_STANDARD 17)
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

add_compile_options(
  "-Wall" "-Wpedantic" "-Wextra"
)

include(CTest)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(InstallRequiredSystemLibraries)
include(GenerateExportHeader)

add_subdirectory(src)
