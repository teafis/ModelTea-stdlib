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

    const auto NUMBER_TEST = std::to_array({2.0, 3.0, 4.0, 5.0, 6.1, 7.9, -10.2, -3845.5, 0.0, 2093.4});

    for (const auto& a : NUMBER_TEST) {
        block_test.s_in.vals[0] = a;

        for (const auto& b : NUMBER_TEST) {
            block_test.s_in.vals[1] = b;

            for (const auto& c : NUMBER_TEST) {
                block_test.s_in.vals[2] = c;

                block_test.step();

                const auto test_sum = a + b + c;

                REQUIRE(std::abs(block_test.s_out.val - test_sum) < 1e-6);
            }
        }
    }
}

TEST_CASE( "Block Arithmetic Add Dynamic", "[arith]" ) {
    tmdl::stdlib::arith_block_dynamic<double, tmdl::stdlib::ArithType::ADD> block_test;

    const auto SIZE_VALUES = std::to_array<size_t>({1, 2, 3, 4, 5, 6});
    const auto NUMBER_TEST = std::to_array({2.0, 3.0, 4.0, 5.0, 6.1, 7.9, -10.2, -3845.5, 0.0, 2093.4});

    for (const auto s : SIZE_VALUES) {
        std::vector<double> numbers(s, 0.0);

        block_test.s_in.size = s;
        block_test.s_in.vals = numbers.data();

        block_test.init();

        REQUIRE(std::abs(block_test.s_out.val) < 1e-6);

        bool finished = false;
        std::vector<size_t> indices(s, 0);

        while (!finished) {
            // Set Values
            double expected = 0.0;
            for (size_t i = 0; i < s; ++i) {
                block_test.s_in.vals[i] = NUMBER_TEST[indices[i]];
                expected += NUMBER_TEST[indices[i]];
            }

            // Step the model and check results
            block_test.step();
            REQUIRE(std::abs(block_test.s_out.val - expected) < 1e-6);

            // Add the permutation values
            for (size_t i = 0; i < s; ++i) {
                indices[i] += 1;

                if (indices[i] >= NUMBER_TEST.size()) {
                    indices[i] = 0;

                    auto next_i = i + 1;
                    if (next_i >= s) {
                        finished = true;
                        break;
                    }
                } else {
                    break;
                }
            }
        }
    }
}
