# SPDX-License-Identifier: MIT

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
    include/mtstd.hpp
    include/mtstd_ext.hpp
    src/mtstd_ext.cpp
    include/mtstd_types.hpp
    src/mtstd_types.cpp
)

set(
    MODELTEA_STD_FULL_SOURCES
    ${MODELTEA_STD_MODEL_SOURCES}
    include/mtstd_except.hpp
    include/mtstd_string.hpp
    src/mtstd_string.cpp
    include/mtstd_compat.h
    include/mtstd_compat_types.h
    src/mtstd_compat.cpp
    include/mtstd_creation.hpp
    src/mtstd_creation.cpp
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
        tests/compat_block_list.cpp
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
