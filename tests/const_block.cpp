// SPDX-License-Identifier: MIT

#include <catch2/catch_all.hpp>

#include <array>
#include <cstdint>
#include <limits>

#include <mtstd.hpp>


TEST_CASE("Block Const Int", "[const]") {
    using data_t = int32_t;

    const auto INT_VALUES = std::to_array<data_t>({-11, 10, 10000, 0, -3234234, 4948, std::numeric_limits<data_t>::min(), std::numeric_limits<data_t>::max()});

    for (const auto& i : INT_VALUES) {
        mt::stdlib::const_block<data_t> blk(i);
        REQUIRE(blk.s_out.value == i);
    }
}

TEST_CASE("Block Const UInt", "[const]") {
    using data_t = uint32_t;

    const auto INT_VALUES = std::to_array<data_t>({11, 10, 10000, 0, 3234234, 4948, std::numeric_limits<data_t>::min(), std::numeric_limits<data_t>::max()});

    for (const auto& i : INT_VALUES) {
        mt::stdlib::const_block<data_t> blk(i);
        REQUIRE(blk.s_out.value == i);
    }
}

TEST_CASE("Block Const Boolean", "[const]") {
    using data_t = bool;

    const auto BOOL_VALUES = std::to_array<data_t>({true, false});

    for (const auto& b : BOOL_VALUES) {
        mt::stdlib::const_block<data_t> blk(b);
        REQUIRE(blk.s_out.value == b);
    }
}

TEST_CASE("Block Const Float", "[const]") {
    using data_t = float;

    const auto FLOAT_VALUES = std::to_array<data_t>({0.0000001f, std::numeric_limits<data_t>::quiet_NaN(), 34.303f, -23494.3f, 29384.59843895f, -0.00000001f, 0.0f, -0.0f});

    for (const auto& f : FLOAT_VALUES) {
        mt::stdlib::const_block<data_t> blk(f);

        if (!std::isnan(f)) {
            REQUIRE(blk.s_out.value == f);
        } else {
            REQUIRE(std::isnan(blk.s_out.value));
        }
    }
}

TEST_CASE("Block Const Double", "[const]") {
    using data_t = double;

    const auto FLOAT_VALUES = std::to_array<data_t>({0.0000001, std::numeric_limits<data_t>::quiet_NaN(), 34.303, -23494.3, 29384.59843895, -0.00000001, 0.0, -0.0});

    for (const auto& f : FLOAT_VALUES) {
        mt::stdlib::const_block<data_t> blk(f);
        if (!std::isnan(f)) {
            REQUIRE(blk.s_out.value == f);
        } else {
            REQUIRE(std::isnan(blk.s_out.value));
        }
    }
}
