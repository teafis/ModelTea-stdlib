# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18)

set(VCPKG_FEATURE_FLAGS "versions")

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
)

# Define the library
add_library(
    tmdlstd
    ${TEAFIS_STD_MODEL_SOURCES}
)

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
