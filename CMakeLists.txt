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

