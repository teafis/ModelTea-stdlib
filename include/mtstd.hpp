// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_H
#define MT_STDLIB_H

#include <array>
#include <cstddef>
#include <sstream>

#include "mtstd_ext.hpp"
#include "mtstd_types.hpp"

#ifdef MT_STDLIB_USE_STRING_FUNCS
#include "mtstd_string.hpp"
#include <sstream>
#include <string>
#endif

// TODO - Add string nameof-type function?

namespace mt {
namespace stdlib {

#if MT_STDLIB_USE_STRING_FUNCS
extern const std::string BASE_NAMESPACE;
#endif

#ifdef MT_USE_C_COMPAT
#define MT_COMPAT_SUBCLASS : public block_interface
#define MT_COMPAT_OVERRIDE override
#else
#define MT_COMPAT_SUBCLASS
#define MT_COMPAT_OVERRIDE
#endif

template <typename T, ArithType AT>
struct ArithOperation {};

template <typename T>
struct ArithOperation<T, ArithType::ADD> {
    static T operation(const T a, const T b) {
        return a + b;
    }
};

template <typename T>
struct ArithOperation<T, ArithType::SUB> {
    static T operation(const T a, const T b) {
        return a - b;
    }
};

template <typename T>
struct ArithOperation<T, ArithType::MUL> {
    static T operation(const T a, const T b) {
        return a * b;
    }
};

template <typename T>
struct ArithOperation<T, ArithType::DIV> {
    static T operation(const T a, const T b) {
        return a / b;
    }
};

template <typename T>
struct ArithOperation<T, ArithType::MOD> {
    static T operation(const T a, const T b) {
        return t_mod(a, b);
    }
};

template <typename T, ArithType AT>
struct arith_block_dynamic MT_COMPAT_SUBCLASS {
    struct input_t {
        T* values;
        int size;
    };

    struct output_t {
        T value;
    };

    arith_block_dynamic() = default;
    arith_block_dynamic(const arith_block_dynamic&) = delete;
    arith_block_dynamic& operator=(const arith_block_dynamic&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        T val = s_in.values[0];

        for (int i = 1; i < s_in.size; ++i) {
            val = ArithOperation<T, AT>::operation(val, s_in.values[i]);
        }

        s_out.value = val;
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "arith_block_dynamic<" << type_info<T>::name << ", " << arith_to_string(AT) << ">";
        return oss.str();
    }
#endif

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

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "arith_block<" << type_info<T>::name << ", " << arith_to_string(AT) << ", " << SIZE << ">";
        return oss.str();
    }
#endif

private:
    std::array<T, SIZE> _input_array;
};

struct clock_block MT_COMPAT_SUBCLASS {
    struct output_t {
        double val;
    };

    explicit clock_block(double dt);

    clock_block(const clock_block&) = delete;
    clock_block& operator=(const clock_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE;

    void step() MT_COMPAT_OVERRIDE;

    output_t s_out;

    const double time_step;

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        return "clock_block";
    }
#endif
};

template <typename T>
struct const_block MT_COMPAT_SUBCLASS {
    struct output_t {
        T value;
    };

    explicit const_block(const T val) : s_out{.value = val} {
        // Empty Constructor
    }

    const_block(const const_block&) = delete;
    const_block& operator=(const const_block&) = delete;

    const output_t s_out;

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "const_block<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif
};

template <typename T>
struct delay_block MT_COMPAT_SUBCLASS {
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

    void reset() MT_COMPAT_OVERRIDE {
        next_value = s_in.reset;
        s_out.value = s_in.reset;
    }

    void step() MT_COMPAT_OVERRIDE {
        if (s_in.reset_flag) {
            reset();
        }

        s_out.value = next_value;
        next_value = s_in.value;
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "delay_block<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;

    T next_value;
};

template <typename T>
struct derivative_block MT_COMPAT_SUBCLASS {
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

    void reset() MT_COMPAT_OVERRIDE {
        last_value = {};
        s_out.value = {};
    }

    void step() MT_COMPAT_OVERRIDE {
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

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "derivative_block<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif
};

template <typename T>
struct integrator_block MT_COMPAT_SUBCLASS {
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

    void reset() MT_COMPAT_OVERRIDE { s_out.value = s_in.reset; }

    void step() MT_COMPAT_OVERRIDE {
        if (s_in.reset_flag) {
            reset();
        } else {
            s_out.value += s_in.value * time_step;
        }
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "integrator_block<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;

    const double time_step;
};

template <typename T>
struct switch_block MT_COMPAT_SUBCLASS {
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

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        if (s_in.value_flag) {
            s_out.value = s_in.value_a;
        } else {
            s_out.value = s_in.value_b;
        }
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "switch_block<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

template <typename T>
struct limiter_block MT_COMPAT_SUBCLASS {
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

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        T x = s_in.value;

        if (x < s_in.limit_lower) {
            x = s_in.limit_lower;
        } else if (x > s_in.limit_upper) {
            x = s_in.limit_upper;
        }

        s_out.value = x;
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "limiter_block<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

template <typename T>
struct limiter_block_const MT_COMPAT_SUBCLASS {
    struct input_t {
        T value;
    };

    struct output_t {
        T value;
    };

    limiter_block_const(const T upper, const T lower)
        : bound_upper{upper}, bound_lower{lower} {}

    limiter_block_const(const limiter_block_const&) = delete;
    limiter_block_const& operator=(const limiter_block_const&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        T x = s_in.value;

        if (x < bound_lower) {
            x = bound_lower;
        } else if (x > bound_upper) {
            x = bound_upper;
        }

        s_out.value = x;
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "limiter_block_const<" << type_info<T>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;

    const T bound_upper;
    const T bound_lower;
};

template <typename T, RelationalOperator OP>
struct RelationalOperation {};

template <typename T>
struct RelationalOperation<T, RelationalOperator::EQUAL> {
    static bool operation(const T a, const T b) {
        return a == b;
    }
};

template <typename T>
struct RelationalOperation<T, RelationalOperator::NOT_EQUAL> {
    static bool operation(const T a, const T b) {
        return a != b;
    }
};

template <typename T>
struct RelationalOperation<T, RelationalOperator::GREATER_THAN> {
    static bool operation(const T a, const T b) {
        return a > b;
    }
};

template <typename T>
struct RelationalOperation<T, RelationalOperator::GREATER_THAN_EQUAL> {
    static bool operation(const T a, const T b) {
        return a >= b;
    }
};

template <typename T>
struct RelationalOperation<T, RelationalOperator::LESS_THAN> {
    static bool operation(const T a, const T b) {
        return a < b;
    }
};

template <typename T>
struct RelationalOperation<T, RelationalOperator::LESS_THAN_EQUAL> {
    static bool operation(const T a, const T b) {
        return a <= b;
    }
};

template <typename T, RelationalOperator OP>
struct relational_block MT_COMPAT_SUBCLASS {
    struct input_t {
        T value_a;
        T value_b;
    };

    struct output_t {
        bool value;
    };

    relational_block() = default;
    relational_block(const relational_block&) = delete;
    relational_block& operator=(const relational_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        s_out.value = RelationalOperation<T, OP>::operation(s_in.value_a, s_in.value_b);
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "relational_block<" << type_info<T>::name << ", " << relational_to_string(OP) << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

template <TrigFunction FCN>
struct TrigInfo {
    static const size_t input_count = 1;
};

template <>
struct TrigInfo<TrigFunction::ATAN2> {
    static const size_t input_count = 2;
};

template <typename T, size_t N, TrigFunction FCN>
struct TrigOperation {};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::SIN> {
    static T operation(const T values[N]) {
        static_assert(N == 1, "sin requires a single argument");
        return t_sin(values[0]);
    }
};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::COS> {
    static T operation(const T values[N]) {
        static_assert(N == 1, "cos requires a single argument");
        return t_cos(values[0]);
    }
};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::TAN> {
    static T operation(const T values[N]) {
        static_assert(N == 1, "tan requires a single argument");
        return t_tan(values[0]);
    }
};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::ASIN> {
    static T operation(const T values[N]) {
        static_assert(N == 1, "arcsin requires a single argument");
        return t_asin(values[0]);
    }
};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::ACOS> {
    static T operation(const T values[N]) {
        static_assert(N == 1, "arccos requires a single argument");
        return t_acos(values[0]);
    }
};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::ATAN> {
    static T operation(const T values[N]) {
        static_assert(N == 1, "arctan requires a single argument");
        return t_atan(values[0]);
    }
};

template <typename T, size_t N>
struct TrigOperation<T, N, TrigFunction::ATAN2> {
    static T operation(const T values[N]) {
        static_assert(N == 2, "arctan2 requires a single argument");
        return t_atan2(values[0], values[1]);
    }
};

template <typename T, TrigFunction FCN>
struct trig_block MT_COMPAT_SUBCLASS {
    struct input_t {
        T values[TrigInfo<FCN>::input_count];
    };

    struct output_t {
        T value;
    };

    trig_block() = default;
    trig_block(const trig_block&) = delete;
    trig_block& operator=(const trig_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        s_out.value = TrigOperation<T, TrigInfo<FCN>::input_count, FCN>::operation(s_in.values);
    }

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_inner_type_name() const MT_COMPAT_OVERRIDE {
        std::ostringstream oss;
        oss << "trig_block<" << type_info<T>::name << ", " << trig_func_to_string(FCN) << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

}
}

#endif // MT_STDLIB_H
