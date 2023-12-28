// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_TYPES_H
#define MT_STDLIB_TYPES_H

#include <cstdint>

#ifdef MT_STDLIB_USE_STRING_FUNCS
#include <string>
#endif

namespace mt {
namespace stdlib {

#ifdef MT_STDLIB_USE_STRING_FUNCS
extern const std::string BASE_NAMESPACE;
#endif

enum class DataType {
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

template <typename>
struct type_info {};

template <>
struct type_info<uint8_t> {
    using type_t = uint8_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "uint8_t";
#endif
};

template <>
struct type_info<int8_t> {
    using type_t = int8_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "int8_t";
#endif
};

template <>
struct type_info<uint16_t> {
    using type_t = uint16_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "uint16_t";
#endif
};

template <>
struct type_info<int16_t> {
    using type_t = int16_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "int16_t";
#endif
};

template <>
struct type_info<uint32_t> {
    using type_t = uint32_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "uint32_t";
#endif
};

template <>
struct type_info<int32_t> {
    using type_t = int32_t;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "int32_t";
#endif
};

template <>
struct type_info<float> {
    using type_t = float;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "float";
#endif
};

template <>
struct type_info<double> {
    using type_t = double;
#if MT_STDLIB_USE_STRING_FUNCS
    static constexpr const char* name = "double";
#endif
};

template <>
struct type_info<bool> {
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

#ifdef MT_STDLIB_USE_STRING_FUNCS
    std::string get_type_name(bool include_namespace = true);
#endif

protected:
#ifdef MT_STDLIB_USE_STRING_FUNCS
    virtual std::string get_inner_type_name() const = 0;
#endif
};
#endif

}
}

#endif // MT_STDLIB_TYPES_H
