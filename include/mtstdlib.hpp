// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_H
#define MT_STDLIB_H

#include <array>
#include <cstddef>
#include <sstream>

#include "mtstdlib_except.hpp"
#include "mtstdlib_ext.hpp"
#include "mtstdlib_types.hpp"

#ifdef MT_STDLIB_USE_FULL_LIB
#include "mtstdlib_string.hpp"
#include <sstream>
#endif

namespace mt {
namespace stdlib {

#ifdef MT_STDLIB_USE_FULL_LIB
#define MT_COMPAT_SUBCLASS : public block_interface
#define MT_COMPAT_OVERRIDE override
#else
#define MT_COMPAT_SUBCLASS
#define MT_COMPAT_OVERRIDE
#endif

using time_step_t = double;

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
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { step(); }

    void step() noexcept MT_COMPAT_OVERRIDE {
        if (s_in.size > 0) {
            data_t val = s_in.values[0];

            for (int i = 1; i < s_in.size; ++i) {
                val = ArithOperation<DT, AT>::operation(val, s_in.values[i]);
            }

            s_out.value = val;
        } else {
            s_out.value = {};
        }
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    using type_info_t = arith_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (s_in.values != nullptr && port_num < s_in.size) {
            set_input_value<DT>(s_in.values[port_num], value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            return get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return s_in.size;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < get_input_num();
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < s_in.size) {
            return DT;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
        if (port_num < s_in.size) {
            return (std::ostringstream() << "values[" << port_num << "]").str();
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "arith_block_dynamic<" << datatype_to_string(DT) << ", " << arith_to_string(AT) << ">";
        return oss.str();
    }

    std::string get_class_name_codegen() const override {
        std::ostringstream oss;
        oss << "arith_block<" << datatype_to_string(DT) << ", " << arith_to_string(AT) << ", " << s_in.size << ">";
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        if constexpr (AT == ArithType::ADD) {
            return BLK_NAME_ARITH_ADD;
        } else if constexpr (AT == ArithType::SUB) {
            return BLK_NAME_ARITH_SUB;
        } else if constexpr (AT == ArithType::MUL) {
            return BLK_NAME_ARITH_MUL;
        } else if constexpr (AT == ArithType::DIV) {
            return BLK_NAME_ARITH_DIV;
        } else if constexpr (AT == ArithType::MOD) {
            return BLK_NAME_ARITH_MOD;
        } else {
            static_assert("unknown arithmetic type provided");
        }
    }
#endif

public:
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
protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "arith_block<" << datatype_to_string(DT) << ", " << arith_to_string(AT) << ", " << SIZE << ">";
        return oss.str();
    }
#endif

private:
    std::array<data_t, SIZE> _input_array;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct clock_block_types {
    static constexpr bool uses_integral = false;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { s_out.value = 0.0; }

    void step() noexcept MT_COMPAT_OVERRIDE { s_out.value += time_step; }

#ifdef MT_STDLIB_USE_FULL_LIB
    explicit clock_block(const Argument* input) : clock_block(get_model_value<DT>(input)) {}

    using type_info_t = clock_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        throw block_error("input port too high");
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            return get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override { return 0; }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return false;
    }

    size_t get_output_num() const noexcept override { return 1; }

    DataType get_input_type(size_t port_num) const override {
        throw block_error("input port too high");
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
            throw block_error("input port too high");
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        return (std::ostringstream{} << "clock_block<" << datatype_to_string(DT) << '>').str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_CLOCK;
    }
#endif

    output_t s_out;

    const data_t time_step;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct const_block_types {
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = true;
};
#endif // MT_STDLIB_USE_FULL_LIB

template <DataType DT>
struct const_block MT_COMPAT_SUBCLASS {
    using data_t = typename type_info<DT>::type_t;

    struct output_t {
        data_t value;
    };

    const_block(const data_t val) : s_out{.value = val} {
        // Empty Constructor
    }

    const_block(const const_block&) = delete;
    const_block& operator=(const const_block&) = delete;

    const output_t s_out;

#ifdef MT_STDLIB_USE_FULL_LIB
    explicit const_block(const Argument* value) : const_block(get_model_value<DT>(value)) {}

    using type_info_t = const_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        throw block_error("input port too high");
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            return get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 0;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return false;
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        throw block_error("input port too high");
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
            throw block_error("input port too high");
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "const_block<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_CONST;
    }
#endif
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct delay_block_types {
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = true;
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

    void reset() noexcept MT_COMPAT_OVERRIDE {
        next_value = s_in.reset;
        s_out.value = s_in.reset;
    }

    void step() noexcept MT_COMPAT_OVERRIDE {
        if (s_in.reset_flag) {
            reset();
        }

        s_out.value = next_value;
        next_value = s_in.value;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    static const size_t PORT_VALUE_NUM = 0;
    static const size_t PORT_RESET_NUM = 1;
    static const size_t PORT_FLAG_NUM = 2;

    using type_info_t = delay_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == PORT_VALUE_NUM) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == PORT_RESET_NUM) {
            set_input_value<DT>(s_in.reset, value);
        } else if (port_num == PORT_FLAG_NUM) {
            set_input_value<DataType::BOOL>(s_in.reset_flag, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 3;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < PORT_FLAG_NUM;
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0 || port_num == 1) {
            return DT;
        } else if (port_num == 2) {
            return DataType::BOOL;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    bool outputs_are_delayed() const noexcept override { return true; }

    std::string get_input_name(size_t port_num) const override {
        if (port_num == PORT_VALUE_NUM) {
            return "value";
        } else if (port_num == PORT_RESET_NUM) {
            return "reset";
        } else if (port_num == PORT_FLAG_NUM) {
            return "reset_flag";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "delay_block<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_DELAY;
    }
#endif

    input_t s_in;
    output_t s_out;

    data_t next_value;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct derivative_block_types {
    static constexpr bool uses_integral = false;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
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

    explicit derivative_block(const time_step_t dt) : s_in{}, s_out{}, time_step(dt) {
        static_assert(type_info<DT>::is_float, "derivative data type must be a floating point type");
    }

    derivative_block(const derivative_block&) = delete;
    derivative_block& operator=(const derivative_block&) = delete;

    void reset() noexcept MT_COMPAT_OVERRIDE {
        last_value = {};
        s_out.value = {};
    }

    void step() noexcept MT_COMPAT_OVERRIDE {
        if (s_in.reset_flag) {
            reset();
        }

        s_out.value = (s_in.value - last_value) / time_step;
        last_value = s_in.value;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    explicit derivative_block(const Argument* dt) : derivative_block(get_model_value<DT>(dt)) {}

    using type_info_t = derivative_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == 1) {
            set_input_value<DataType::BOOL>(s_in.reset_flag, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 2;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num == 0;
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else if (port_num == 1) {
            return DataType::BOOL;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    bool outputs_are_delayed() const noexcept override { return true; }

std::string get_input_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else if (port_num == 1) {
            return "reset_flag";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "derivative_block<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_DERIV;
    }
#endif

    input_t s_in;
    output_t s_out;

    data_t last_value;
    const time_step_t time_step;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct integrator_block_types {
    static constexpr bool uses_integral = false;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
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

    explicit integrator_block(const time_step_t dt) : time_step(dt) {
        static_assert(type_info<DT>::is_float, "integrator data type must be a floating point type");
    }

    integrator_block(const integrator_block&) = delete;
    integrator_block& operator=(const integrator_block&) = delete;

    void reset() noexcept MT_COMPAT_OVERRIDE { s_out.value = s_in.reset; }

    void step() noexcept MT_COMPAT_OVERRIDE {
        if (s_in.reset_flag) {
            reset();
        } else {
            s_out.value += s_in.value * time_step;
        }
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    explicit integrator_block(const Argument* dt) : integrator_block(get_model_value<DT>(dt)) {}

    static const size_t PORT_VALUE_NUM = 0;
    static const size_t PORT_RESET_NUM = 1;
    static const size_t PORT_FLAG_NUM = 2;

    using type_info_t = integrator_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == PORT_VALUE_NUM) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == PORT_RESET_NUM) {
            set_input_value<DT>(s_in.reset, value);
        } else if (port_num == PORT_FLAG_NUM) {
            set_input_value<DataType::BOOL>(s_in.reset_flag, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 3;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < PORT_FLAG_NUM;
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == 0 || port_num == 1) {
            return DT;
        } else if (port_num == 2) {
            return DataType::BOOL;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    bool outputs_are_delayed() const noexcept override { return true; }

    std::string get_input_name(size_t port_num) const override {
        if (port_num == PORT_VALUE_NUM) {
            return "value";
        } else if (port_num == PORT_RESET_NUM) {
            return "reset";
        } else if (port_num == PORT_FLAG_NUM) {
            return "reset_flag";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "integrator_block<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_INTEG;
    }
#endif

    input_t s_in;
    output_t s_out;

    const time_step_t time_step;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct switch_block_types {
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = true;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { step(); }

    void step() noexcept MT_COMPAT_OVERRIDE {
        if (s_in.value_flag) {
            s_out.value = s_in.value_a;
        } else {
            s_out.value = s_in.value_b;
        }
    }

#ifdef MT_STDLIB_USE_FULL_LIB
protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "switch_block<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

    static const size_t PORT_VALUE_FLAG = 0;
    static const size_t PORT_VALUE_A = 1;
    static const size_t PORT_VALUE_B = 2;

public:
    std::string get_block_name() const override {
        return BLK_NAME_SWITCH;
    }

    using type_info_t = switch_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == PORT_VALUE_FLAG) {
            set_input_value<DataType::BOOL>(s_in.value_flag, value);
        } else if (port_num == PORT_VALUE_A) {
            set_input_value<DT>(s_in.value_a, value);
        } else if (port_num == PORT_VALUE_B) {
            set_input_value<DT>(s_in.value_b, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 3;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < get_input_num() && port_num != PORT_VALUE_FLAG;
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num == PORT_VALUE_FLAG) {
            return DataType::BOOL;
        } else if (port_num == PORT_VALUE_A || port_num == PORT_VALUE_B) {
            return DT;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
        if (port_num == PORT_VALUE_FLAG) {
            return "value_flag";
        } else if (port_num == PORT_VALUE_A) {
            return "value_a";
        } else if (port_num == PORT_VALUE_B) {
            return "value_b";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }
#endif

    input_t s_in;
    output_t s_out;
};

#ifdef MT_STDLIB_USE_FULL_LIB
struct limiter_block_types {
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { step(); }

    void step() noexcept MT_COMPAT_OVERRIDE {
        data_t x = s_in.value;

        if (x < s_in.limit_lower) {
            x = s_in.limit_lower;
        } else if (x > s_in.limit_upper) {
            x = s_in.limit_upper;
        }

        s_out.value = x;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
protected:
    static const size_t PORT_VALUE = 0;
    static const size_t PORT_LIMIT_UPPER = 1;
    static const size_t PORT_LIMIT_LOWER = 2;

public:
    using type_info_t = limiter_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == PORT_VALUE) {
            set_input_value<DT>(s_in.value, value);
        } else if (port_num == PORT_LIMIT_LOWER) {
            set_input_value<DT>(s_in.limit_lower, value);
        } else if (port_num == PORT_LIMIT_UPPER) {
            set_input_value<DT>(s_in.limit_upper, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 3;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < get_input_num();
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
        if (port_num == PORT_VALUE) {
            return "value";
        } else if (port_num == PORT_LIMIT_LOWER) {
            return "limit_lower";
        } else if (port_num == PORT_LIMIT_UPPER) {
            return "limit_upper";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "limiter_block<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_LIMITER;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { step(); }

    void step() noexcept MT_COMPAT_OVERRIDE {
        data_t x = s_in.value;

        if (x < bound_lower) {
            x = bound_lower;
        } else if (x > bound_upper) {
            x = bound_upper;
        }

        s_out.value = x;
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    explicit limiter_block_const(const Argument* upper, const Argument* lower) : limiter_block_const(get_model_value<DT>(upper), get_model_value<DT>(lower)) {}

    using type_info_t = limiter_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 1;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < get_input_num();
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "limiter_block_const<" << datatype_to_string(DT) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        return BLK_NAME_LIMITER;
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
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
};

template <>
struct relational_block_types<RelationalOperator::EQUAL> {
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = true;
};

template <>
struct relational_block_types<RelationalOperator::NOT_EQUAL> {
    static constexpr bool uses_integral = true;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = true;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { step(); }

    void step() noexcept MT_COMPAT_OVERRIDE {
        s_out.value = RelationalOperation<DT, OP>::operation(s_in.value_a, s_in.value_b);
    }

#ifdef MT_STDLIB_USE_FULL_LIB
protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "relational_block<" << datatype_to_string(DT) << ", " << relational_to_string(OP) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        if constexpr (OP == RelationalOperator::EQUAL) {
            return BLK_NAME_REL_EQ;
        } else if constexpr (OP == RelationalOperator::NOT_EQUAL) {
            return BLK_NAME_REL_NEQ;
        } else if constexpr (OP == RelationalOperator::GREATER_THAN) {
            return BLK_NAME_REL_GT;
        } else if constexpr (OP == RelationalOperator::GREATER_THAN_EQUAL) {
            return BLK_NAME_REL_GEQ;
        } else if constexpr (OP == RelationalOperator::LESS_THAN) {
            return BLK_NAME_REL_LT;
        } else if constexpr (OP == RelationalOperator::LESS_THAN_EQUAL) {
            return BLK_NAME_REL_LEQ;
        } else {
            static_assert("unknown relational operation provided");
        }
    }

    using type_info_t = relational_block_types<OP>;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num == 0) {
            set_input_value<DT>(s_in.value_a, value);
        } else if (port_num == 0) {
            set_input_value<DT>(s_in.value_b, value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return 2;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < get_input_num();
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DataType::BOOL;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value_a";
        } else if (port_num == 1) {
            return "value_b";
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
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
    static constexpr bool uses_integral = false;
    static constexpr bool uses_float = true;
    static constexpr bool uses_logical = false;
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

    void reset() noexcept MT_COMPAT_OVERRIDE { step(); }

    void step() noexcept MT_COMPAT_OVERRIDE {
        s_out.value = TrigOperation<DT, TrigInfo<FCN>::input_count, FCN>::operation(s_in.values);
    }

#ifdef MT_STDLIB_USE_FULL_LIB
    using type_info_t = trig_block_types;
    block_types get_supported_types() const noexcept override {
        return block_types{
            .uses_integral = type_info_t::uses_integral,
            .uses_float = type_info_t::uses_float,
            .uses_logical = type_info_t::uses_logical,
        };
    }

    DataType get_current_type() const noexcept override {
        return DT;
    }

    void set_input(size_t port_num, const Argument* value) override {
        if (port_num < get_input_num()) {
            set_input_value<DT>(s_in.values[port_num], value);
        } else {
            throw block_error("input port too high");
        }
    }

    void get_output(size_t port_num, Argument* value) const override {
        if (port_num == 0) {
            get_output_value<DT>(s_out.value, value);
        } else {
            throw block_error("output port too high");
        }
    }

    size_t get_input_num() const noexcept override {
        return TrigInfo<FCN>::input_count;
    }

    bool get_input_type_settable(size_t port_num) const noexcept override {
        return port_num < get_input_num();
    }

    size_t get_output_num() const noexcept override {
        return 1;
    }

    DataType get_input_type(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return DT;
        } else {
            throw block_error("input port too high");
        }
    }

    DataType get_output_type(size_t port_num) const override {
        if (port_num == 0) {
            return DT;
        } else {
            throw block_error("output port too high");
        }
    }

    std::string get_input_name(size_t port_num) const override {
        if (port_num < get_input_num()) {
            return (std::ostringstream() << "values[" << port_num << "]").str();
        } else {
            throw block_error("input port too high");
        }
    }

    std::string get_output_name(size_t port_num) const override {
        if (port_num == 0) {
            return "value";
        } else {
            throw block_error("output port too high");
        }
    }

protected:
    std::string get_class_name() const override {
        std::ostringstream oss;
        oss << "trig_block<" << datatype_to_string(DT) << ", " << trig_func_to_string(FCN) << '>';
        return oss.str();
    }

public:
    std::string get_block_name() const override {
        if constexpr (FCN == TrigFunction::SIN) {
            return BLK_NAME_TRIG_SIN;
        } else if (FCN == TrigFunction::COS) {
            return BLK_NAME_TRIG_COS;
        } else if (FCN == TrigFunction::TAN) {
            return BLK_NAME_TRIG_TAN;
        } else if (FCN == TrigFunction::ASIN) {
            return BLK_NAME_TRIG_ASIN;
        } else if (FCN == TrigFunction::ACOS) {
            return BLK_NAME_TRIG_ACOS;
        } else if (FCN == TrigFunction::ATAN) {
            return BLK_NAME_TRIG_ATAN;
        } else if (FCN == TrigFunction::ATAN2) {
            return BLK_NAME_TRIG_ATAN2;
        } else {
            static_assert("unknown trig block type");
        }
    }

#endif

    input_t s_in;
    output_t s_out;
};

}
}

#endif // MT_STDLIB_H
