// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstd_compat.h"

#include <cstring>
#include <memory>
#include <unordered_map>
#include <utility>

#include "mtstd.hpp"
#include "mtstd_types.hpp"

const static std::string BLK_ARITH_ADD = "arith_add";
const static std::string BLK_ARITH_SUB = "arith_sub";
const static std::string BLK_ARITH_MUL = "arith_mul";
const static std::string BLK_ARITH_DIV = "arith_div";
const static std::string BLK_ARITH_MOD = "arith_mod";
const static std::string BLK_CLOCK = "clock";
const static std::string BLK_CONST = "constant";
const static std::string BLK_DELAY = "delay";
const static std::string BLK_DERIV = "derivative";
const static std::string BLK_INTEG = "integrator";
const static std::string BLK_SWITCH = "switch";
const static std::string BLK_LIMITER = "limiter";
const static std::string BLK_REL_GT = "rel_greater";
const static std::string BLK_REL_GEQ = "rel_greater_eq";
const static std::string BLK_REL_LT = "rel_less";
const static std::string BLK_REL_LEQ = "rel_less_eq";
const static std::string BLK_REL_EQ = "rel_equal";
const static std::string BLK_REL_NEQ = "rel_not_equal";
const static std::string BLK_TRIG_SIN = "trig_sin";
const static std::string BLK_TRIG_COS = "trig_cos";
const static std::string BLK_TRIG_TAN = "trig_tan";
const static std::string BLK_TRIG_ASIN = "trig_asin";
const static std::string BLK_TRIG_ACOS = "trig_acos";
const static std::string BLK_TRIG_ATAN = "trig_atan";
const static std::string BLK_TRIG_ATAN2 = "trig_atan2";

struct inner_block_info {
    inner_block_info(const std::string& name, const std::string& display, const std::string& create) : name(name), create_flags(create) {}

    const std::string name;
    const std::string display;
    const std::string create_flags;
};

const static std::unordered_map<std::string, inner_block_info> BLK_INFOS{
    {BLK_ARITH_ADD, inner_block_info(BLK_ARITH_ADD, "+", "size")},
    {BLK_ARITH_SUB, inner_block_info(BLK_ARITH_SUB, "-", "size")},
    {BLK_ARITH_MUL, inner_block_info(BLK_ARITH_MUL, "*", "size")},
    {BLK_ARITH_DIV, inner_block_info(BLK_ARITH_DIV, "/", "size")},
    {BLK_ARITH_MOD, inner_block_info(BLK_ARITH_MOD, "%", "size")},
    {BLK_CLOCK, inner_block_info(BLK_CLOCK, "clock", "dt")},
    {BLK_CONST, inner_block_info(BLK_CONST, "const", "value")},
    {BLK_DELAY, inner_block_info(BLK_DELAY, "delay", "")},
    {BLK_DERIV, inner_block_info(BLK_DERIV, "deriv", "dt")},
    {BLK_INTEG, inner_block_info(BLK_INTEG, "integ", "dt")},
    {BLK_SWITCH, inner_block_info(BLK_SWITCH, "switch", "")},
    {BLK_LIMITER, inner_block_info(BLK_LIMITER, "limit", "")},
    {BLK_REL_GT, inner_block_info(BLK_REL_GT, ">", "")},
    {BLK_REL_GEQ, inner_block_info(BLK_REL_GEQ, ">=", "")},
    {BLK_REL_LT, inner_block_info(BLK_REL_LT, "<", "")},
    {BLK_REL_LEQ, inner_block_info(BLK_REL_LEQ, "<=", "")},
    {BLK_REL_EQ, inner_block_info(BLK_REL_EQ, "==", "")},
    {BLK_REL_NEQ, inner_block_info(BLK_REL_NEQ, "!=", "")},
    {BLK_TRIG_SIN, inner_block_info(BLK_TRIG_SIN, "trig_sin", "")},
    {BLK_TRIG_COS, inner_block_info(BLK_TRIG_COS, "trig_cos", "")},
    {BLK_TRIG_TAN, inner_block_info(BLK_TRIG_TAN, "trig_tan", "")},
    {BLK_TRIG_ASIN, inner_block_info(BLK_TRIG_ASIN, "trig_asin", "")},
    {BLK_TRIG_ACOS, inner_block_info(BLK_TRIG_ACOS, "trig_acos", "")},
    {BLK_TRIG_ATAN, inner_block_info(BLK_TRIG_ATAN, "trig_atan", "")},
    {BLK_TRIG_ATAN2, inner_block_info(BLK_TRIG_ATAN2, "trig_atan2", "")},
};

const block_info* mt_stdlib_info_init() {
    // Initialize several iteration parameters
    block_info* first = nullptr;
    block_info* current = nullptr;

    for (const auto& kv : BLK_INFOS) {
        // Create the info parameter
        auto info = new block_info{};
        info->name = kv.second.name.c_str();
        info->create_flags = kv.second.create_flags.c_str();
        info->display = kv.second.display.c_str();
        info->next = nullptr;

        // Set the first parameters if on the first loop
        if (first == nullptr) {
            first = info;
            current = info;
        }

        // Add info to the list
        current->next = info;

        // Set current to the next item in the list
        current = const_cast<block_info*>(current->next);
    }

    // Return the result
    return first;
}

int32_t mt_stdlib_info_destroy(const block_info* info) {
    // Create an iteration segment, to delete the list front-to-back
    block_info* current = const_cast<block_info*>(info);

    // Loop while not empty
    while (current != nullptr) {
        // Create a pointer to the next before we delete the current
        auto next = current->next;

        // Delete the current value, then set to the next pointer in the list
        delete current;
        current = const_cast<block_info*>(next);
    }

    return 1;
}

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

template <mt::stdlib::DataType DT>
struct StandardBlockFunctor {
    model_block* operator()(const std::string& name) {
        mt::stdlib::block_interface* inter = nullptr;

        if (name == BLK_DELAY) {
            inter = new mt::stdlib::delay_block<DT>();
        } else if (name == BLK_SWITCH) {
            inter = new mt::stdlib::switch_block<DT>();
        } else if (name == BLK_LIMITER) {
            inter = new mt::stdlib::switch_block<DT>();
        } else if (name == BLK_REL_GT) {
            inter = new mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::GREATER_THAN>();
        } else if (name == BLK_REL_GEQ) {
            inter = new mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::GREATER_THAN_EQUAL>();
        } else if (name == BLK_REL_LT) {
            inter = new mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::LESS_THAN>();
        } else if (name == BLK_REL_LEQ) {
            inter = new mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::LESS_THAN_EQUAL>();
        } else if (name == BLK_REL_EQ) {
            inter = new mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::EQUAL>();
        } else if (name == BLK_REL_NEQ) {
            inter = new mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::NOT_EQUAL>();
        }

        if (inter == nullptr) {
            return nullptr;
        } else {
            return new model_block(inter);
        }
    }
};

template <mt::stdlib::DataType DT>
struct DeltaTimeBlockFunctor {
    model_block* operator()(const std::string& name, const double dt) {
        mt::stdlib::block_interface* inter = nullptr;

        if (name == BLK_CLOCK) {
            if (DT == mt::stdlib::DataType::F64) {
                inter = new mt::stdlib::clock_block(dt);
            }
        } else if (name == BLK_DERIV) {
            inter = new mt::stdlib::derivative_block<DT>(dt);
        } else if (name == BLK_INTEG) {
            inter = new mt::stdlib::integrator_block<DT>(dt);
        }

        if (inter == nullptr) {
            return nullptr;
        } else {
            return new model_block(inter);
        }
    }
};

template <mt::stdlib::DataType DT>
struct ValueBlockFunctor {
    model_block* operator()(const std::string& name, const model_value* value) {
        using data_t = typename mt::stdlib::type_info<DT>::type_t;
        if (value == nullptr || value->data == nullptr || value->size != sizeof(data_t) || static_cast<mt::stdlib::DataType>(value->type) != DT) {
            return nullptr;
        }

        const auto data = *static_cast<const data_t*>(value->data);

        if (name == BLK_CONST) {
            return new model_block(new mt::stdlib::const_block<DT>(data));
        } else {
            return nullptr;
        }
    }
};

template <mt::stdlib::DataType DT>
struct SizeBlockFunctor {
    template <mt::stdlib::ArithType AT>
    static model_block* create_arith_block(const size_t size) {
        if (mt::stdlib::type_info<DT>::is_arith) {
            auto arith = new mt::stdlib::arith_block_dynamic<DT, AT>();
            auto ptr = new model_arith_block<DT>(arith, size);
            arith->s_in.size = size;
            arith->s_in.values = ptr->data.get();
            return ptr;
        } else {
            return nullptr;
        }
    }

    model_block* operator()(const std::string& name, const size_t size) {
        if (name == BLK_ARITH_ADD) {
            return create_arith_block<mt::stdlib::ArithType::ADD>(size);
        } else if (name == BLK_ARITH_SUB) {
            return create_arith_block<mt::stdlib::ArithType::SUB>(size);
        } else if (name == BLK_ARITH_MUL) {
            return create_arith_block<mt::stdlib::ArithType::MUL>(size);
        } else if (name == BLK_ARITH_DIV) {
            return create_arith_block<mt::stdlib::ArithType::DIV>(size);
        } else if (name == BLK_ARITH_MOD) {
            return create_arith_block<mt::stdlib::ArithType::MOD>(size);
        } else {
            return nullptr;
        }
    }
};

template <template <mt::stdlib::DataType> class FCN, typename... Args>
model_block* create_block_with_type(const uint32_t data_type, Args&&... args) {
    const auto dt = static_cast<mt::stdlib::DataType>(data_type);

    switch (dt) {
    case mt::stdlib::DataType::U8:
        return FCN<mt::stdlib::DataType::U8>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::I8:
        return FCN<mt::stdlib::DataType::I8>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::U16:
        return FCN<mt::stdlib::DataType::U16>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::I16:
        return FCN<mt::stdlib::DataType::U16>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::U32:
        return FCN<mt::stdlib::DataType::U32>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::I32:
        return FCN<mt::stdlib::DataType::I32>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::F32:
        return FCN<mt::stdlib::DataType::F32>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::F64:
        return FCN<mt::stdlib::DataType::F64>()(std::forward<Args>(args)...);
    case mt::stdlib::DataType::BOOL:
        return FCN<mt::stdlib::DataType::BOOL>()(std::forward<Args>(args)...);
    default:
        return nullptr;
    }
}

model_block* mt_stdlib_create(const char* name, uint32_t data_type) {
    return create_block_with_type<StandardBlockFunctor>(data_type, name);
}

model_block* mt_stdlib_create_with_size(const char* name, uint32_t data_type, uint32_t size) {
    return create_block_with_type<SizeBlockFunctor>(data_type, name, size);
}

model_block* mt_stdlib_create_with_value(const char* name, const model_value* value) {
    if (value == nullptr) {
        return nullptr;
    }
    return create_block_with_type<ValueBlockFunctor>(value->type, name, value);
}

model_block* mt_stdlib_create_with_time_step(const char* name, uint32_t data_type, double dt) {
    return create_block_with_type<DeltaTimeBlockFunctor>(data_type, name, dt);
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

int32_t mt_stdlib_set_input(model_block* blk, uint32_t port_num, const model_value* value) {
    if (blk == nullptr || value == nullptr) {
        return 0;
    }

    const auto dt = static_cast<mt::stdlib::DataType>(value->type);

    if (get_dt_size(dt) != value->size) {
        return 0;
    }

    return blk->block->set_input(port_num, dt, value->data, value->size) ? 1 : 0;
}

int32_t mt_stdlib_get_output(const model_block* blk, uint32_t port_num, model_value* value) {
    if (blk == nullptr || value == nullptr) {
        return 0;
    }

    const auto dt = static_cast<mt::stdlib::DataType>(value->type);

    if (get_dt_size(dt) != value->size) {
        return 0;
    }

    return blk->block->get_output(port_num, dt, value->data, value->size);
}

int32_t mt_stdlib_get_num_inputs(const model_block* blk, uint32_t* num) {
    if (blk == nullptr || num == nullptr) {
        return 0;
    }

    *num = blk->block->get_input_num();
    return 1;
}

int32_t mt_stdlib_get_num_outputs(const model_block* blk, uint32_t* num) {
    if (blk == nullptr || num == nullptr) {
        return 0;
    }

    *num = blk->block->get_output_num();
    return 1;
}

int32_t mt_stdlib_get_input_type(const model_block* blk, const uint32_t port_num, uint32_t* data_type) {
    if (blk == nullptr || data_type == nullptr) {
        return 0;
    }

    *data_type = static_cast<uint32_t>(blk->block->get_input_type(port_num));
    return 1;
}

int32_t mt_stdlib_get_output_type(const model_block* blk, const uint32_t port_num, uint32_t* data_type) {
    if (blk == nullptr || data_type == nullptr) {
        return 0;
    }

    *data_type = static_cast<uint32_t>(blk->block->get_output_type(port_num));
    return 1;
}

#endif // MT_STDLIB_USE_FULL_LIB
