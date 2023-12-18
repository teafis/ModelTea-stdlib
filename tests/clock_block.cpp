// SPDX-License-Identifier: MIT

#include <catch2/catch_all.hpp>

#include <catch2/matchers/catch_matchers.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include <tmdlstd/tmdlstd.hpp>

static void test_with_timestamp(const double dt) {
    double check_val = 0.0;

    tmdl::stdlib::clock_block clock(dt);
    clock.init();

    REQUIRE_THAT(clock.time_step, Catch::Matchers::WithinRel(dt));
    REQUIRE_THAT(clock.s_out.val, Catch::Matchers::WithinRel(0.0));

    for (size_t i = 0; i < 1000000; ++i) {
        clock.step();
        check_val += dt;

        REQUIRE_THAT(clock.s_out.val, Catch::Matchers::WithinRel(check_val));
    }
}

TEST_CASE("Block Clock", "[arith]") {
    test_with_timestamp(10.0);
    test_with_timestamp(1.0);
    test_with_timestamp(0.1);
    test_with_timestamp(0.00001);
}
