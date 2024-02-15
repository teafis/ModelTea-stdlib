// SPDX-License-Identifier: MIT

#include "mtstd_compat_types.h"

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstd_compat.h"

#include <cstring>
#include <memory>
#include <unordered_map>
#include <utility>

#include "mtstd.hpp"
#include "mtstd_creation.hpp"
#include "mtstd_except.hpp"
#include "mtstd_types.hpp"


struct mt_block_t {
    mt_block_t(std::unique_ptr<mt::stdlib::block_interface>&& interface) : block(std::move(interface)){
        if (block == nullptr) {
            throw mt::stdlib::block_error("unable to get block result");
        }
    }

    const std::unique_ptr<mt::stdlib::block_interface> block;
};

const int32_t MT_TYPE_FLAGS_NONE = static_cast<int32_t>(mt::stdlib::BlockInformation::TypeOptions::NONE);
const int32_t MT_TYPE_FLAGS_BOOL = static_cast<int32_t>(mt::stdlib::BlockInformation::TypeOptions::BOOL);
const int32_t MT_TYPE_FLAGS_INTEGRAL = static_cast<int32_t>(mt::stdlib::BlockInformation::TypeOptions::INTEGRAL);
const int32_t MT_TYPE_FLAGS_NUMERIC = static_cast<int32_t>(mt::stdlib::BlockInformation::TypeOptions::NUMERIC);
const int32_t MT_TYPE_FLAGS_FLOAT = static_cast<int32_t>(mt::stdlib::BlockInformation::TypeOptions::FLOAT);
const int32_t MT_TYPE_FLAGS_ALL = static_cast<int32_t>(mt::stdlib::BlockInformation::TypeOptions::ALL);

const int32_t MT_INIT_DEFAULT = static_cast<int32_t>(mt::stdlib::BlockInformation::ConstructorOptions::DEFAULT);
const int32_t MT_INIT_SIZE = static_cast<int32_t>(mt::stdlib::BlockInformation::ConstructorOptions::SIZE);
const int32_t MT_INIT_VALUE = static_cast<int32_t>(mt::stdlib::BlockInformation::ConstructorOptions::VALUE);


const mt_block_info_t* mt_stdlib_info_init() {
    // Initialize several iteration parameters
    std::vector<std::unique_ptr<mt_block_info_t>> blocks;

    thread_local const std::vector<std::string> EMPTY_NAME = { "" };

    // Create a block for each element
    for (const auto& base_info : mt::stdlib::get_available_blocks()) {
        // Create the info parameter
        auto info = std::make_unique<mt_block_info_t>();
        info->name = base_info.base_name.c_str();
        info->sub_name = base_info.sub_name.c_str();

        switch (base_info.constructor)
        {
            using enum mt::stdlib::BlockInformation::ConstructorOptions;
        case DEFAULT:
            info->create_flags = MT_INIT_DEFAULT;
            break;
        case VALUE:
            info->create_flags = MT_INIT_VALUE;
            break;
        case SIZE:
            info->create_flags = MT_INIT_SIZE;
            break;
        default:
            return nullptr;
        }

        switch (base_info.types)
        {
            using enum mt::stdlib::BlockInformation::TypeOptions;
        case NONE:
            info->type_flags = MT_TYPE_FLAGS_NONE;
            break;
        case FLOAT:
            info->type_flags = MT_TYPE_FLAGS_FLOAT;
            break;
        case INTEGRAL:
            info->type_flags = MT_TYPE_FLAGS_INTEGRAL;
            break;
        case NUMERIC:
            info->type_flags = MT_TYPE_FLAGS_NUMERIC;
            break;
        case BOOL:
            info->type_flags = MT_TYPE_FLAGS_BOOL;
            break;
        case ALL:
            info->type_flags = MT_TYPE_FLAGS_ALL;
            break;
        default:
            return nullptr;
        }

        info->next = nullptr;
        blocks.emplace_back(std::move(info));
    }

    // Combine into a linked list
    mt_block_info_t* first = nullptr;
    mt_block_info_t* last = nullptr;

    for (auto& ptr : blocks)
    {
        mt_block_info_t* current = ptr.release();
        current->next = nullptr;

        if (first == nullptr)
        {
            first = current;
        }

        if (last != nullptr)
        {
            last->next = current;
        }

        last = current;
    }

    // Return the result
    return first;
}

void mt_stdlib_info_destroy(const mt_block_info_t* info) {
    // Create an iteration segment, to delete the list front-to-back
    mt_block_info_t* current = const_cast<mt_block_info_t*>(info);

    // Loop while not empty
    while (current != nullptr) {
        // Create a pointer to the next before we delete the current
        auto next = current->next;

        // Delete the current value, then set to the next pointer in the list
        delete current;
        current = const_cast<mt_block_info_t*>(next);
    }
}

uint32_t mt_stdlib_type_size(uint32_t data_type) {
    switch (static_cast<mt::stdlib::DataType>(data_type)) {
    case mt::stdlib::DataType::U8:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::U8>::type_t);
    case mt::stdlib::DataType::U16:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::U16>::type_t);
    case mt::stdlib::DataType::U32:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::U32>::type_t);
    case mt::stdlib::DataType::I8:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::I8>::type_t);
    case mt::stdlib::DataType::I16:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::I16>::type_t);
    case mt::stdlib::DataType::I32:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::I32>::type_t);
    case mt::stdlib::DataType::F32:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::F32>::type_t);
    case mt::stdlib::DataType::F64:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::F64>::type_t);
    case mt::stdlib::DataType::BOOL:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::BOOL>::type_t);
    default:
        return 0;
    }
}

template <mt::stdlib::DataType DT>
static std::unique_ptr<mt::stdlib::ArgumentBox<DT>> make_arg_type(const mt_value_t& value)
{
    using namespace mt::stdlib;

    if (value.size != mt_stdlib_type_size(value.type) || static_cast<DataType>(value.type) != DT || value.data == nullptr)
    {
        return nullptr;
    }
    else
    {
        return std::make_unique<ArgumentBox<DT>>(*static_cast<const ArgumentBox<DT>::data_t*>(value.data));
    }
}

static std::unique_ptr<mt::stdlib::ArgumentValue> make_arg(const mt_value_t& value)
{
    using namespace mt::stdlib;

    DataType dt = static_cast<DataType>(value.type);

    switch (dt)
    {
        using enum DataType;
    case BOOL:
        return make_arg_type<BOOL>(value);
    case U8:
        return make_arg_type<U8>(value);
    case I8:
        return make_arg_type<I8>(value);
    case U16:
        return make_arg_type<U16>(value);
    case I16:
        return make_arg_type<I16>(value);
    case U32:
        return make_arg_type<U32>(value);
    case I32:
        return make_arg_type<I32>(value);
    case F32:
        return make_arg_type<F32>(value);
    case F64:
        return make_arg_type<F64>(value);
    default:
        return nullptr;
    }
}

template <mt::stdlib::DataType DT>
static bool set_arg_type(const mt_value_t& value, const mt::stdlib::ArgumentValue* arg)
{
    using namespace mt::stdlib;
    auto cast_arg = dynamic_cast<const mt::stdlib::ArgumentBox<DT>*>(arg);

    if (value.size != mt_stdlib_type_size(value.type) || static_cast<DataType>(value.type) != DT || value.data == nullptr || cast_arg == nullptr)
    {
        return false;
    }
    else
    {
        *static_cast<typename type_info<DT>::type_t*>(value.data) = cast_arg->value;
        return true;
    }
}

static bool set_arg(mt_value_t& value, const mt::stdlib::ArgumentValue* arg)
{
    using namespace mt::stdlib;

    DataType dt = static_cast<DataType>(value.type);

    switch (dt)
    {
        using enum DataType;
    case BOOL:
        return set_arg_type<BOOL>(value, arg);
    case U8:
        return set_arg_type<U8>(value, arg);
    case I8:
        return set_arg_type<I8>(value, arg);
    case U16:
        return set_arg_type<U16>(value, arg);
    case I16:
        return set_arg_type<I16>(value, arg);
    case U32:
        return set_arg_type<U32>(value, arg);
    case I32:
        return set_arg_type<I32>(value, arg);
    case F32:
        return set_arg_type<F32>(value, arg);
    case F64:
        return set_arg_type<F64>(value, arg);
    default:
        return false;
    }
}

mt_block_t* mt_stdlib_blk_create(const char* name, const char* sub_name, uint32_t data_type) {
    if (name == nullptr || sub_name == nullptr)
    {
        return nullptr;
    }

    try
    {
        return new mt_block_t(mt::stdlib::create_block(name, sub_name, static_cast<mt::stdlib::DataType>(data_type), nullptr));
    }
    catch (const mt::stdlib::block_error& err)
    {
        return nullptr;
    }
}

mt_block_t* mt_stdlib_blk_create_with_size(const char* name, const char* sub_name, uint32_t data_type, uint32_t size) {
    if (name == nullptr || sub_name == nullptr)
    {
        return nullptr;
    }

    try
    {
        const auto arg = std::make_unique<mt::stdlib::ArgumentBox<mt::stdlib::DataType::U32>>(size);
        return new mt_block_t(mt::stdlib::create_block(name, sub_name, static_cast<mt::stdlib::DataType>(data_type), arg.get()));
    }
    catch (const mt::stdlib::block_error& err)
    {
        return nullptr;
    }
}

mt_block_t* mt_stdlib_blk_create_with_value(const char* name, const char* sub_name, const mt_value_t* value) {
    if (name == nullptr || sub_name == nullptr || value == nullptr)
    {
        return nullptr;
    }

    try
    {
        return new mt_block_t(mt::stdlib::create_block(name, sub_name, static_cast<mt::stdlib::DataType>(value->type), make_arg(*value).get()));
    }
    catch (const mt::stdlib::block_error& err)
    {
        return nullptr;
    }
}

void mt_stdlib_blk_destroy(const mt_block_t* blk) {
    if (blk != nullptr) {
        delete blk;
    }
}

void mt_stdlib_blk_step(mt_block_t* blk) {
    if (blk != nullptr) {
        blk->block->step();
    }
}

void mt_stdlib_blk_reset(mt_block_t* blk) {
    if (blk != nullptr) {
        blk->block->reset();
    }
}

static int32_t block_get_name(const mt_block_t* blk, char* str, const uint32_t buffer_size, const bool include_namespace) {
    if (blk == nullptr) {
        return 0;
    }

    const auto name = blk->block->get_type_name(include_namespace);
    if (name.size() + 1 < buffer_size) {
        const auto last = std::strncpy(str, name.c_str(), std::min(static_cast<uint32_t>(name.size() + 1), buffer_size));
        return static_cast<int32_t>(last - str);
    }
    else {
        return 0;
    }
}

int32_t mt_stdlib_blk_get_class_name(const mt_block_t* blk, char* str, const uint32_t buffer_size) {
    return block_get_name(blk, str, buffer_size, false);
}

int32_t mt_stdlib_blk_get_full_name(const mt_block_t* blk, char* str, const uint32_t buffer_size) {
    return block_get_name(blk, str, buffer_size, true);
}

int32_t mt_stdlib_blk_set_input(mt_block_t* blk, uint32_t port_num, const mt_value_t* value) {
    if (blk == nullptr || value == nullptr) {
        return 0;
    }

    try {
        blk->block->set_input(port_num, make_arg(*value).get());
        return 1;
    }
    catch (const mt::stdlib::block_error&) {
        return 0;
    }
}

int32_t mt_stdlib_blk_get_output(const mt_block_t* blk, uint32_t port_num, mt_value_t* value) {
    if (blk == nullptr || value == nullptr) {
        return 0;
    }

    try {
        auto ptr_val = make_arg(*value);
        blk->block->get_output(port_num, ptr_val.get());
        set_arg(*value, ptr_val.get());
        return 1;
    }
    catch (const mt::stdlib::block_error&) {
        return 0;
    }
}

int32_t mt_stdlib_blk_get_num_inputs(const mt_block_t* blk, uint32_t* num) {
    if (blk == nullptr || num == nullptr) {
        return 0;
    }

    *num = blk->block->get_input_num();
    return 1;
}

int32_t mt_stdlib_blk_get_num_outputs(const mt_block_t* blk, uint32_t* num) {
    if (blk == nullptr || num == nullptr) {
        return 0;
    }

    *num = blk->block->get_output_num();
    return 1;
}

int32_t mt_stdlib_blk_get_input_type(const mt_block_t* blk, const uint32_t port_num, uint32_t* data_type) {
    if (blk == nullptr || data_type == nullptr) {
        return 0;
    }

    *data_type = static_cast<uint32_t>(blk->block->get_input_type(port_num));
    return 1;
}

int32_t mt_stdlib_blk_get_output_type(const mt_block_t* blk, const uint32_t port_num, uint32_t* data_type) {
    if (blk == nullptr || data_type == nullptr) {
        return 0;
    }

    *data_type = static_cast<uint32_t>(blk->block->get_output_type(port_num));
    return 1;
}

#endif // MT_STDLIB_USE_FULL_LIB
