// SPDX-License-Identifier: MIT

#include <catch2/catch_all.hpp>

#include <ranges>
#include <unordered_map>

#include "mtstd_compat.h"

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
        REQUIRE(iter->display != nullptr);

        auto it = expected_names.find(iter->name);
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
