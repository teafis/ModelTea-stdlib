#include "mtstd_compat.h"

#include <cstring>
#include <memory>

#include "mtstd.hpp"
#include "mtstd_types.hpp"

struct model_block {
    model_block(mt::stdlib::block_interface* interface) {
        block = std::unique_ptr<mt::stdlib::block_interface>(interface);
    }

    std::unique_ptr<mt::stdlib::block_interface> block;
};

template <mt::stdlib::DataType DT>
struct model_arith_block : public model_block {
    using data_t = typename mt::stdlib::type_info<DT>::type_t;

    model_arith_block(mt::stdlib::block_interface* interface, size_t size) : model_block(interface) {
        data = std::unique_ptr<data_t[]>(new data_t[size]);
    }

    std::unique_ptr<data_t[]> data;
};

template <mt::stdlib::DataType DT, mt::stdlib::ArithType AT>
static model_block* create_arith_block(const size_t size) {
    auto arith = new mt::stdlib::arith_block_dynamic<DT, AT>();
    auto ptr = new model_arith_block<DT>(arith, size);
    arith->s_in.size = size;
    arith->s_in.values = ptr->data.get();
    return ptr;
}

template <mt::stdlib::DataType DT>
static model_block* create_block(const std::string& name, const size_t size) {
    if (name == "add") {
        return create_arith_block<DT, mt::stdlib::ArithType::ADD>(size);
    } else if (name == "sub") {
        return create_arith_block<DT, mt::stdlib::ArithType::SUB>(size);
    } else if (name == "mul") {
        return create_arith_block<DT, mt::stdlib::ArithType::MUL>(size);
    } else if (name == "div") {
        return create_arith_block<DT, mt::stdlib::ArithType::DIV>(size);
    } else if (name == "mod") {
        return create_arith_block<DT, mt::stdlib::ArithType::MOD>(size);
    } else if (name == "clock") {
        if (DT == mt::stdlib::DataType::F64) {
            return new model_block(new mt::stdlib::clock_block(0.01)); // TODO - DT!
        } else {
            return nullptr;
        }
    } else {
        return nullptr;
    }
}

model_block* mt_stdlib_create_with_size(const char* name, uint32_t data_type, uint32_t size) {
    const auto dt = static_cast<mt::stdlib::DataType>(data_type);

    switch (dt) {
    case mt::stdlib::DataType::U8:
        return create_block<mt::stdlib::DataType::U8>(name, size);
    case mt::stdlib::DataType::I8:
        return create_block<mt::stdlib::DataType::I8>(name, size);
    case mt::stdlib::DataType::U16:
        return create_block<mt::stdlib::DataType::U16>(name, size);
    case mt::stdlib::DataType::I16:
        return create_block<mt::stdlib::DataType::I16>(name, size);
    case mt::stdlib::DataType::U32:
        return create_block<mt::stdlib::DataType::U32>(name, size);
    case mt::stdlib::DataType::I32:
        return create_block<mt::stdlib::DataType::I32>(name, size);
    case mt::stdlib::DataType::F32:
        return create_block<mt::stdlib::DataType::F32>(name, size);
    case mt::stdlib::DataType::F64:
        return create_block<mt::stdlib::DataType::F64>(name, size);
    case mt::stdlib::DataType::BOOL:
        return create_block<mt::stdlib::DataType::BOOL>(name, size);
    default:
        return nullptr;
    }
}

void mt_stdlib_destroy(const model_block* blk) {
    if (blk != nullptr) {
        delete blk;
    }
}

void mt_stdlib_step(model_block* blk) {
    if (blk != nullptr) {
        blk->block->step();
    }
}

void mt_stdlib_reset(model_block* blk) {
    if (blk != nullptr) {
        blk->block.reset();
    }
}

#ifdef MT_STDLIB_USE_STRING_FUNCS
int32_t mt_stdlib_get_class_name(const model_block* blk, char* str, uint32_t buffer_size);
int32_t mt_stdlib_get_full_name(const model_block* blk, char* str, uint32_t buffer_size);

static int32_t block_get_name(const model_block* blk, char* str, const uint32_t buffer_size, const bool include_namespace) {
    if (blk == nullptr) {
        return 0;
    }

    const auto name = blk->block->get_type_name(include_namespace);
    if (name.size() + 1 < buffer_size) {
        auto last = std::strncpy(str, name.c_str(), std::min(static_cast<uint32_t>(name.size() + 1), buffer_size));
        return last - str;
    } else {
        return 0;
    }
}

int32_t mt_stdlib_get_class_name(const model_block* blk, char* str, const uint32_t buffer_size) {
    return block_get_name(blk, str, buffer_size, false);
}

int32_t mt_stdlib_get_full_name(const model_block* blk, char* str, const uint32_t buffer_size) {
    return block_get_name(blk, str, buffer_size, true);
}
#endif

static int32_t get_dt_size(const mt::stdlib::DataType dt) {
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

int32_t mt_stdlib_set_input(model_block* blk, uint32_t port_num, uint32_t data_type, const void* data, uint32_t data_size) {
    if (blk == nullptr) {
        return 0;
    }

    const auto dt = static_cast<mt::stdlib::DataType>(data_type);

    if (get_dt_size(dt) != data_size) {
        return 0;
    }

    return blk->block->set_input_type(port_num, dt, data, data_size) ? 1 : 0;
}

int32_t mt_stdlib_get_output(const model_block* blk, uint32_t port_num, uint32_t data_type, void* data, uint32_t data_size) {
    return 0;
}

int32_t mt_stdlib_get_num_inputs(const model_block* blk, uint32_t* num);

int32_t mt_stdlib_get_num_outputs(const model_block* blk, uint32_t* num);

int32_t mt_stdlib_get_input_type(const model_block* blk, uint32_t* data_type);

int32_t mt_stdlib_get_output_type(const model_block* blk, uint32_t* data_type);
