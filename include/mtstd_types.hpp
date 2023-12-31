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
struct block_interface {
    virtual void reset();

    virtual void step();

    virtual void set_input(size_t port_num, const mt_value_t* value) = 0;

    virtual void get_output(size_t port_num, mt_value_t* value) = 0;

    virtual size_t get_input_num() const = 0;

    virtual size_t get_output_num() const = 0;

    virtual DataType get_input_type(size_t port_num) const = 0;

    virtual DataType get_output_type(size_t port_num) const = 0;

protected:
    template <DataType DT>
    static type_info<DT>::type_t get_model_value(const mt_value_t* value) {
        using data_t = typename type_info<DT>::type_t;

        if (value == nullptr) {
            throw block_error("value cannot be nullptr");
        } else if (value->data == nullptr) {
            throw block_error("value data cannot be nullptr");
        } else if (sizeof(data_t) != value->size || static_cast<DataType>(value->type) != DT) {
            throw block_error("data types do not match for provided model value");
        }

        return *static_cast<const data_t*>(value->data);
    }

    template <DataType DT>
    static void set_model_value(mt_value_t* value, const type_info<DT>::type_t x) {
        using data_t = typename type_info<DT>::type_t;

        if (value == nullptr) {
            throw block_error("value cannot be nullptr");
        } else if (value->data == nullptr) {
            throw block_error("value data cannot be nullptr");
        } else if (sizeof(data_t) != value->size || static_cast<DataType>(value->type) != DT) {
            throw block_error("data types do not match for provided model value");
        }

        *static_cast<data_t*>(value->data) = x;
    }

    template <DataType DT>
    static void set_input_value(typename type_info<DT>::type_t& value, const mt_value_t* input) {
        value = get_model_value<DT>(input);
    }

    template <DataType DT>
    static void get_output_value(const typename type_info<DT>::type_t& value, mt_value_t* output) {
        set_model_value<DT>(output, value);
    }

public:
    std::string get_type_name(bool include_namespace = true);

protected:
    virtual std::string get_class_name() const = 0;
};

#endif // MT_STDLIB_USE_FULL_LIB

}
}

#endif // MT_STDLIB_TYPES_H
