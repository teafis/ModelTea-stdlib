// SPDX-License-Identifier: MIT

#include <catch2/catch_all.hpp>

#include "mtea.hpp"
#include "mtea_types.hpp"

#include <array>
#include <limits>



TEST_CASE("Block Const Int", "[const]") {
    const mtea::DataType DT = mtea::DataType::I32;
    using data_t = mtea::type_info<DT>::type_t;

    const auto INT_VALUES = std::to_array<mtea::type_info<DT>::type_t>({-11, 10, 10000, 0, -3234234, 4948, std::numeric_limits<data_t>::min(), std::numeric_limits<data_t>::max()});

    for (const auto& i : INT_VALUES) {
        mtea::const_block<DT> blk(i);
        REQUIRE(blk.s_out.value == i);
    }
}

TEST_CASE("Block Const UInt", "[const]") {
    const mtea::DataType DT = mtea::DataType::U32;
    using data_t = mtea::type_info<DT>::type_t;

    const auto INT_VALUES = std::to_array<mtea::type_info<DT>::type_t>({11, 10, 10000, 0, 3234234, 4948, std::numeric_limits<data_t>::min(), std::numeric_limits<data_t>::max()});

    for (const auto& i : INT_VALUES) {
        mtea::const_block<DT> blk(i);
        REQUIRE(blk.s_out.value == i);
    }
}

TEST_CASE("Block Const Boolean", "[const]") {
    const mtea::DataType DT = mtea::DataType::BOOL;
    using data_t = mtea::type_info<DT>::type_t;

    const auto BOOL_VALUES = std::to_array<mtea::type_info<DT>::type_t>({true, false});

    for (const auto& b : BOOL_VALUES) {
        mtea::const_block<DT> blk(b);
        REQUIRE(blk.s_out.value == b);
    }
}

TEST_CASE("Block Const Float", "[const]") {
    const mtea::DataType DT = mtea::DataType::F32;
    using data_t = mtea::type_info<DT>::type_t;

    const auto FLOAT_VALUES = std::to_array<mtea::type_info<DT>::type_t>({0.0000001f, std::numeric_limits<data_t>::quiet_NaN(), 34.303f, -23494.3f, 29384.59843895f, -0.00000001f, 0.0f, -0.0f});

    for (const auto& f : FLOAT_VALUES) {
        mtea::const_block<DT> blk(f);

        if (!std::isnan(f)) {
            REQUIRE(blk.s_out.value == f);
        } else {
            REQUIRE(std::isnan(blk.s_out.value));
        }
    }
}

TEST_CASE("Block Const Double", "[const]") {
    const mtea::DataType DT = mtea::DataType::F64;
    using data_t = mtea::type_info<DT>::type_t;

    const auto FLOAT_VALUES = std::to_array<mtea::type_info<DT>::type_t>({0.0000001, std::numeric_limits<data_t>::quiet_NaN(), 34.303, -23494.3, 29384.59843895, -0.00000001, 0.0, -0.0});

    for (const auto& f : FLOAT_VALUES) {
        mtea::const_block<DT> blk(f);
        if (!std::isnan(f)) {
            REQUIRE(blk.s_out.value == f);
        } else {
            REQUIRE(std::isnan(blk.s_out.value));
        }
    }
}
