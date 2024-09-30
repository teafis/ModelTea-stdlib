// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB
#ifndef MT_STDLIB_CREATION_H
#define MT_STDLIB_CREATION_H

#include <cstdint>
#include <memory>
#include <optional>
#include <span>
#include <string>
#include <vector>

#include "mtstdlib_types.hpp"

namespace mt {
namespace stdlib {

struct BlockInformation {
    enum class ConstructorOptions : int32_t {
        NONE = 0,
        SIZE,
        VALUE,
        VALUE_PTR,
        TIMESTEP,
    };

    BlockInformation(std::string_view name, ConstructorOptions constructor, const block_interface::block_types& types);

    BlockInformation with_uses_input_as_type(bool val) const;

    BlockInformation with_constructor_codegen(ConstructorOptions options) const;

    BlockInformation with_symbolic_name(std::optional<std::string> name) const;

    BlockInformation with_required_type_count(size_t count) const;

    mt::stdlib::DataType get_default_data_type() const;

    bool type_supported(DataType dt) const;

    std::string name;
    std::optional<std::string> symbolic_name;
    ConstructorOptions constructor_dynamic;
    ConstructorOptions constructor_codegen;
    block_interface::block_types types;

    size_t required_type_count;
    bool uses_input_as_type;
};

const std::span<const BlockInformation> get_available_blocks();

std::unique_ptr<block_interface> create_block(
    const std::string& name,
    std::span<const DataType> data_type,
    const Argument* argument = nullptr);

std::unique_ptr<block_interface> create_block(
    const BlockInformation& info,
    std::span<const DataType> data_type,
    const Argument* argument = nullptr);

}
}

#endif // MT_STDLIB_CREATION_H
#endif // MT_STDLIB_USE_FULL_LIB
