// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_TYPES_H
#define MT_STDLIB_TYPES_H

#include <cstdint>
#include <cstring>

#ifdef MT_STDLIB_USE_STRING_FUNCS
#include <string>
#endif

namespace mt {
namespace stdlib {

#ifdef MT_STDLIB_USE_STRING_FUNCS
extern const std::string BASE_NAMESPACE;
#endif

enum class DataType : uint32_t {
    BOOL = 0,
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
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "uint8_t";
#endif
};

template <>
struct type_info<DataType::I8> {
    using type_t = int8_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "int8_t";
#endif
};

template <>
struct type_info<DataType::U16> {
    using type_t = uint16_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "uint16_t";
#endif
};

template <>
struct type_info<DataType::I16> {
    using type_t = int16_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "int16_t";
#endif
};

template <>
struct type_info<DataType::U32> {
    using type_t = uint32_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "uint32_t";
#endif
};

template <>
struct type_info<DataType::I32> {
    using type_t = int32_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "int32_t";
#endif
};

template <>
struct type_info<DataType::F32> {
    using type_t = float;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "float";
#endif
};

template <>
struct type_info<DataType::F64> {
    using type_t = double;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "double";
#endif
};

template <>
struct type_info<DataType::BOOL> {
    using type_t = bool;
#if MT_STDLIB_USE_STRING_FUNCS
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

#ifdef MT_USE_C_COMPAT
struct block_interface {
    virtual void reset();

    virtual void step();

    virtual bool set_input_type(int port_num, DataType dt, const void* input, int data_size) = 0;

    virtual bool get_output_type(int port_num, DataType dt, void* output, int data_size) = 0;

protected:
    template <DataType DT>
    static bool set_input_value(typename type_info<DT>::type_t& value, const void* input, const int data_size) {
        using data_t = typename type_info<DT>::type_t;

        if (data_size != sizeof(data_t)) {
            return false;
        } else if (input == nullptr) {
            return false;
        }

        value = *static_cast<const data_t*>(input);
        return true;
    }

    template <DataType DT>
    static bool get_output_value(const typename type_info<DT>::type_t& value, void* output, const int data_size) {
        using data_t = typename type_info<DT>::type_t;

        if (data_size != sizeof(data_t)) {
            return false;
        } else if (output == nullptr) {
            return false;
        }

        std::memcpy(static_cast<data_t*>(output), &value, sizeof(value));
        return true;
    }

public:
#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_type_name(bool include_namespace = true);
#endif

protected:
#ifdef MT_STDLIB_USE_STRING_FUNCS
    virtual std::string get_class_name() const = 0;
#endif
};
#endif

}
}

#endif // MT_STDLIB_TYPES_H
