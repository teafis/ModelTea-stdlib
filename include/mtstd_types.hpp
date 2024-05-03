// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_TYPES_H
#define MT_STDLIB_TYPES_H

#include "mtstd_except.hpp"
#include <cstdint>

#ifdef MT_STDLIB_USE_FULL_LIB
#include <cstring>
#include <string>
#include <string_view>
#include <vector>

#endif

namespace mt {
namespace stdlib {

#ifdef MT_STDLIB_USE_FULL_LIB
extern const std::string BASE_NAMESPACE;
#endif

enum class DataType : uint32_t {
    NONE = 0,
    BOOL,
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    F32,
    F64,
};

template <DataType>
struct type_info {};

#ifdef MT_STDLIB_USE_FULL_LIB
struct type_info_meta {
    virtual ~type_info_meta() = default;
    virtual bool get_is_numeric() const = 0;
    virtual bool get_is_integral() const = 0;
    virtual bool get_is_float() const = 0;
    virtual bool get_is_signed() const = 0;
    virtual const char* get_name() const = 0;
    virtual DataType get_data_type() const = 0;
};

#define MT_STDLIB_TYPE_BASE : type_info_meta
const type_info_meta* get_meta_type(DataType dt);
const type_info_meta* get_meta_type(std::string_view s);
std::vector<const type_info_meta*> get_meta_types();
#else
#define MT_STDLIB_TYPE_BASE
#endif

template <>
struct type_info<DataType::U8> MT_STDLIB_TYPE_BASE {
    using type_t = uint8_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "uint8_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::U8; }
#endif
};

template <>
struct type_info<DataType::I8> MT_STDLIB_TYPE_BASE {
    using type_t = int8_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "int8_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::I8; }
#endif
};

template <>
struct type_info<DataType::U16> MT_STDLIB_TYPE_BASE {
    using type_t = uint16_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "uint16_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::U16; }
#endif
};

template <>
struct type_info<DataType::I16> MT_STDLIB_TYPE_BASE {
    using type_t = int16_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "int16_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::I16; }
#endif
};

template <>
struct type_info<DataType::U32> MT_STDLIB_TYPE_BASE {
    using type_t = uint32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "uint32_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::U32; }
#endif
};

template <>
struct type_info<DataType::I32> MT_STDLIB_TYPE_BASE {
    using type_t = int32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "int32_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::I32; }
#endif
};

template <>
struct type_info<DataType::F32> MT_STDLIB_TYPE_BASE {
    using type_t = float;
    static const bool is_numeric = true;
    static const bool is_integral = false;
    static const bool is_float = true;
    static const bool is_signed = true;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "float";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::F32; }
#endif
};

template <>
struct type_info<DataType::F64> MT_STDLIB_TYPE_BASE {
    using type_t = double;
    static const bool is_numeric = true;
    static const bool is_integral = false;
    static const bool is_float = true;
    static const bool is_signed = true;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "double";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::F64; }
#endif
};

template <>
struct type_info<DataType::BOOL> MT_STDLIB_TYPE_BASE {
    using type_t = bool;
    static const bool is_numeric = false;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "bool";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::BOOL; }
#endif
};

enum class ArithType {
    ADD = 0,
    SUB,
    MUL,
    DIV,
    MOD,
};

enum class RelationalOperator {
    EQUAL = 0,
    NOT_EQUAL,
    GREATER_THAN,
    GREATER_THAN_EQUAL,
    LESS_THAN,
    LESS_THAN_EQUAL,
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

#ifdef MT_STDLIB_USE_FULL_LIB

struct Argument {
    virtual ~Argument() = default;

    virtual DataType get_type() const = 0;

    virtual size_t as_size() const = 0;
};

template <DataType DT>
struct ArgumentBox final : public Argument {
    using data_t = typename type_info<DT>::type_t;

    ArgumentBox() : value{} {}
    ArgumentBox(const data_t val) : value{val} {}

    DataType get_type() const override {
        return DT;
    }

    size_t as_size() const override {
        if constexpr (type_info<DT>::is_integral) {
            if (value >= 0) {
                return static_cast<size_t>(value);
            } else {
                return 0;
            }
        } else {
            throw block_error("unable to convert data type to size");
        }
    }

    data_t value;
};

struct block_interface {
    struct block_types {
        bool uses_integral;
        bool uses_float;
        bool uses_logical;
    };

    virtual ~block_interface() = default;

    virtual block_types get_supported_types() const noexcept = 0;

    virtual void reset();

    virtual void step();

    virtual DataType get_current_type() const noexcept = 0;

    virtual void set_input(size_t port_num, const Argument* value) = 0;

    virtual void get_output(size_t port_num, Argument* value) const = 0;

    virtual size_t get_input_num() const noexcept = 0;

    virtual size_t get_output_num() const noexcept = 0;

    virtual DataType get_input_type(size_t port_num) const = 0;

    virtual DataType get_output_type(size_t port_num) const = 0;

    virtual bool get_input_type_settable(size_t port_num) const noexcept = 0;

    virtual bool outputs_are_delayed() const noexcept { return false; }

protected:
    template <DataType DT>
    static typename type_info<DT>::type_t get_model_value(const Argument* value) {
        using data_t = typename type_info<DT>::type_t;
        const auto cast_val = dynamic_cast<const ArgumentBox<DT>*>(value);

        if (cast_val == nullptr) {
            throw block_error("value cannot be nullptr");
        } else {
            return cast_val->value;
        }
    }

    template <DataType DT>
    static void set_model_value(Argument* value, const typename type_info<DT>::type_t x) {
        using data_t = typename type_info<DT>::type_t;
        auto cast_val = dynamic_cast<ArgumentBox<DT>*>(value);

        if (cast_val == nullptr) {
            throw block_error("value cannot be nullptr");
        } else {
            cast_val->value = x;
        }
    }

    template <DataType DT>
    static void set_input_value(typename type_info<DT>::type_t& value, const Argument* input) {
        value = get_model_value<DT>(input);
    }

    template <DataType DT>
    static void get_output_value(const typename type_info<DT>::type_t& value, Argument* output) {
        set_model_value<DT>(output, value);
    }

public:
    std::string get_type_name(bool include_namespace = true) const;

    virtual std::string get_block_name() const = 0;

protected:
    virtual std::string get_class_name() const = 0;
};

#endif // MT_STDLIB_USE_FULL_LIB

}
}

#endif // MT_STDLIB_TYPES_H
