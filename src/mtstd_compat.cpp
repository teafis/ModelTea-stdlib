// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstd_compat.h"

#include <cstring>
#include <memory>
#include <unordered_map>
#include <utility>

#include "mtstd.hpp"
#include "mtstd_except.hpp"
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
    inner_block_info(const std::string& name, const std::string& display, const std::string& create, const std::string& type)
        : name(name), display(display), create_flags(create), type_flags(type) {}

    const std::string name;
    const std::string display;
    const std::string create_flags;
    const std::string type_flags;
};

const char* MT_TYPE_FLAGS_NUMERIC = "num";
const char* MT_TYPE_FLAGS_FLOAT = "float";
const char* MT_TYPE_FLAGS_ALL = "all";

const char* MT_INIT_DEFAULT = "default";
const char* MT_INIT_SIZE = "size";
const char* MT_INIT_DT = "dt";
const char* MT_INIT_VALUE = "val";

const static std::unordered_map<std::string, inner_block_info> BLK_INFOS{
    {BLK_ARITH_ADD, inner_block_info(BLK_ARITH_ADD, "+", MT_INIT_SIZE, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_ARITH_SUB, inner_block_info(BLK_ARITH_SUB, "-", MT_INIT_SIZE, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_ARITH_MUL, inner_block_info(BLK_ARITH_MUL, "*", MT_INIT_SIZE, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_ARITH_DIV, inner_block_info(BLK_ARITH_DIV, "/", MT_INIT_SIZE, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_ARITH_MOD, inner_block_info(BLK_ARITH_MOD, "%", MT_INIT_SIZE, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_CLOCK, inner_block_info(BLK_CLOCK, "clock", MT_INIT_VALUE, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_CONST, inner_block_info(BLK_CONST, "const", MT_INIT_VALUE, MT_TYPE_FLAGS_ALL)},
    {BLK_DELAY, inner_block_info(BLK_DELAY, "delay", MT_INIT_DEFAULT, MT_TYPE_FLAGS_ALL)},
    {BLK_DERIV, inner_block_info(BLK_DERIV, "deriv", MT_INIT_DT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_INTEG, inner_block_info(BLK_INTEG, "integ", MT_INIT_DT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_SWITCH, inner_block_info(BLK_SWITCH, "switch", MT_INIT_DEFAULT, MT_TYPE_FLAGS_ALL)},
    {BLK_LIMITER, inner_block_info(BLK_LIMITER, "limit", MT_INIT_DEFAULT, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_REL_GT, inner_block_info(BLK_REL_GT, ">", MT_INIT_DEFAULT, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_REL_GEQ, inner_block_info(BLK_REL_GEQ, ">=", MT_INIT_DEFAULT, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_REL_LT, inner_block_info(BLK_REL_LT, "<", MT_INIT_DEFAULT, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_REL_LEQ, inner_block_info(BLK_REL_LEQ, "<=", MT_INIT_DEFAULT, MT_TYPE_FLAGS_NUMERIC)},
    {BLK_REL_EQ, inner_block_info(BLK_REL_EQ, "==", MT_INIT_DEFAULT, MT_TYPE_FLAGS_ALL)},
    {BLK_REL_NEQ, inner_block_info(BLK_REL_NEQ, "!=", MT_INIT_DEFAULT, MT_TYPE_FLAGS_ALL)},
    {BLK_TRIG_SIN, inner_block_info(BLK_TRIG_SIN, "sin", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_TRIG_COS, inner_block_info(BLK_TRIG_COS, "cos", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_TRIG_TAN, inner_block_info(BLK_TRIG_TAN, "tan", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_TRIG_ASIN, inner_block_info(BLK_TRIG_ASIN, "asin", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_TRIG_ACOS, inner_block_info(BLK_TRIG_ACOS, "acos", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_TRIG_ATAN, inner_block_info(BLK_TRIG_ATAN, "atan", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
    {BLK_TRIG_ATAN2, inner_block_info(BLK_TRIG_ATAN2, "atan2", MT_INIT_DEFAULT, MT_TYPE_FLAGS_FLOAT)},
};

const mt_block_info_t* mt_stdlib_info_init() {
    // Initialize several iteration parameters
    mt_block_info_t* first = nullptr;
    mt_block_info_t* current = nullptr;

    for (const auto& kv : BLK_INFOS) {
        // Create the info parameter
        auto info = new mt_block_info_t{};
        info->name = kv.second.name.c_str();
        info->create_flags = kv.second.create_flags.c_str();
        info->type_flags = kv.second.type_flags.c_str();
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
        current = const_cast<mt_block_info_t*>(current->next);
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

static const char* MT_ERROR_NONE = "";
static const char* MT_ERROR_UNKNOWN_DTYPE = "unknown data type";
static const char* MT_ERROR_UNSUPPORTED_DTYPE = "unsupported data type";
static const char* MT_ERROR_DTYPE_IS_NOT_ARITH = "data type is not arithmetic";
static const char* MT_ERROR_UNKNOWN_ERROR = "unknown error";
static const char* MT_ERROR_UNKNOWN_BLOCK_NAME = "unknown block name";
static const char* MT_ERROR_NULLPTR_PROVIDED = "nullptr provided";
static const char* MT_ERROR_BUFFER_TOO_SMALL = "buffer too small";

struct mt_error_message : public mt::stdlib::block_error {
    mt_error_message(const char* err) : mt::stdlib::block_error(err), err{err} {}

    const char* err;
};

struct mt_block_t {
    mt_block_t(mt::stdlib::block_interface* interface) {
        if (interface == nullptr) {
            throw mt_error_message(MT_ERROR_NULLPTR_PROVIDED);
        }
        block = std::unique_ptr<mt::stdlib::block_interface>(interface);
    }

    std::unique_ptr<mt::stdlib::block_interface> block;
};

template <mt::stdlib::DataType DT>
struct model_arith_block : public mt_block_t {
    using data_t = typename mt::stdlib::type_info<DT>::type_t;

    model_arith_block(mt::stdlib::block_interface* interface, size_t size) : mt_block_t(interface) {
        data = std::unique_ptr<data_t[]>(new data_t[size]);
    }

    std::unique_ptr<data_t[]> data;
};

template <template <mt::stdlib::DataType> class BLK, class TYPES, typename... Args>
static mt_block_creation_t create_block_of_type(const uint32_t data_type, Args&&... args) {
    const auto dt = static_cast<mt::stdlib::DataType>(data_type);
    mt::stdlib::block_interface* ptr = nullptr;

    try {
        if constexpr (TYPES::uses_integral) {
            switch (dt) {
            case mt::stdlib::DataType::U8:
                ptr = new BLK<mt::stdlib::DataType::U8>(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::I8:
                ptr = new BLK<mt::stdlib::DataType::I8>(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::U16:
                ptr = new BLK<mt::stdlib::DataType::U16>(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::I16:
                ptr = new BLK<mt::stdlib::DataType::I16>(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::U32:
                ptr = new BLK<mt::stdlib::DataType::U32>(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::I32:
                ptr = new BLK<mt::stdlib::DataType::I32>(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

        if constexpr (TYPES::uses_float) {
            switch (dt) {
            case mt::stdlib::DataType::F32:
                ptr = new BLK<mt::stdlib::DataType::F32>(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::F64:
                ptr = new BLK<mt::stdlib::DataType::F64>(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

        if constexpr (TYPES::uses_logical) {
            switch (dt) {
            case mt::stdlib::DataType::BOOL:
                ptr = new BLK<mt::stdlib::DataType::BOOL>(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

        if (ptr == nullptr) {
            return mt_block_creation_t{
                .block = nullptr,
                .err = MT_ERROR_UNKNOWN_DTYPE,
            };
        } else {
            return mt_block_creation_t{
                .block = new mt_block_t(ptr),
                .err = nullptr,
            };
        }

    } catch (const mt_error_message& err) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = err.err,
        };
    } catch (const mt::stdlib::block_error& err) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = MT_ERROR_UNKNOWN_ERROR,
        };
    }
}

struct StandardBlockFunctor {
    template <mt::stdlib::DataType DT>
    using blk_rel_eq = mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::EQUAL>;

    template <mt::stdlib::DataType DT>
    using blk_rel_neq = mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::NOT_EQUAL>;

    template <mt::stdlib::DataType DT>
    using blk_rel_gt = mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::GREATER_THAN>;

    template <mt::stdlib::DataType DT>
    using blk_rel_geq = mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::GREATER_THAN_EQUAL>;

    template <mt::stdlib::DataType DT>
    using blk_rel_lt = mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::LESS_THAN>;

    template <mt::stdlib::DataType DT>
    using blk_rel_leq = mt::stdlib::relational_block<DT, mt::stdlib::RelationalOperator::LESS_THAN_EQUAL>;

    template <mt::stdlib::DataType DT>
    using blk_trig_sin = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::SIN>;

    template <mt::stdlib::DataType DT>
    using blk_trig_cos = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::COS>;

    template <mt::stdlib::DataType DT>
    using blk_trig_tan = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::TAN>;

    template <mt::stdlib::DataType DT>
    using blk_trig_asin = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::ASIN>;

    template <mt::stdlib::DataType DT>
    using blk_trig_acos = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::ACOS>;

    template <mt::stdlib::DataType DT>
    using blk_trig_atan = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::ATAN>;

    template <mt::stdlib::DataType DT>
    using blk_trig_atan2 = mt::stdlib::trig_block<DT, mt::stdlib::TrigFunction::ATAN2>;

    mt_block_creation_t operator()(const uint64_t data_type, const std::string& name) {
        mt::stdlib::block_interface* inter = nullptr;

        if (name == BLK_DELAY) {
            return create_block_of_type<mt::stdlib::delay_block, mt::stdlib::delay_block_types>(data_type);
        } else if (name == BLK_SWITCH) {
            return create_block_of_type<mt::stdlib::switch_block, mt::stdlib::switch_block_types>(data_type);
        } else if (name == BLK_LIMITER) {
            return create_block_of_type<mt::stdlib::limiter_block, mt::stdlib::limiter_block_types>(data_type);
        } else if (name == BLK_REL_EQ) {
            return create_block_of_type<blk_rel_eq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::EQUAL>>(data_type);
        } else if (name == BLK_REL_NEQ) {
            return create_block_of_type<blk_rel_neq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::NOT_EQUAL>>(data_type);
        } else if (name == BLK_REL_GT) {
            return create_block_of_type<blk_rel_gt, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::GREATER_THAN>>(data_type);
        } else if (name == BLK_REL_GEQ) {
            return create_block_of_type<blk_rel_geq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::GREATER_THAN_EQUAL>>(data_type);
        } else if (name == BLK_REL_LT) {
            return create_block_of_type<blk_rel_lt, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::LESS_THAN>>(data_type);
        } else if (name == BLK_REL_LEQ) {
            return create_block_of_type<blk_rel_leq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::LESS_THAN_EQUAL>>(data_type);
        } else if (name == BLK_TRIG_SIN) {
            return create_block_of_type<blk_trig_sin, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_TRIG_COS) {
            return create_block_of_type<blk_trig_cos, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_TRIG_TAN) {
            return create_block_of_type<blk_trig_tan, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_TRIG_ASIN) {
            return create_block_of_type<blk_trig_asin, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_TRIG_ACOS) {
            return create_block_of_type<blk_trig_acos, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_TRIG_ATAN) {
            return create_block_of_type<blk_trig_atan, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_TRIG_ATAN2) {
            return create_block_of_type<blk_trig_atan2, mt::stdlib::trig_block_types>(data_type);
        } else {
            throw mt_error_message(MT_ERROR_UNKNOWN_BLOCK_NAME);
        }
    }
};

struct DeltaTimeBlockFunctor {
    mt_block_creation_t operator()(const uint64_t data_type, const std::string& name, const double dt) {
        if (name == BLK_DERIV) {
            return create_block_of_type<mt::stdlib::derivative_block, mt::stdlib::derivative_block_types>(data_type, dt);
        } else if (name == BLK_INTEG) {
            return create_block_of_type<mt::stdlib::integrator_block, mt::stdlib::integrator_block_types>(data_type, dt);
        } else {
            throw mt_error_message(MT_ERROR_UNKNOWN_BLOCK_NAME);
        }
    }
};

template <mt::stdlib::DataType DT>
struct ValueBlockFunctor {
    mt_block_t* operator()(const uint64_t data_type, const std::string& name, const mt_value_t* value) {
        using data_t = typename mt::stdlib::type_info<DT>::type_t;
        if (value == nullptr || value->data == nullptr || value->size != sizeof(data_t) || static_cast<mt::stdlib::DataType>(value->type) != DT) {
            throw mt_error_message(MT_ERROR_NULLPTR_PROVIDED);
        }

        const auto data = *static_cast<const data_t*>(value->data);

        if (name == BLK_CONST) {
            return new mt_block_t(new mt::stdlib::const_block<DT>(data));
        } else if (name == BLK_CLOCK) {
            if constexpr (mt::stdlib::type_info<DT>::is_numeric) {
                return new mt_block_t(new mt::stdlib::clock_block<DT>(data));
            } else {
                throw mt_error_message(MT_ERROR_UNSUPPORTED_DTYPE);
            }
        } else {
            throw mt_error_message(MT_ERROR_UNKNOWN_BLOCK_NAME);
        }
    }
};

template <mt::stdlib::DataType DT>
struct ArithmeticBlockFunctor {
    template <mt::stdlib::ArithType AT>
    static mt_block_t* create_arith_block(const size_t size) {
        if constexpr (mt::stdlib::type_info<DT>::is_numeric) {
            auto arith = new mt::stdlib::arith_block_dynamic<DT, AT>();
            auto ptr = new model_arith_block<DT>(arith, size);
            arith->s_in.size = size;
            arith->s_in.values = ptr->data.get();
            return ptr;
        } else {
            throw mt_error_message(MT_ERROR_DTYPE_IS_NOT_ARITH);
        }
    }

    mt_block_t* operator()(const std::string& name, const size_t size) {
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
            throw mt_error_message(MT_ERROR_UNKNOWN_BLOCK_NAME);
        }
    }
};

template <template <mt::stdlib::DataType> class FCN, bool USE_INTEG, bool USE_FLOAT, bool USE_OTHER, typename... Args>
static mt_block_creation_t create_block_with_type_inner(const uint32_t data_type, Args&&... args) {
    const auto dt = static_cast<mt::stdlib::DataType>(data_type);
    mt_block_t* ptr = nullptr;

    try {
        if constexpr (USE_INTEG) {
            switch (dt) {
            case mt::stdlib::DataType::U8:
                ptr = FCN<mt::stdlib::DataType::U8>()(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::I8:
                ptr = FCN<mt::stdlib::DataType::I8>()(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::U16:
                ptr = FCN<mt::stdlib::DataType::U16>()(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::I16:
                ptr = FCN<mt::stdlib::DataType::I16>()(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::U32:
                ptr = FCN<mt::stdlib::DataType::U32>()(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::I32:
                ptr = FCN<mt::stdlib::DataType::I32>()(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

        if constexpr (USE_FLOAT) {
            switch (dt) {
            case mt::stdlib::DataType::F32:
                ptr = FCN<mt::stdlib::DataType::F32>()(std::forward<Args>(args)...);
                break;
            case mt::stdlib::DataType::F64:
                ptr = FCN<mt::stdlib::DataType::F64>()(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

        if constexpr (USE_OTHER) {
            switch (dt) {
            case mt::stdlib::DataType::BOOL:
                ptr = FCN<mt::stdlib::DataType::BOOL>()(std::forward<Args>(args)...);
                break;
            default:
                break;
            }
        }

        if (ptr == nullptr) {
            return mt_block_creation_t{
                .block = nullptr,
                .err = MT_ERROR_UNKNOWN_DTYPE,
            };
        } else {
            return mt_block_creation_t{
                .block = ptr,
                .err = nullptr,
            };
        }

    } catch (const mt_error_message& err) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = err.err,
        };
    }
}

mt_block_creation_t mt_stdlib_blk_create(const char* name, uint32_t data_type) {
    auto it = BLK_INFOS.find(name);
    if (it == BLK_INFOS.end()) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = MT_ERROR_UNKNOWN_BLOCK_NAME,
        };
    }

    try {
        return StandardBlockFunctor()(data_type, name);
    } catch (const mt_error_message& msg) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = msg.err};
    } catch (const mt::stdlib::block_error&) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = MT_ERROR_UNKNOWN_ERROR};
    }
}

mt_block_creation_t mt_stdlib_blk_create_with_size(const char* name, uint32_t data_type, uint32_t size) {
    return create_block_with_type_inner<ArithmeticBlockFunctor, true, true, false>(data_type, name, size);
}

mt_block_creation_t mt_stdlib_blk_create_with_value(const char* name, const mt_value_t* value) {
    if (value == nullptr) {
        return mt_block_creation_t{
            .block = nullptr,
            .err = MT_ERROR_NULLPTR_PROVIDED,
        };
    }

    return create_block_with_type_inner<ValueBlockFunctor, true, true, true>(value->type, value->type, name, value);
}

mt_block_creation_t mt_stdlib_blk_create_with_time_step(const char* name, uint32_t data_type, double dt) {
    return DeltaTimeBlockFunctor()(data_type, name, dt);
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
        blk->block.reset();
    }
}

static uint32_t block_get_name(const mt_block_t* blk, char* str, const uint32_t buffer_size, const bool include_namespace) {
    if (blk == nullptr) {
        return 0;
    }

    const auto name = blk->block->get_type_name(include_namespace);
    if (name.size() + 1 < buffer_size) {
        const auto last = std::strncpy(str, name.c_str(), std::min(static_cast<uint32_t>(name.size() + 1), buffer_size));
        return last - str;
    } else {
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
        blk->block->set_input(port_num, value);
        return 1;
    } catch (const mt::stdlib::block_error&) {
        return 0;
    }
}

int32_t mt_stdlib_blk_get_output(const mt_block_t* blk, uint32_t port_num, mt_value_t* value) {
    if (blk == nullptr || value == nullptr) {
        return 0;
    }

    try {
        blk->block->get_output(port_num, value);
        return 1;
    } catch (const mt::stdlib::block_error&) {
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
