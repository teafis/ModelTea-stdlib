﻿# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18)

set(VCPKG_FEATURE_FLAGS "versions")

# Define the project information
project (
    mtstdlib
    VERSION 0.1
    LANGUAGES CXX
)

set(
    MODELTEA_STD_MODEL_SOURCES
    include/mtstdlib.hpp
    include/mtstdlib_ext.hpp
    src/mtstdlib_ext.cpp
    include/mtstdlib_types.hpp
    src/mtstdlib_types.cpp
)

set(
    MODELTEA_STD_FULL_SOURCES
    ${MODELTEA_STD_MODEL_SOURCES}
    include/mtstdlib_except.hpp
    include/mtstdlib_string.hpp
    include/mtstdlib_string_common.hpp
    src/mtstdlib_string.cpp
    include/mtstdlib_creation.hpp
    src/mtstdlib_creation.cpp
)

# Define the library
add_library(
    mtstdlib
    ${MODELTEA_STD_FULL_SOURCES}
)

add_library(
    mtstdlib_rt
    ${MODELTEA_STD_MODEL_SOURCES}
)

# Define the C++ Standard Required
set_property(TARGET mtstdlib PROPERTY CXX_STANDARD 20)
set_property(TARGET mtstdlib PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET mtstdlib_rt PROPERTY CXX_STANDARD 11)
set_property(TARGET mtstdlib_rt PROPERTY CXX_STANDARD_REQUIRED ON)

# Include Directories
target_include_directories(mtstdlib PUBLIC include)
target_include_directories(mtstdlib_rt PUBLIC include)

# Add include parameter if needed
target_compile_definitions(mtstdlib PUBLIC MT_STDLIB_USE_FULL_LIB)

# Set appropriate catch libraries

if(CMAKE_TESTING_ENABLED)
    find_package(Catch2 CONFIG REQUIRED)

    set(
        MTSTD_TEST_FILES
        tests/block_arith.cpp
        tests/block_clock.cpp
        tests/block_const.cpp
        )

    add_executable(
        mtstd_test
        ${MTSTD_TEST_FILES}
        )

    set_property(TARGET mtstd_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET mtstd_test PROPERTY CXX_STANDARD_REQUIRED ON)

    target_link_libraries(mtstd_test PRIVATE mtstdlib)
    target_link_libraries(mtstd_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

    add_test(NAME mtstd_test COMMAND mtstd_test)

    add_executable(
        mtstd_test_rt
        ${MTSTD_TEST_FILES}
        )

    set_property(TARGET mtstd_test_rt PROPERTY CXX_STANDARD 20)
    set_property(TARGET mtstd_test_rt PROPERTY CXX_STANDARD_REQUIRED ON)

    target_link_libraries(mtstd_test_rt PRIVATE mtstdlib_rt)

    target_link_libraries(mtstd_test_rt PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

    add_test(NAME mtstd_test_rt COMMAND mtstd_test_rt)
endif()
