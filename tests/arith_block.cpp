#include <catch2/catch_all.hpp>

#include <tmdlstd/tmdlstd.hpp>

#include <cmath>

TEST_CASE( "Block Arithmetic Add", "[arith]" ) {
    tmdl::stdlib::arith_block<double, tmdl::stdlib::ArithType::ADD, 3> block_test;
    block_test.s_in.vals[0] = 1.0;
    block_test.s_in.vals[1] = 2.0;
    block_test.s_in.vals[2] = 3.0;

    block_test.init();

    REQUIRE(std::abs(block_test.s_out.val - 6.0) < 1e-6);
}
