// SPDX-License-Identifier: MIT

#include <algorithm>
#include <catch2/catch_all.hpp>

#include <memory>
#include <ranges>
#include <unordered_map>
#include <vector>

#include "mtstd_compat.h"
#include "mtstd_types.hpp"

static const std::vector<mt::stdlib::DataType> DT_NUM = {
    mt::stdlib::DataType::F32,
    mt::stdlib::DataType::F64,
    mt::stdlib::DataType::U8,
    mt::stdlib::DataType::U16,
    mt::stdlib::DataType::U32,
    mt::stdlib::DataType::I8,
    mt::stdlib::DataType::I16,
    mt::stdlib::DataType::I32,
};

static const std::vector<mt::stdlib::DataType> DT_ALL_VALID = {
    mt::stdlib::DataType::F32,
    mt::stdlib::DataType::F64,
    mt::stdlib::DataType::U8,
    mt::stdlib::DataType::U16,
    mt::stdlib::DataType::U32,
    mt::stdlib::DataType::I8,
    mt::stdlib::DataType::I16,
    mt::stdlib::DataType::I32,
    mt::stdlib::DataType::BOOL,
};

static const std::vector<mt::stdlib::DataType> DT_ALL = {
    mt::stdlib::DataType::F32,
    mt::stdlib::DataType::F64,
    mt::stdlib::DataType::U8,
    mt::stdlib::DataType::U16,
    mt::stdlib::DataType::U32,
    mt::stdlib::DataType::I8,
    mt::stdlib::DataType::I16,
    mt::stdlib::DataType::I32,
    mt::stdlib::DataType::BOOL,
    mt::stdlib::DataType::NONE,
};


static const std::vector<mt::stdlib::DataType> DT_FLOATING = {
    mt::stdlib::DataType::F32,
    mt::stdlib::DataType::F64,
};

TEST_CASE("Test Block List", "[compat]") {
    auto init_vals = mt_stdlib_info_init();

    REQUIRE(init_vals != nullptr);

    std::unordered_map<std::string, bool> expected_names = {
        {"arith_add", false},
        {"arith_sub", false},
        {"arith_mul", false},
        {"arith_div", false},
        {"arith_mod", false},
        {"clock", false},
        {"constant", false},
        {"delay", false},
        {"derivative", false},
        {"integrator", false},
        {"switch", false},
        {"limiter", false},
        {"rel_greater", false},
        {"rel_greater_eq", false},
        {"rel_less", false},
        {"rel_less_eq", false},
        {"rel_equal", false},
        {"rel_not_equal", false},
        {"trig_sin", false},
        {"trig_cos", false},
        {"trig_tan", false},
        {"trig_asin", false},
        {"trig_acos", false},
        {"trig_atan", false},
        {"trig_atan2", false},
    };

    auto iter = init_vals;
    size_t i = 0;
    while (iter != nullptr) {
        REQUIRE(iter != nullptr);
        REQUIRE(iter->name != nullptr);
        REQUIRE(iter->sub_name != nullptr);

        std::ostringstream oss;
        oss << iter->name;
        if (!std::string(iter->sub_name).empty())
        {
            oss << '_' << iter->sub_name;
        }
        const std::string combined_name = oss.str();

        auto it = expected_names.find(combined_name);
        REQUIRE(it != expected_names.end());
        REQUIRE(it->second == false);

        it->second = true;

        iter = iter->next;
        i += 1;

        REQUIRE(i < 2000);
    }

    mt_stdlib_info_destroy(init_vals);

    for (const auto& found : expected_names | std::ranges::views::values) {
        REQUIRE(found == true);
    }
}

#include <iostream>

TEST_CASE("Data Type Model Check", "[compat]") {
    auto init_vals = mt_stdlib_info_init();
    REQUIRE(init_vals != nullptr);

    auto iter = init_vals;
    size_t i = 0;

    while (iter != nullptr) {
        const int32_t creation_flags = iter->create_flags;
        const int32_t type_flags = iter->type_flags;

        std::vector<mt::stdlib::DataType> dtypes;

        if (MT_TYPE_FLAGS_FLOAT == type_flags) {
            dtypes = DT_FLOATING;
        } else if (MT_TYPE_FLAGS_NUMERIC == type_flags) {
            dtypes = DT_NUM;
        } else if (MT_TYPE_FLAGS_ALL == type_flags) {
            dtypes = DT_ALL_VALID;
        }

        REQUIRE(dtypes.size() > 0);

        for (const auto& dt : DT_ALL) {
            mt_block_t* blk;
            const auto dt_int = static_cast<uint32_t>(dt);

            if (MT_INIT_SIZE == creation_flags) {
                blk = mt_stdlib_blk_create_with_size(iter->name, iter->sub_name, dt_int, 3);
            } else if (MT_INIT_VALUE == creation_flags) {
                auto data_ptr = std::unique_ptr<uint8_t[]>(new uint8_t[mt_stdlib_type_size(dt_int)]);
                mt_value_t value {
                    .type = dt_int,
                    .size = mt_stdlib_type_size(dt_int),
                    .data = data_ptr.get()
                };
                blk = mt_stdlib_blk_create_with_value(iter->name, iter->sub_name, &value);
            } else if (MT_INIT_DEFAULT == creation_flags) {
                blk = mt_stdlib_blk_create(iter->name, iter->sub_name, dt_int);
            } else {
                REQUIRE(false);
            }

            // Ensure that the block gets created correctly
            if (std::find(dtypes.begin(), dtypes.end(), dt) != dtypes.end()) {
                REQUIRE(blk != nullptr);
            } else {
                if (blk != nullptr)
                {
                    std::cerr << iter->name << "::" << iter->sub_name << std::endl;
                }
                REQUIRE(blk == nullptr);
            }

            // Destory the block if needed
            if (blk != nullptr) {
                mt_stdlib_blk_destroy(blk);
            }
        }

        iter = iter->next;
        i += 1;
        REQUIRE(i < 2000);
    }

    mt_stdlib_info_destroy(init_vals);
}

TEST_CASE("Arith Block Creation", "[compat]") {
    std::vector<std::string> arith_blocks{
        "add",
        "sub",
        "mul",
        "div",
        "mod",
    };

    for (const auto& name : arith_blocks) {
        for (const auto& dt : DT_NUM) {
            const auto dt_int = static_cast<uint32_t>(dt);
            auto mdl = mt_stdlib_blk_create_with_size("arith", name.c_str(), dt_int, 2);

            REQUIRE(mdl != nullptr);

            mt_stdlib_blk_destroy(mdl);
        }
    }
}
