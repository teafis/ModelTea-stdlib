#include "mtstd_compat.h"

#include <cstring>
#include <memory>

#include "mtstd.hpp"
#include "mtstd_types.hpp"

struct model_block {
    std::unique_ptr<mt::stdlib::block_interface> block;
};

const model_block* mt_stdlib_create_block(const char* name, int data_type) {
    return nullptr;
}

void mt_stdlib_block_destory(const model_block* blk) {
    if (blk != nullptr) {
        delete blk;
    }
}

void mt_stdlib_block_step(model_block* blk) {
    if (blk != nullptr) {
        blk->block->step();
    }
}

void mt_stdlib_block_reset(model_block* blk) {
    if (blk != nullptr) {
        blk->block.reset();
    }
}

#ifdef MT_STDLIB_USE_STRING_FUNCS
int mt_stdlib_block_get_class_name(const model_block* blk, char* str, int buffer_size);
int mt_stdlib_block_get_full_name(const model_block* blk, char* str, int buffer_size);

static int block_get_name(const model_block* blk, char* str, const int buffer_size, const bool include_namespace) {
    if (blk == nullptr) {
        return 0;
    }

    const auto name = blk->block->get_type_name(include_namespace);
    if (name.size() + 1 < buffer_size) {
        auto last = std::strncpy(str, name.c_str(), std::min(static_cast<int>(name.size() + 1), buffer_size));
        return last - str;
    } else {
        return 0;
    }
}

int mt_stdlib_block_get_class_name(const model_block* blk, char* str, const int buffer_size) {
    return block_get_name(blk, str, buffer_size, false);
}

int mt_stdlib_block_get_full_name(const model_block* blk, char* str, const int buffer_size) {
    return block_get_name(blk, str, buffer_size, true);
}
#endif

static int get_dt_size(const mt::stdlib::DataType dt) {
    switch (dt) {
    case mt::stdlib::DataType::BOOL:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::BOOL>::type_t);
    case mt::stdlib::DataType::U8:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::U8>::type_t);
    case mt::stdlib::DataType::I8:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::I8>::type_t);
    case mt::stdlib::DataType::U16:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::U16>::type_t);
    case mt::stdlib::DataType::I16:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::I16>::type_t);
    case mt::stdlib::DataType::U32:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::U32>::type_t);
    case mt::stdlib::DataType::I32:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::I32>::type_t);
    case mt::stdlib::DataType::F32:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::F32>::type_t);
    case mt::stdlib::DataType::F64:
        return sizeof(mt::stdlib::type_info<mt::stdlib::DataType::F64>::type_t);
    default:
        return 0;
    }
}

int mt_stdlib_set_input(model_block* blk, int port_num, int data_type, const void* data, int data_size) {
    if (blk == nullptr) {
        return 0;
    }

    const auto dt = static_cast<mt::stdlib::DataType>(data_type);

    if (get_dt_size(dt) != data_size) {
        return 0;
    }

    return blk->block->set_input_type(port_num, dt, data, data_size) ? 1 : 0;
}

int mt_stdlib_get_output(const model_block* blk, int port_num, int data_type, void* data, int data_size) {
    return 0;
}
