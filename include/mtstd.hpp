// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_H
#define MT_STDLIB_H

#include <array>
#include <cstddef>
#include <sstream>

#include "mtstd_compat_types.h"
#include "mtstd_except.hpp"
#include "mtstd_ext.hpp"
#include "mtstd_types.hpp"

#ifdef MT_STDLIB_USE_FULL_LIB
#include "mtstd_string.hpp"
#include <sstream>
#include <string>
#endif

namespace mt {
namespace stdlib {

#if MT_STDLIB_USE_FULL_LIB
extern const std::string BASE_NAMESPACE;
#define MT_COMPAT_SUBCLASS : public block_interface
#define MT_COMPAT_OVERRIDE override
#else
#define MT_COMPAT_SUBCLASS
#define MT_COMPAT_OVERRIDE
#endif

template <DataType DT, ArithType AT>
struct ArithOperation {};

template <DataType DT>
struct ArithOperation<DT, ArithType::ADD> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t a, const data_t b) {
        return a + b;
    }
};

template <DataType DT>
struct ArithOperation<DT, ArithType::SUB> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t a, const data_t b) {
        return a - b;
    }
};

template <DataType DT>
struct ArithOperation<DT, ArithType::MUL> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t a, const data_t b) {
        return a * b;
    }
};

template <DataType DT>
struct ArithOperation<DT, ArithType::DIV> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t a, const data_t b) {
        return a / b;
    }
};

template <DataType DT>
struct ArithOperation<DT, ArithType::MOD> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t a, const data_t b) {
        return t_mod(a, b);
    }
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct arith_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT, ArithType AT>
struct arith_block_dynamic MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t* values;
        int size;
    };

    struct output_t {
        data_t value;
    };

    arith_block_dynamic() = default;
    arith_block_dynamic(const arith_block_dynamic&) = delete;
    arith_block_dynamic& operator=(const arith_block_dynamic&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        data_t val = s_in.values[0];

        for (int i = 1; i < s_in.size; ++i) {
            val = ArithOperation<DT, AT>::operation(val, s_in.values[i]);
        }

        s_out.value = val;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        if (s_in.values != nullptr && port_num < s_in.size) {
            set_input_value<DT>(s_in.values[port_num], value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            return get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return s_in.size;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < s_in.size) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "arith_block_dynamic<" << type_info<DT>::name << ", " << arith_to_string(AT) << ">";
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

template <DataType DT, ArithType AT, int SIZE>
struct arith_block : public arith_block_dynamic<DT, AT> {
    using data_t = typename type_info<DT>::type_t;

    arith_block() {
        this->s_in.size = SIZE;
        this->s_in.values = _input_array.data();
    }

    arith_block(const arith_block&) = delete;
    arith_block& operator=(const arith_block&) = delete;

#ifdef MT_STDLIB_USE_FULL_LIB
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "arith_block<" << type_info<DT>::name << ", " << arith_to_string(AT) << ", " << SIZE << ">";
        return oss.str();
    }
#endif

private:
    std::array<data_t, SIZE> _input_array;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct clock_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct clock_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct output_t {
        data_t value;
    };

    explicit clock_block(data_t dt) : time_step{dt} {
        static_assert(type_info<DT>::is_numeric, "data type must be numeric");
        reset();
    }

    clock_block(const clock_block&) = delete;
    clock_block& operator=(const clock_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE { s_out.value = 0.0; }

    void step() MT_COMPAT_OVERRIDE { s_out.value += time_step; }

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        throw block_error("input port too high");
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            return get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override { return 0; }

    size_t get_output_num() const override { return 1; }

    DataType get_input_type(size_t port_num) const override {
        return DataType::NONE;
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        return "clock_block";
    }
#endif

    output_t s_out;

    const data_t time_step;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct const_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = true;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct const_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct output_t {
        data_t value;
    };

    explicit const_block(const data_t val) : s_out{.value = val} {
        // Empty Constructor
    }

    const_block(const const_block&) = delete;
    const_block& operator=(const const_block&) = delete;

    const output_t s_out;

#ifdef MT_STDLIB_USE_FULL_LIB
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "const_block<" << type_info<DT>::name << '>';
        return oss.str();
    }
    void set_input(size_t port_num, const mt_value_t* value) override {
        throw block_error("input port too high");
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            return get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 0;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        return DataType::NONE;
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }
#endif
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct delay_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = true;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct delay_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t value;
        data_t reset;
        bool reset_flag;
    };

    struct output_t {
        data_t value;
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

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == 1) {
            set_input_value<DT>(s_in.reset, value);
        } else if (port_num == 2) {
            set_input_value<DataType::BOOL>(s_in.reset_flag, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 3;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0 || port_num == 1) {
            return DT;
        } else if (port_num == 2) {
            return DataType::BOOL;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "delay_block<" << type_info<DT>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;

    data_t next_value;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct derivative_block_types {
    static const bool uses_integral = false;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct derivative_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t value;
        bool reset_flag;
    };

    struct output_t {
        data_t value;
    };

    explicit derivative_block(const double dt) : s_in{}, s_out{}, time_step(dt) {
        static_assert(type_info<DT>::is_float, "derivative data type must be a floating point type");
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

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == 1) {
            set_input_value<DataType::BOOL>(s_in.reset_flag, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 2;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else if (port_num == 1) {
            return DataType::BOOL;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "derivative_block<" << type_info<DT>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;

    data_t last_value;
    const double time_step;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct integrator_block_types {
    static const bool uses_integral = false;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct integrator_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t value;
        data_t reset;
        bool reset_flag;
    };

    struct output_t {
        data_t value;
    };

    explicit integrator_block(const double dt) : time_step(dt) {
        static_assert(type_info<DT>::is_float, "integrator data type must be a floating point type");
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

#ifdef MT_STDLIB_USE_FULL_LIB
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "integrator_block<" << type_info<DT>::name << '>';
        return oss.str();
    }

    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == 1) {
            set_input_value<DT>(s_in.reset, value);
        } else if (port_num == 2) {
            set_input_value<DataType::BOOL>(s_in.reset_flag, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 3;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0 || port_num == 1) {
            return DT;
        } else if (port_num == 2) {
            return DataType::BOOL;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }
#endif

    input_t s_in;
    output_t s_out;

    const double time_step;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct switch_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = true;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct switch_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        bool value_flag;
        data_t value_a;
        data_t value_b;
    };

    struct output_t {
        data_t value;
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

#ifdef MT_STDLIB_USE_FULL_LIB
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "switch_block<" << type_info<DT>::name << '>';
        return oss.str();
    }

    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DataType::BOOL>(s_in.value_flag, value);
        } else if (port_num == 1) {
            set_input_value<DT>(s_in.value_a, value);
        } else if (port_num == 2) {
            set_input_value<DT>(s_in.value_b, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 3;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0) {
            return DataType::BOOL;
        } else if (port_num == 1 || port_num == 2) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }
#endif

    input_t s_in;
    output_t s_out;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct limiter_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct limiter_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t value;
        data_t limit_upper;
        data_t limit_lower;
    };

    struct output_t {
        data_t value;
    };

    limiter_block() = default;
    limiter_block(const limiter_block&) = delete;
    limiter_block& operator=(const limiter_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        data_t x = s_in.value;

        if (x < s_in.limit_lower) {
            x = s_in.limit_lower;
        } else if (x > s_in.limit_upper) {
            x = s_in.limit_upper;
        }

        s_out.value = x;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == 1) {
            set_input_value<DT>(s_in.limit_lower, value);
        } else if (port_num == 2) {
            set_input_value<DT>(s_in.limit_upper, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 3;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "limiter_block<" << type_info<DT>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

template <DataType DT>
struct limiter_block_const MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t value;
    };

    struct output_t {
        data_t value;
    };

    limiter_block_const(const data_t upper, const data_t lower)
        : bound_upper{upper}, bound_lower{lower} {}

    limiter_block_const(const limiter_block_const&) = delete;
    limiter_block_const& operator=(const limiter_block_const&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        data_t x = s_in.value;

        if (x < bound_lower) {
            x = bound_lower;
        } else if (x > bound_upper) {
            x = bound_upper;
        }

        s_out.value = x;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 1;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "limiter_block_const<" << type_info<DT>::name << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;

    const data_t bound_upper;
    const data_t bound_lower;
};

template <DataType DT, RelationalOperator OP>
struct RelationalOperation {};

template <DataType DT>
struct RelationalOperation<DT, RelationalOperator::EQUAL> {
    using data_t = typename type_info<DT>::type_t;
    static bool operation(const data_t a, const data_t b) {
        return a == b;
    }
};

template <DataType DT>
struct RelationalOperation<DT, RelationalOperator::NOT_EQUAL> {
    using data_t = typename type_info<DT>::type_t;
    static bool operation(const data_t a, const data_t b) {
        return a != b;
    }
};

template <DataType DT>
struct RelationalOperation<DT, RelationalOperator::GREATER_THAN> {
    using data_t = typename type_info<DT>::type_t;
    static bool operation(const data_t a, const data_t b) {
        return a > b;
    }
};

template <DataType DT>
struct RelationalOperation<DT, RelationalOperator::GREATER_THAN_EQUAL> {
    using data_t = typename type_info<DT>::type_t;
    static bool operation(const data_t a, const data_t b) {
        return a >= b;
    }
};

template <DataType DT>
struct RelationalOperation<DT, RelationalOperator::LESS_THAN> {
    using data_t = typename type_info<DT>::type_t;
    static bool operation(const data_t a, const data_t b) {
        return a < b;
    }
};

template <DataType DT>
struct RelationalOperation<DT, RelationalOperator::LESS_THAN_EQUAL> {
    using data_t = typename type_info<DT>::type_t;
    static bool operation(const data_t a, const data_t b) {
        return a <= b;
    }
};

#ifdef MT_STDLIB_USE_FULL_LIB
template <RelationalOperator OP>
struct relational_block_types {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};

template <>
struct relational_block_types<RelationalOperator::EQUAL> {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = true;
};

template <>
struct relational_block_types<RelationalOperator::NOT_EQUAL> {
    static const bool uses_integral = true;
    static const bool uses_float = true;
    static const bool uses_logical = true;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT, RelationalOperator OP>
struct relational_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t value_a;
        data_t value_b;
    };

    struct output_t {
        bool value;
    };

    relational_block() = default;
    relational_block(const relational_block&) = delete;
    relational_block& operator=(const relational_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        s_out.value = RelationalOperation<DT, OP>::operation(s_in.value_a, s_in.value_b);
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "relational_block<" << type_info<DT>::name << ", " << relational_to_string(OP) << '>';
        return oss.str();
    }

    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value_a, value);
        } else if (port_num == 0) {
            set_input_value<DT>(s_in.value_b, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return 2;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DataType::BOOL;
        } else {
            return DataType::NONE;
        }
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

template <DataType DT, size_t N, TrigFunction FCN>
struct TrigOperation {};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::SIN> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 1, "sin requires a single argument");
        static_assert(type_info<DT>::is_numeric, "sin requires a numeric data type");
        return t_sin(values[0]);
    }
};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::COS> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 1, "cos requires a single argument");
        static_assert(type_info<DT>::is_numeric, "cos requires a numeric data type");
        return t_cos(values[0]);
    }
};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::TAN> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 1, "tan requires a single argument");
        static_assert(type_info<DT>::is_numeric, "tan requires a numeric data type");
        return t_tan(values[0]);
    }
};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::ASIN> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 1, "arcsin requires a single argument");
        static_assert(type_info<DT>::is_numeric, "arcsin requires a numeric data type");
        return t_asin(values[0]);
    }
};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::ACOS> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 1, "arccos requires a single argument");
        static_assert(type_info<DT>::is_numeric, "arccos requires a numeric data type");
        return t_acos(values[0]);
    }
};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::ATAN> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 1, "arctan requires a single argument");
        static_assert(type_info<DT>::is_numeric, "arctan requires a numeric data type");
        return t_atan(values[0]);
    }
};

template <DataType DT, size_t N>
struct TrigOperation<DT, N, TrigFunction::ATAN2> {
    using data_t = typename type_info<DT>::type_t;
    static data_t operation(const data_t values[N]) {
        static_assert(N == 2, "arctan2 requires a single argument");
        static_assert(type_info<DT>::is_numeric, "arctan2 requires a numeric data type");
        return t_atan2(values[0], values[1]);
    }
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct trig_block_types {
    static const bool uses_integral = false;
    static const bool uses_float = true;
    static const bool uses_logical = false;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT, TrigFunction FCN>
struct trig_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct input_t {
        data_t values[TrigInfo<FCN>::input_count];
    };

    struct output_t {
        data_t value;
    };

    trig_block() = default;
    trig_block(const trig_block&) = delete;
    trig_block& operator=(const trig_block&) = delete;

    void reset() MT_COMPAT_OVERRIDE { step(); }

    void step() MT_COMPAT_OVERRIDE {
        s_out.value = TrigOperation<DT, TrigInfo<FCN>::input_count, FCN>::operation(s_in.values);
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    void set_input(size_t port_num, const mt_value_t* value) override {
        if (port_num < get_input_num()) {
            set_input_value<DT>(s_in.values[port_num], value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, mt_value_t* value) override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const override {
        return TrigInfo<FCN>::input_count;
    }

    size_t get_output_num() const override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            return DataType::NONE;
        }
    }

    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "trig_block<" << type_info<DT>::name << ", " << trig_func_to_string(FCN) << '>';
        return oss.str();
    }
#endif

    input_t s_in;
    output_t s_out;
};

}
}

#endif // MT_STDLIB_H
