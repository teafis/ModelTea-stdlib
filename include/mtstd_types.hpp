// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_TYPES_H
#define MT_STDLIB_TYPES_H

#include "mtstd_except.hpp"
#include <cstdint>

#ifdef MT_STDLIB_USE_FULL_LIB
#include <cstring>
#include <string>

#include "mtstd_compat_types.h"
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

template <>
struct type_info<DataType::U8> {
    using type_t = uint8_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "uint8_t";
#endif
};

template <>
struct type_info<DataType::I8> {
    using type_t = int8_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "int8_t";
#endif
};

template <>
struct type_info<DataType::U16> {
    using type_t = uint16_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "uint16_t";
#endif
};

template <>
struct type_info<DataType::I16> {
    using type_t = int16_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "int16_t";
#endif
};

template <>
struct type_info<DataType::U32> {
    using type_t = uint32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "uint32_t";
#endif
};

template <>
struct type_info<DataType::I32> {
    using type_t = int32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "int32_t";
#endif
};

template <>
struct type_info<DataType::F32> {
    using type_t = float;
    static const bool is_numeric = true;
    static const bool is_integral = false;
    static const bool is_float = true;
    static const bool is_signed = true;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "float";
#endif
};

template <>
struct type_info<DataType::F64> {
    using type_t = double;
    static const bool is_numeric = true;
    static const bool is_integral = false;
    static const bool is_float = true;
    static const bool is_signed = true;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "double";
#endif
};

template <>
struct type_info<DataType::BOOL> {
    using type_t = bool;
    static const bool is_numeric = false;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#if MT_STDLIB_USE_FULL_LIB
    static constexpr const char* name = "bool";
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

struct ArgumentValue
{
    virtual DataType get_type() const = 0;

    virtual size_t as_size() const = 0;
};

template <DataType DT>
struct ArgumentBox final : public ArgumentValue
{
    using data_t = typename type_info<DT>::type_t;

    ArgumentBox() : value{} {}
    ArgumentBox(const data_t val) : value{ val } {}

    DataType get_type() const override
    {
        return DT;
    }

    size_t as_size() const override
    {
        if constexpr (type_info<DT>::is_integral) {
            if (value >= 0)
            {
                return static_cast<size_t>(value);
            }
            else
            {
                return 0;
            }
        }
        else
        {
            throw block_error("unable to convert data type to size");
        }
    }

    data_t value;
};

struct block_interface {
    virtual void reset();

    virtual void step();

    virtual void set_input(size_t port_num, const ArgumentValue* value) = 0;

    virtual void get_output(size_t port_num, ArgumentValue* value) const = 0;

    virtual size_t get_input_num() const = 0;

    virtual size_t get_output_num() const = 0;

    virtual DataType get_input_type(size_t port_num) const = 0;

    virtual DataType get_output_type(size_t port_num) const = 0;

    virtual bool get_input_type_settable(size_t port_num) const = 0;

protected:
    template <DataType DT>
    static typename type_info<DT>::type_t get_model_value(const ArgumentValue* value) {
        using data_t = typename type_info<DT>::type_t;
        const auto cast_val = dynamic_cast<const ArgumentBox<DT>*>(value);

        if (cast_val == nullptr) {
            throw block_error("value cannot be nullptr");
        }
        else {
            return cast_val->value;
        }
    }

    template <DataType DT>
    static void set_model_value(ArgumentValue* value, const typename type_info<DT>::type_t x) {
        using data_t = typename type_info<DT>::type_t;
        auto cast_val = dynamic_cast<ArgumentBox<DT>*>(value);

        if (cast_val == nullptr) {
            throw block_error("value cannot be nullptr");
        }
        else {
            cast_val->value = x;
        }
    }

    template <DataType DT>
    static void set_input_value(typename type_info<DT>::type_t& value, const ArgumentValue* input) {
        value = get_model_value<DT>(input);
    }

    template <DataType DT>
    static void get_output_value(const typename type_info<DT>::type_t& value, ArgumentValue* output) {
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
