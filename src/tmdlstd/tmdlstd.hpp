// SPDX-License-Identifier: MIT

#ifndef TMDL_STDLIB_H
#define TMDL_STDLIB_H

#include <array>
#include <stdexcept>

#include "tmdlstd_ext.hpp"

namespace tmdl::stdlib {

class block_error : public std::runtime_error {
public:
    block_error(const char* msg) : std::runtime_error(msg) {}
    block_error(const std::string& msg) : std::runtime_error(msg) {}
};

enum class ArithType {
    ADD = 0,
    SUB,
    MUL,
    DIV,
    MOD,
};

template <typename T, ArithType AT>
struct arith_block_dynamic {
    struct input_t {
        T* values;
        int size;
    };

    struct output_t {
        T val;
    };

    arith_block_dynamic() = default;
    arith_block_dynamic(const arith_block_dynamic&) = delete;
    arith_block_dynamic& operator=(const arith_block_dynamic&) = delete;

    void reset() { step(); }

    void step() {
        T val = s_in.values[0];

        for (int i = 1; i < s_in.size; ++i) {
            using enum ArithType;
            const auto& v = s_in.values[i];

            if constexpr (AT == ADD) {
                val += v;
            } else if constexpr (AT == SUB) {
                val -= v;
            } else if constexpr (AT == MUL) {
                val *= v;
            } else if constexpr (AT == DIV) {
                val /= v;
            } else if constexpr (AT == MOD) {
                val = t_mod(val, v);
            } else {
                static_assert(false, "unsuppored arithmetic type provided");
            }
        }

        s_out.val = val;
    }

    input_t s_in;
    output_t s_out;
};

template <typename T, ArithType AT, int SIZE>
struct arith_block : public arith_block_dynamic<T, AT> {
    arith_block() {
        this->s_in.size = SIZE;
        this->s_in.values = _input_array.data();
    }

    arith_block(const arith_block&) = delete;
    arith_block& operator=(const arith_block&) = delete;

private:
    std::array<T, SIZE> _input_array;
};

struct clock_block {
    struct output_t {
        double val;
    };

    explicit clock_block(double dt);

    clock_block(const clock_block&) = delete;
    clock_block& operator=(const clock_block&) = delete;

    void reset();

    void step();

    output_t s_out;

    const double time_step;
};

template <typename T>
struct const_block {
    struct output_t {
        T value;
    };

    explicit const_block(const T val) : s_out{.value = val} {
        // Empty Constructor
    }

    const_block(const const_block&) = delete;
    const_block& operator=(const const_block&) = delete;

    const output_t s_out;
};

template <typename T>
struct delay_block {
    struct input_t {
        T value;
        T reset;
        bool reset_flag;
    };

    struct output_t {
        T value;
    };

    delay_block() = default;
    delay_block(const delay_block&) = delete;
    delay_block& operator=(const delay_block&) = delete;

    void reset() {
        next_value = s_in.reset;
        s_out.value = s_in.reset;
    }

    void step() {
        if (s_in.reset_flag) {
            reset();
        }

        s_out.value = next_value;
        next_value = s_in.value;
    }

    input_t s_in;
    output_t s_out;

    T next_value;
};

template <typename T>
struct derivative_block {
    struct input_t {
        T value;
        bool reset_flag;
    };

    struct output_t {
        T value;
    };

    explicit derivative_block(const double dt) : time_step(dt) {
        // Empty Constructor
    }

    derivative_block(const derivative_block&) = delete;
    derivative_block& operator=(const derivative_block&) = delete;

    void reset() {
        last_value = {};
        s_out.value = {};
    }

    void step() {
        if (s_in.reset_flag) {
            reset();
        }

        s_out.value = (s_in.value - last_value) / time_step;
        last_value = s_in.value;
    }

    input_t s_in;
    output_t s_out;

    T last_value;
    const double time_step;
};

template <typename T>
struct integrator_block {
    struct input_t {
        T value;
        T reset;
        bool reset_flag;
    };

    struct output_t {
        T value;
    };

    explicit integrator_block(const double dt) : time_step(dt) {
        // Empty Constructor
    }

    integrator_block(const integrator_block&) = delete;
    integrator_block& operator=(const integrator_block&) = delete;

    void reset() { s_out.value = s_in.reset; }

    void step() {
        if (s_in.reset_flag) {
            reset();
        } else {
            s_out.value += s_in.value * time_step;
        }
    }

    input_t s_in;
    output_t s_out;

    const double time_step;
};

template <typename T>
struct switch_block {
    struct input_t {
        bool value_flag;
        T value_a;
        T value_b;
    };

    struct output_t {
        T value;
    };

    switch_block() = default;
    switch_block(const switch_block&) = delete;
    switch_block& operator=(const switch_block&) = delete;

    void reset() { step(); }

    void step() {
        if (s_in.value_flag) {
            s_out.value = s_in.value_a;
        } else {
            s_out.value = s_in.value_b;
        }
    }

    input_t s_in;
    output_t s_out;
};

template <typename T>
struct limiter_block {
    struct input_t {
        T value;
        T limit_upper;
        T limit_lower;
    };

    struct output_t {
        T value;
    };

    limiter_block() = default;
    limiter_block(const limiter_block&) = delete;
    limiter_block& operator=(const limiter_block&) = delete;

    void reset() { step(); }

    void step() {
        T x = s_in.value;

        if (x < s_in.limit_lower) {
            x = s_in.limit_lower;
        } else if (x > s_in.limit_upper) {
            x = s_in.limit_upper;
        }

        s_out.value = x;
    }

    input_t s_in;
    output_t s_out;
};

template <typename T>
struct limiter_block_const {
    struct input_t {
        T value;
    };

    struct output_t {
        T value;
    };

    limiter_block_const(const T upper, const T lower)
        : bound_upper{upper}, bound_lower{lower} {
        if (upper < lower) {
            throw block_error("upper bound must be >= lower bound");
        }
    }

    limiter_block_const(const limiter_block_const&) = delete;
    limiter_block_const& operator=(const limiter_block_const&) = delete;

    void reset() { step(); }

    void step() {
        T x = s_in.input_value;

        if (x < bound_lower) {
            x = bound_lower;
        } else if (x > bound_upper) {
            x = bound_upper;
        }

        s_out.output_value = x;
    }

    input_t s_in;
    output_t s_out;

    const T bound_upper;
    const T bound_lower;
};

enum class RelationalOperator {
    EQUAL = 0,
    NOT_EQUAL,
    GREATER_THAN,
    GREATER_THAN_EQUAL,
    LESS_THAN,
    LESS_THAN_EQUAL,
};

template <typename T, RelationalOperator OP>
struct relational_block {
    struct input_t {
        T value_a;
        T value_b;
    };

    struct output_t {
        bool output_value;
    };

    relational_block() = default;
    relational_block(const relational_block&) = delete;
    relational_block& operator=(const relational_block&) = delete;

    void reset() { step(); }

    void step() {
        using enum RelationalOperator;

        bool v = false;
        const T a = s_in.value_a;
        const T b = s_in.value_b;

        if constexpr (OP == EQUAL) {
            v = a == b;
        } else if constexpr (OP == NOT_EQUAL) {
            v = a != b;
        } else if constexpr (OP == GREATER_THAN) {
            v = a > b;
        } else if constexpr (OP == GREATER_THAN_EQUAL) {
            v = a >= b;
        } else if constexpr (OP == LESS_THAN) {
            v = a < b;
        } else if constexpr (OP == LESS_THAN_EQUAL) {
            v = a <= b;
        }

        s_out.output_value = v;
    }

    input_t s_in;
    output_t s_out;
};

enum class TrigFunction {
    SIN = 0,
    COS,
    TAN,
    ASIN,
    ACOS,
    ATAN,
    ATAN2,
};

template <TrigFunction FCN>
struct TrigInfo {
    static const size_t input_count = 1;
};

template <>
struct TrigInfo<TrigFunction::ATAN2> {
    static const size_t input_count = 2;
};

template <typename T, TrigFunction FCN>
struct trig_block {
    struct input_t {
        T values[TrigInfo<FCN>::input_count];
    };

    struct output_t {
        T value;
    };

    trig_block() = default;
    trig_block(const trig_block&) = delete;
    trig_block& operator=(const trig_block&) = delete;

    void reset() { step(); }

    void step() {
        static_assert(TrigInfo<FCN>::input_count > 0);

        T y{};
        const T x = s_in.values[0];

        if constexpr (FCN == TrigFunction::SIN) {
            y = t_sin(x);
        } else if constexpr (FCN == TrigFunction::COS) {
            y = t_cos(x);
        } else if constexpr (FCN == TrigFunction::TAN) {
            y = t_tan(x);
        } else if constexpr (FCN == TrigFunction::ASIN) {
            y = t_asin(x);
        } else if constexpr (FCN == TrigFunction::ACOS) {
            y = t_acos(x);
        } else if constexpr (FCN == TrigFunction::ATAN) {
            y = t_atan(x);
        } else if constexpr (FCN == TrigFunction::ATAN2) {
            y = t_atan2(x, s_in.values[1]);
        } else {
            static_assert(false, "unsupported trig function");
        }

        s_out.value = y;
    }

    input_t s_in;
    output_t s_out;
};

}

#endif // TMDL_STDLIB_H
