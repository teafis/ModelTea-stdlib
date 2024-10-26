// SPDX-License-Identifier: MIT

#include <catch2/catch_all.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "mtea.hpp"
#include "mtea_types.hpp"

#include <ranges>

const auto TEST_SIZES = std::to_array<size_t>({1, 2, 3, 4, 5, 6});
const auto TEST_NUMBERS =
    std::to_array({2.0, 3.0, 4.0, 5.0, 6.1, 7.9, -10.2, -3845.5, 0.0, 2093.4});
const auto TEST_NUMBERS_NON_ZERO =
    std::to_array({2.0, 3.0, 4.0, 5.0, 6.1, 7.9, -10.2, -3845.5, -0.5, 2093.4});

template <mtea::ArithType OP>
static double compute_expected(std::span<const double> vals) {
    double (*reduce_fn)(double, double) = nullptr;

    if constexpr (OP == mtea::ArithType::ADD) {
        reduce_fn = [](double a, double b) { return a + b; };
    } else if constexpr (OP == mtea::ArithType::SUB) {
        reduce_fn = [](double a, double b) { return a - b; };
    } else if constexpr (OP == mtea::ArithType::MUL) {
        reduce_fn = [](double a, double b) { return a * b; };
    } else if constexpr (OP == mtea::ArithType::DIV) {
        reduce_fn = [](double a, double b) { return a / b; };
    } else if constexpr (OP == mtea::ArithType::MOD) {
        reduce_fn = [](double a, double b) { return std::fmod(a, b); };
    } else {
        static_assert(false, "unsupported operation");
    }

    REQUIRE(vals.size() > 0);

    double result = vals[0];
    for (const auto& v : vals | std::views::drop(1)) {
        result = reduce_fn(result, v);
    }

    return result;
}

template <mtea::ArithType OP>
static void test_static_block(std::span<const double> test_values) {
    mtea::arith_block<mtea::DataType::F64, OP, 3> block_test;

    block_test.s_in.values[0] = 1.0;
    block_test.s_in.values[1] = 2.0;
    block_test.s_in.values[2] = 3.0;

    block_test.reset();

    std::vector<double> init_values(
        block_test.s_in.values, block_test.s_in.values + block_test.s_in.size);

    REQUIRE_THAT(block_test.s_out.value,
                 Catch::Matchers::WithinRel(compute_expected<OP>(init_values)));

    for (const auto& a : test_values) {
        block_test.s_in.values[0] = a;

        for (const auto& b : test_values) {
            block_test.s_in.values[1] = b;

            for (const auto& c : test_values) {
                block_test.s_in.values[2] = c;

                block_test.step();

                const auto expected =
                    compute_expected<OP>(std::to_array({a, b, c}));
                REQUIRE_THAT(block_test.s_out.value,
                             Catch::Matchers::WithinRel(expected));
            }
        }
    }
}

template <mtea::ArithType OP>
static void test_dynamic_block(std::span<const size_t> test_sizes,
                               std::span<const double> test_values) {
    mtea::arith_block_dynamic<mtea::DataType::F64, OP> block_test;

    for (const auto s : test_sizes) {
        std::vector<double> numbers(s, 0.0);

        block_test.s_in.size = s;
        block_test.s_in.values = numbers.data();

        block_test.reset();

        if constexpr (OP != mtea::ArithType::DIV && OP != mtea::ArithType::MOD) {
            REQUIRE_THAT(block_test.s_out.value, Catch::Matchers::WithinRel(0.0));
        }

        bool finished = false;
        std::vector<size_t> indices(s, 0);

        while (!finished) {
            // Set Values
            std::vector<double> used_values(s, 0.0);
            for (size_t i = 0; i < s; ++i) {
                block_test.s_in.values[i] = test_values[indices[i]];
                used_values[i] = test_values[indices[i]];
            }
            const double expected = compute_expected<OP>(used_values);

            // Step the model and check results
            block_test.step();
            REQUIRE_THAT(block_test.s_out.value,
                         Catch::Matchers::WithinRel(expected));

            // Add the permutation values
            for (size_t i = 0; i < s; ++i) {
                indices[i] += 1;

                if (indices[i] >= test_values.size()) {
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

TEST_CASE("Block Arithmetic Add", "[arith]") {
    test_static_block<mtea::ArithType::ADD>(TEST_NUMBERS);
    test_static_block<mtea::ArithType::ADD>(TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Add Dynamic", "[arith]") {
    test_dynamic_block<mtea::ArithType::ADD>(TEST_SIZES, TEST_NUMBERS);
    test_dynamic_block<mtea::ArithType::ADD>(TEST_SIZES,
                                                     TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Sub", "[arith]") {
    test_static_block<mtea::ArithType::SUB>(TEST_NUMBERS);
    test_static_block<mtea::ArithType::SUB>(TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Sub Dynamic", "[arith]") {
    test_dynamic_block<mtea::ArithType::SUB>(TEST_SIZES, TEST_NUMBERS);
    test_dynamic_block<mtea::ArithType::SUB>(TEST_SIZES,
                                                     TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Mul", "[arith]") {
    test_static_block<mtea::ArithType::MUL>(TEST_NUMBERS);
    test_static_block<mtea::ArithType::MUL>(TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Mul Dynamic", "[arith]") {
    test_dynamic_block<mtea::ArithType::MUL>(TEST_SIZES, TEST_NUMBERS);
    test_dynamic_block<mtea::ArithType::MUL>(TEST_SIZES,
                                                     TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Div", "[arith]") {
    test_static_block<mtea::ArithType::DIV>(TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Div Dynamic", "[arith]") {
    test_dynamic_block<mtea::ArithType::DIV>(TEST_SIZES,
                                                     TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Mod", "[arith]") {
    test_static_block<mtea::ArithType::MOD>(TEST_NUMBERS_NON_ZERO);
}

TEST_CASE("Block Arithmetic Mod Dynamic", "[arith]") {
    test_dynamic_block<mtea::ArithType::MOD>(TEST_SIZES,
                                                     TEST_NUMBERS_NON_ZERO);
}
