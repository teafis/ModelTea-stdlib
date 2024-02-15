// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB
#ifndef MT_STDLIB_CREATION_H
#define MT_STDLIB_CREATION_H

#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <vector>

#include "mtstd_types.hpp"

namespace mt {
namespace stdlib {

struct BlockInformation final
{
    enum class ConstructorOptions : int32_t
    {
        DEFAULT = 0,
        SIZE = 1 << 0,
        VALUE = 1 << 1,
    };

    enum class TypeOptions : int32_t
    {
        NONE = 0,
        FLOAT = 1 << 0,
        INTEGRAL = 1 << 1,
        NUMERIC = FLOAT | INTEGRAL,
        BOOL = 1 << 2,
        ALL = FLOAT | INTEGRAL | BOOL,
    };

    BlockInformation(std::string_view base_name, std::string_view sub_name, ConstructorOptions constructor, TypeOptions types) :
        base_name(base_name),
        sub_name(sub_name),
        constructor(constructor),
        types(types)
    {
        // Empty Constructor
    }

    std::string base_name;
    std::string sub_name;
    ConstructorOptions constructor;
    TypeOptions types;
};

const std::span<const BlockInformation> get_available_blocks();

std::unique_ptr<block_interface> create_block(
    const std::string& name,
    const std::string& sub_name,
    DataType data_type,
    const ArgumentValue* argument = nullptr);

}
}

#endif // MT_STDLIB_CREATION_H
#endif // MT_STDLIB_USE_FULL_LIB
