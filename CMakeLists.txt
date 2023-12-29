# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18)

set(VCPKG_FEATURE_FLAGS "versions")

option(MTSTD_USE_STRING_FUNCS "Compile in string functions for conversion from enums to strings" ON)
option(MTSTD_USE_C_COMPAT "Compile in a C-level compatibility library" ON)

# Define the project information
project (
    mtstdlib
    VERSION 0.1
    LANGUAGES CXX
)

set(
    MODELTEA_STD_MODEL_SOURCES
    include/mtstd.hpp
    src/mtstd.cpp
    include/mtstd_ext.hpp
    src/mtstd_ext.cpp
    include/mtstd_types.hpp
    src/mtstd_types.cpp
)

if(MTSTD_USE_STRING_FUNCS)
    list(APPEND
        MODELTEA_STD_MODEL_SOURCES
        include/mtstd_except.hpp
        include/mtstd_string.hpp
        src/mtstd_string.cpp)
endif()

if(MTSTD_USE_C_COMPAT)
    list(APPEND
        MODELTEA_STD_MODEL_SOURCES
        include/mtstd_compat.h
        src/mtstd_compat.cpp)
endif()

# Define the library
add_library(
    mtstdlib
    ${MODELTEA_STD_MODEL_SOURCES}
)

# Define the C++ Standard Required
set_property(TARGET mtstdlib PROPERTY CXX_STANDARD 11)
set_property(TARGET mtstdlib PROPERTY CXX_STANDARD_REQUIRED ON)

# Include Directories
target_include_directories(mtstdlib PUBLIC include)

# Add include parameter if needed
if(MTSTD_USE_STRING_FUNCS)
    target_compile_definitions(mtstdlib PUBLIC MT_STDLIB_USE_STRING_FUNCS)
endif()

if(MTSTD_USE_C_COMPAT)
    target_compile_definitions(mtstdlib PUBLIC MT_USE_C_COMPAT)
endif()

# Set appropriate catch libraries

if(CMAKE_TESTING_ENABLED)
    add_executable(
        mtstd_test
        tests/arith_block.cpp
        tests/clock_block.cpp
        tests/const_block.cpp)

    set_property(TARGET mtstd_test PROPERTY CXX_STANDARD 20)
    set_property(TARGET mtstd_test PROPERTY CXX_STANDARD_REQUIRED ON)

    target_link_libraries(mtstd_test PRIVATE mtstdlib)

    find_package(Catch2 CONFIG REQUIRED)
    target_link_libraries(mtstd_test PRIVATE Catch2::Catch2 Catch2::Catch2WithMain)

    add_test(NAME mtstd_test COMMAND mtstd_test)
endif()
