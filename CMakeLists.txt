# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18)

set(VCPKG_FEATURE_FLAGS "versions")

option(TMDLSTD_USE_DEFAULT_MATH_FUNCS "Compile in default functions for special mathematical operations like trig and arithmetic operators" ON)
option(TMDLSTD_USE_STRING_FUNCS "Compile in string functions for conversion from enums to strings" ON)

# Define the project information
project (
    tmdlstd
    VERSION 0.1
    LANGUAGES CXX
)

set(
    TEAFIS_STD_MODEL_SOURCES
    src/tmdlstd/tmdlstd.hpp
    src/tmdlstd/tmdlstd.cpp
    src/tmdlstd/tmdlstd_ext.hpp
    src/tmdlstd/tmdlstd_string.hpp
    src/tmdlstd/tmdlstd_string.cpp
)

if(TMDLSTD_USE_DEFAULT_MATH_FUNCS)
    list(APPEND
        TEAFIS_STD_MODEL_SOURCES
        src/tmdlstd/tmdlstd_ext.cpp)
endif()

if(TMDLSTD_USE_STRING_FUNCS)
    list(APPEND
        TEAFIS_STD_MODEL_SOURCES
        src/tmdlstd/tmdlstd_string.hpp
        src/tmdlstd/tmdlstd_string.cpp)
endif()

# Define the library
add_library(
    tmdlstd
    ${TEAFIS_STD_MODEL_SOURCES}
)

if(TMDLSTD_USE_STRING_FUNCS)
    target_link_libraries(tmdlstd PUBLIC fmt::fmt)
endif()

# Define the C++ Standard Required
set_property(TARGET tmdlstd PROPERTY CXX_STANDARD 20)
set_property(TARGET tmdlstd PROPERTY CXX_STANDARD_REQUIRED ON)

# Include Directories
target_include_directories(tmdlstd PUBLIC src)

# Set appropriate catch libraries

add_executable(
    tmdlstd_test
    tests/arith_block.cpp
    tests/clock_block.cpp)

set_property(TARGET tmdlstd_test PROPERTY CXX_STANDARD 20)
set_property(TARGET tmdlstd_test PROPERTY CXX_STANDARD_REQUIRED ON)

target_link_libraries(tmdlstd_test PRIVATE tmdlstd)

find_package(Catch2 CONFIG REQUIRED)
target_link_libraries(tmdlstd_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

add_test(NAME tmdlstd_test COMMAND tmdlstd_test)
