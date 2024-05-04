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
        DEFAULT = 0,
        SIZE,
        VALUE,
        TIMESTEP,
    };

    BlockInformation(std::string_view name, ConstructorOptions constructor, const block_interface::block_types& types);

    BlockInformation(std::string_view name, std::string_view symbolic_name, ConstructorOptions constructor, const block_interface::block_types& types);

    mt::stdlib::DataType get_default_data_type() const;

    bool type_supported(DataType dt) const;

    std::string name;
    std::optional<std::string> symbolic_name;
    ConstructorOptions constructor;
    block_interface::block_types types;
};

const std::span<const BlockInformation> get_available_blocks();

std::unique_ptr<block_interface> create_block(
    const std::string& name,
    DataType data_type,
    const Argument* argument = nullptr);

std::unique_ptr<block_interface> create_block(
    const BlockInformation& info,
    DataType data_type,
    const Argument* argument = nullptr);

}
}

#endif // MT_STDLIB_CREATION_H
#endif // MT_STDLIB_USE_FULL_LIB
