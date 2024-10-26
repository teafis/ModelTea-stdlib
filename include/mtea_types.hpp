// SPDX-License-Identifier: MIT

#ifndef MTEA_TYPES_H
#define MTEA_TYPES_H

#include <cstdint>

#ifdef MTEA_USE_FULL_LIB
#include "mtea_except.hpp"

#include <cstring>
#include <string>
#include <string_view>
#include <vector>
#endif

namespace mtea {

enum class DataType : uint32_t {
    NONE = 0,
    BOOL,
    U8,
    I8,
    U16,
    I16,
    U32,
    I32,
    U64,
    I64,
    F32,
    F64,
};

template <DataType>
struct type_info {};

#ifdef MTEA_USE_FULL_LIB
struct type_info_meta {
    virtual ~type_info_meta() = default;
    virtual bool get_is_numeric() const = 0;
    virtual bool get_is_integral() const = 0;
    virtual bool get_is_float() const = 0;
    virtual bool get_is_signed() const = 0;
    virtual const char* get_name() const = 0;
    virtual DataType get_data_type() const = 0;
};

#define MTEA_TYPE_BASE : type_info_meta
const type_info_meta* get_meta_type(DataType dt);
const type_info_meta* get_meta_type(std::string_view s);
std::vector<const type_info_meta*> get_meta_types();
#else
#define MTEA_TYPE_BASE
#endif

template <>
struct type_info<DataType::U8> MTEA_TYPE_BASE {
    using type_t = uint8_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::I8> MTEA_TYPE_BASE {
    using type_t = int8_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::U16> MTEA_TYPE_BASE {
    using type_t = uint16_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::I16> MTEA_TYPE_BASE {
    using type_t = int16_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::U32> MTEA_TYPE_BASE {
    using type_t = uint32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::I32> MTEA_TYPE_BASE {
    using type_t = int32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::U64> MTEA_TYPE_BASE {
    using type_t = uint32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MTEA_USE_FULL_LIB
    static constexpr const char* name = "uint64_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::U64; }
#endif
};

template <>
struct type_info<DataType::I64> MTEA_TYPE_BASE {
    using type_t = int32_t;
    static const bool is_numeric = true;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = true;
#ifdef MTEA_USE_FULL_LIB
    static constexpr const char* name = "int64_t";

    bool get_is_numeric() const override { return is_numeric; }
    bool get_is_integral() const override { return is_integral; }
    bool get_is_float() const override { return is_float; }
    bool get_is_signed() const override { return is_signed; }
    const char* get_name() const override { return name; }
    DataType get_data_type() const override { return DataType::I64; }
#endif
};

template <>
struct type_info<DataType::F32> MTEA_TYPE_BASE {
    using type_t = float;
    static const bool is_numeric = true;
    static const bool is_integral = false;
    static const bool is_float = true;
    static const bool is_signed = true;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::F64> MTEA_TYPE_BASE {
    using type_t = double;
    static const bool is_numeric = true;
    static const bool is_integral = false;
    static const bool is_float = true;
    static const bool is_signed = true;
#ifdef MTEA_USE_FULL_LIB
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
struct type_info<DataType::BOOL> MTEA_TYPE_BASE {
    using type_t = bool;
    static const bool is_numeric = false;
    static const bool is_integral = true;
    static const bool is_float = false;
    static const bool is_signed = false;
#ifdef MTEA_USE_FULL_LIB
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

#ifdef MTEA_USE_FULL_LIB

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

template <DataType DT>
struct ArgumentPtr final : public Argument {
    using data_t = typename type_info<DT>::type_t;

    ArgumentPtr() : value{nullptr} {}
    ArgumentPtr(const data_t* val) : value{val} {}

    DataType get_type() const override {
        return DT;
    }

    size_t as_size() const override {
        throw block_error("unable to convert data type to size");
    }

    const data_t* value;
};

struct block_interface {
    struct block_types {
        bool uses_integral{false};
        bool uses_float{false};
        bool uses_logical{false};
    };

    virtual ~block_interface() = default;

    virtual block_types get_supported_types() const noexcept = 0;

    virtual void reset() noexcept;

    virtual void step() noexcept;

    virtual DataType get_current_type() const noexcept = 0;

    virtual void set_input(size_t port_num, const Argument* value) = 0;

    virtual void get_output(size_t port_num, Argument* value) const = 0;

    virtual size_t get_input_num() const noexcept = 0;

    virtual size_t get_output_num() const noexcept = 0;

    virtual DataType get_input_type(size_t port_num) const = 0;

    virtual DataType get_output_type(size_t port_num) const = 0;

    virtual bool get_input_type_settable(size_t port_num) const noexcept = 0;

    virtual bool outputs_are_delayed() const noexcept;

    virtual std::string get_input_name(size_t port_num) const = 0;

    virtual std::string get_output_name(size_t port_num) const = 0;

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
    static typename type_info<DT>::type_t* get_model_value_ptr(const Argument* value) {
        using data_t = typename type_info<DT>::type_t;
        const auto cast_val = dynamic_cast<const ArgumentPtr<DT>*>(value);

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
    std::string get_type_name(bool use_codegen_name = true) const;

    virtual std::string get_block_name() const = 0;

protected:
    virtual std::string get_class_name() const = 0;

    virtual std::string get_class_name_codegen() const;
};

#endif // MTEA_USE_FULL_LIB

}

#endif // MTEA_TYPES_H
