# Apache 2.0 License 
# Copyright (c) 2023 Austin Lucas Lake <53884490+austinlucaslake@users.noreply.github.com>
cmake_minimum_required(VERSION 3.23.5 FATAL_ERROR)
project(quaternion)


find_package(${META_PROJECT_NAME} REQUIRED HINTS "${CMAKE_CURRENT_SOURCE_DIR}/../")

set(target main)
message(STATUS "Test ${target}")

set(sources
    quaternion.c
    main.c
)

add_executable(${target}
    ${sources}
)

set_target_properties(${target}
    PROPERTIES
    ${DEFAULT_PROJECT_OPTIONS}
    FOLDER "${IDE_FOLDER}"
)

target_include_directories(${target}
    PRIVATE
    ${DEFAULT_INCLUDE_DIRECTORIES}
)


target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LIBRARIES}
    ${META_PROJECT_NAME}
)

target_compile_definitions(${target}
    PRIVATE
    ${DEFAULT_COMPILE_DEFINITIONS}
)

set(DEFULAT_COMPILE_OPTIONS
	"-Wall -Wextra -Wpedantic -Werror"
)
target_compile_options(${target}
    PRIVATE
    ${DEFAULT_COMPILE_OPTIONS}
)

set(DEBUG_COMPILE_OPTIONS
    "-O0"
)
target_compile_options(${target}
    PRIVATE
    "$<$<CONFIG:DEBUG>:${DEBUG_COMPILE_OPTIONS}>"
)

set(DEBUG_COMPILE_OPTIONS
    "-O3"
)
target_compile_options(${target}
    PRIVATE
    "$<$<CONFIG:RELEASE>:${RELEASE_COMPILE_OPTIONS}>"
)

target_link_libraries(${target}
    PRIVATE
    ${DEFAULT_LINKER_OPTIONS}
)
