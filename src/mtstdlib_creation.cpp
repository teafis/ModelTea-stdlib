// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstdlib_creation.hpp"

#include "mtstdlib_except.hpp"
#include "mtstdlib_string.hpp"

#include "mtstdlib.hpp"

#include <algorithm>
#include <array>
#include <sstream>
#include <unordered_map>

mt::stdlib::BlockInformation::BlockInformation(
    std::string_view name,
    ConstructorOptions constructor,
    const block_interface::block_types& types)
    : name(name),
      symbolic_name{},
      constructor_dynamic(constructor),
      constructor_codegen(constructor),
      types(types),
      required_type_count(1),
      uses_input_as_type(true) {}

mt::stdlib::BlockInformation mt::stdlib::BlockInformation::with_uses_input_as_type(bool val) const {
    auto tmp = *this;
    tmp.uses_input_as_type = val;
    return tmp;
}

mt::stdlib::BlockInformation mt::stdlib::BlockInformation::with_constructor_codegen(ConstructorOptions options) const {
    auto tmp = *this;
    tmp.constructor_codegen = options;
    return tmp;
}

mt::stdlib::BlockInformation mt::stdlib::BlockInformation::with_symbolic_name(std::optional<std::string> name) const {
    auto tmp = *this;
    tmp.symbolic_name = name;
    return tmp;
}

mt::stdlib::BlockInformation mt::stdlib::BlockInformation::with_required_type_count(size_t count) const {
    auto tmp = *this;
    tmp.required_type_count = count;
    return tmp;
}

mt::stdlib::DataType mt::stdlib::BlockInformation::get_default_data_type() const {
    if (types.uses_float) {
        return DataType::F64;
    } else if (types.uses_integral) {
        return DataType::I32;
    } else if (types.uses_logical) {
        return DataType::BOOL;
    } else {
        return DataType::NONE;
    }
}

bool mt::stdlib::BlockInformation::type_supported(DataType dt) const {
    const auto mt = get_meta_type(dt);

    if (mt == nullptr) {
        return false;
    }

    if (mt->get_is_float() && types.uses_float) {
        return true;
    } else if (mt->get_is_integral() && types.uses_integral) {
        return true;
    } else if (dt == DataType::BOOL && types.uses_logical) {
        return true;
    } else {
        return false;
    }
}

template <typename T>
static mt::stdlib::block_interface::block_types create_block_types() {
    return mt::stdlib::block_interface::block_types{
        .uses_integral = T::uses_integral,
        .uses_float = T::uses_float,
        .uses_logical = T::uses_logical};
}

static const auto ARITH_BLOCK_NAMES = std::to_array<std::pair<std::string, std::string>>({
    {mt::stdlib::BLK_NAME_ARITH_ADD, "+"},
    {mt::stdlib::BLK_NAME_ARITH_SUB, "-"},
    {mt::stdlib::BLK_NAME_ARITH_MUL, "*"},
    {mt::stdlib::BLK_NAME_ARITH_DIV, "/"},
    {mt::stdlib::BLK_NAME_ARITH_MOD, "%"},
});

static const auto RELATIONAL_BLOCK_NAMES = std::to_array<std::pair<std::string, std::string>>({
    {mt::stdlib::BLK_NAME_REL_GT, ">"},
    {mt::stdlib::BLK_NAME_REL_GEQ, ">="},
    {mt::stdlib::BLK_NAME_REL_LT, "<"},
    {mt::stdlib::BLK_NAME_REL_LEQ, "<="},
    {mt::stdlib::BLK_NAME_REL_EQ, "=="},
    {mt::stdlib::BLK_NAME_REL_NEQ, "!="},
});

static const auto TRIG_BLOCK_NAMES = std::to_array({
    mt::stdlib::BLK_NAME_TRIG_SIN,
    mt::stdlib::BLK_NAME_TRIG_COS,
    mt::stdlib::BLK_NAME_TRIG_TAN,
    mt::stdlib::BLK_NAME_TRIG_ASIN,
    mt::stdlib::BLK_NAME_TRIG_ACOS,
    mt::stdlib::BLK_NAME_TRIG_ATAN,
    mt::stdlib::BLK_NAME_TRIG_ATAN2,
});

static bool is_block_type(std::string_view s, std::span<const std::string> list) {
    return std::find(list.begin(), list.end(), s) != list.end();
}

const static std::vector<mt::stdlib::BlockInformation> BLK_LIST = []() {
    using namespace mt::stdlib;

    // Standard Blocks
    std::vector<BlockInformation> blks = {
        BlockInformation(BLK_NAME_CLOCK, BlockInformation::ConstructorOptions::TIMESTEP, create_block_types<clock_block_types>()).with_uses_input_as_type(false),
        BlockInformation(BLK_NAME_CONST, BlockInformation::ConstructorOptions::VALUE, create_block_types<const_block_types>()).with_uses_input_as_type(false),
        BlockInformation(BLK_NAME_GAIN_PTR, BlockInformation::ConstructorOptions::VALUE_PTR, create_block_types<gain_ptr_block_types>()).with_uses_input_as_type(false),
        BlockInformation(BLK_NAME_DELAY, BlockInformation::ConstructorOptions::NONE, create_block_types<delay_block_types>()),
        BlockInformation(BLK_NAME_DERIV, BlockInformation::ConstructorOptions::TIMESTEP, create_block_types<derivative_block_types>()),
        BlockInformation(BLK_NAME_INTEG, BlockInformation::ConstructorOptions::TIMESTEP, create_block_types<integrator_block_types>()),
        BlockInformation(BLK_NAME_SWITCH, BlockInformation::ConstructorOptions::NONE, create_block_types<switch_block_types>()),
        BlockInformation(BLK_NAME_LIMITER, BlockInformation::ConstructorOptions::NONE, create_block_types<limiter_block_types>()),
        BlockInformation(BLK_NAME_CONVERSION, BlockInformation::ConstructorOptions::NONE, create_block_types<const_block_types>()).with_required_type_count(2),
    };

    // Arithmetic Blocks
    for (const auto& [a_name, a_symb_name] : ARITH_BLOCK_NAMES) {
        blks.emplace_back(
            BlockInformation(a_name, BlockInformation::ConstructorOptions::SIZE, create_block_types<arith_block_types>())
                .with_symbolic_name(a_symb_name)
                .with_constructor_codegen(BlockInformation::ConstructorOptions::NONE));
    }

    // Relational Blocks
    for (const auto& [r_name, r_symb_name] : RELATIONAL_BLOCK_NAMES) {
        block_interface::block_types dt_info;

        if (r_name == BLK_NAME_REL_EQ) {
            dt_info = create_block_types<relational_block_types<RelationalOperator::EQUAL>>();
        } else if (r_name == BLK_NAME_REL_NEQ) {
            dt_info = create_block_types<relational_block_types<RelationalOperator::NOT_EQUAL>>();
        } else if (r_name == BLK_NAME_REL_GT) {
            dt_info = create_block_types<relational_block_types<RelationalOperator::GREATER_THAN>>();
        } else if (r_name == BLK_NAME_REL_GEQ) {
            dt_info = create_block_types<relational_block_types<RelationalOperator::GREATER_THAN_EQUAL>>();
        } else if (r_name == BLK_NAME_REL_LT) {
            dt_info = create_block_types<relational_block_types<RelationalOperator::LESS_THAN>>();
        } else if (r_name == BLK_NAME_REL_LEQ) {
            dt_info = create_block_types<relational_block_types<RelationalOperator::LESS_THAN_EQUAL>>();
        } else {
            std::ostringstream oss;
            oss << "unsupported block types provided - \"" << r_name << '\"';
            throw block_error(oss.str());
        }

        blks.emplace_back(BlockInformation(r_name, BlockInformation::ConstructorOptions::NONE, dt_info).with_symbolic_name(r_symb_name));
    }

    // Trig Blocks
    for (const auto& t_name : TRIG_BLOCK_NAMES) {
        blks.emplace_back(t_name, BlockInformation::ConstructorOptions::NONE, create_block_types<trig_block_types>());
    }

    return blks;
}();

const static std::unordered_map<std::string, mt::stdlib::BlockInformation> BLK_INFOS = []() {
    std::unordered_map<std::string, mt::stdlib::BlockInformation> blk_map;

    for (const auto& b : BLK_LIST) {
        blk_map.emplace(b.name, b);

        if (b.symbolic_name.has_value()) {
            blk_map.emplace(*b.symbolic_name, b);
        }
    }

    return blk_map;
}();

const std::span<const mt::stdlib::BlockInformation> mt::stdlib::get_available_blocks() {
    return BLK_LIST;
}

template <template <mt::stdlib::DataType> class BLK, class TYPES, typename... Args>
static std::unique_ptr<mt::stdlib::block_interface> create_block_of_type(const mt::stdlib::DataType data_type, Args&&... args) {
    std::unique_ptr<mt::stdlib::block_interface> ptr = nullptr;

    if constexpr (TYPES::uses_integral) {
        switch (data_type) {
        case mt::stdlib::DataType::U8:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::U8>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::I8:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::I8>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::U16:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::U16>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::I16:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::I16>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::U32:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::U32>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::I32:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::I32>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::U64:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::U64>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::I64:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::I64>>(std::forward<Args>(args)...);
            break;
        default:
            break;
        }
    }

    if constexpr (TYPES::uses_float) {
        switch (data_type) {
        case mt::stdlib::DataType::F32:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::F32>>(std::forward<Args>(args)...);
            break;
        case mt::stdlib::DataType::F64:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::F64>>(std::forward<Args>(args)...);
            break;
        default:
            break;
        }
    }

    if constexpr (TYPES::uses_logical) {
        switch (data_type) {
        case mt::stdlib::DataType::BOOL:
            ptr = std::make_unique<BLK<mt::stdlib::DataType::BOOL>>(std::forward<Args>(args)...);
            break;
        default:
            break;
        }
    }

    if (ptr == nullptr) {
        throw mt::stdlib::block_error("unknown data type provided");
    } else {
        return ptr;
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

    std::unique_ptr<mt::stdlib::block_interface> operator()(const mt::stdlib::DataType data_type, const std::string& name) {
        mt::stdlib::block_interface* inter = nullptr;

        using namespace mt::stdlib;

        if (name == mt::stdlib::BLK_NAME_DELAY) {
            return create_block_of_type<delay_block, delay_block_types>(data_type);
        } else if (name == BLK_NAME_SWITCH) {
            return create_block_of_type<switch_block, switch_block_types>(data_type);
        } else if (name == BLK_NAME_LIMITER) {
            return create_block_of_type<limiter_block, limiter_block_types>(data_type);
        } else if (name == BLK_NAME_REL_EQ) {
            return create_block_of_type<blk_rel_eq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::EQUAL>>(data_type);
        } else if (name == BLK_NAME_REL_NEQ) {
            return create_block_of_type<blk_rel_neq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::NOT_EQUAL>>(data_type);
        } else if (name == BLK_NAME_REL_GT) {
            return create_block_of_type<blk_rel_gt, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::GREATER_THAN>>(data_type);
        } else if (name == BLK_NAME_REL_GEQ) {
            return create_block_of_type<blk_rel_geq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::GREATER_THAN_EQUAL>>(data_type);
        } else if (name == BLK_NAME_REL_LT) {
            return create_block_of_type<blk_rel_lt, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::LESS_THAN>>(data_type);
        } else if (name == BLK_NAME_REL_LEQ) {
            return create_block_of_type<blk_rel_leq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::LESS_THAN_EQUAL>>(data_type);
        } else if (name == BLK_NAME_TRIG_SIN) {
            return create_block_of_type<blk_trig_sin, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_NAME_TRIG_COS) {
            return create_block_of_type<blk_trig_cos, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_NAME_TRIG_TAN) {
            return create_block_of_type<blk_trig_tan, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_NAME_TRIG_ASIN) {
            return create_block_of_type<blk_trig_asin, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_NAME_TRIG_ACOS) {
            return create_block_of_type<blk_trig_acos, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_NAME_TRIG_ATAN) {
            return create_block_of_type<blk_trig_atan, mt::stdlib::trig_block_types>(data_type);
        } else if (name == BLK_NAME_TRIG_ATAN2) {
            return create_block_of_type<blk_trig_atan2, mt::stdlib::trig_block_types>(data_type);
        } else {
            std::ostringstream oss;
            oss << "Unknown block provided with \"" << name << "\"";
            throw block_error(oss.str());
        }
    }
};

struct SingleArgConstructor {
    std::unique_ptr<mt::stdlib::block_interface> operator()(const std::string& name, const mt::stdlib::Argument* val) {
        using namespace mt::stdlib;

        if (val == nullptr) {
            throw block_error("null argument provided");
        }

        if (name == BLK_NAME_DERIV) {
            return create_block_of_type<mt::stdlib::derivative_block, mt::stdlib::derivative_block_types>(val->get_type(), val);
        } else if (name == BLK_NAME_INTEG) {
            return create_block_of_type<mt::stdlib::integrator_block, mt::stdlib::integrator_block_types>(val->get_type(), val);
        } else if (name == BLK_NAME_CONST) {
            return create_block_of_type<mt::stdlib::const_block, mt::stdlib::const_block_types>(val->get_type(), val);
        } else if (name == BLK_NAME_CLOCK) {
            return create_block_of_type<mt::stdlib::clock_block, mt::stdlib::clock_block_types>(val->get_type(), val);
        } else {
            std::ostringstream oss;
            oss << "unknown block name \"" << name << "\" provided";
            throw mt::stdlib::block_error(oss.str());
        }
    }
};

template <mt::stdlib::DataType DT>
struct ArithmeticBlockFunctor {
    template <mt::stdlib::ArithType AT>
    class arith_wrapper final : public mt::stdlib::arith_block_dynamic<DT, AT> {
        using data_t = typename mt::stdlib::type_info<DT>::type_t;

    public:
        arith_wrapper(const size_t size) : data(std::unique_ptr<data_t[]>(new data_t[size])) {
            this->s_in.size = size;
            this->s_in.values = data.get();
        }

    private:
        std::unique_ptr<data_t[]> data;
    };

    template <mt::stdlib::ArithType AT>
    static std::unique_ptr<arith_wrapper<AT>> create_arith_block(const size_t size) {
        if constexpr (mt::stdlib::type_info<DT>::is_numeric) {
            return std::make_unique<arith_wrapper<AT>>(size);
        } else {
            throw mt::stdlib::block_error("data type provided is not arithmetic type");
        }
    }

    std::unique_ptr<mt::stdlib::block_interface> operator()(const std::string& name, const size_t size) {
        using namespace mt::stdlib;

        if (name == BLK_NAME_ARITH_ADD) {
            return create_arith_block<mt::stdlib::ArithType::ADD>(size);
        } else if (name == BLK_NAME_ARITH_SUB) {
            return create_arith_block<mt::stdlib::ArithType::SUB>(size);
        } else if (name == BLK_NAME_ARITH_MUL) {
            return create_arith_block<mt::stdlib::ArithType::MUL>(size);
        } else if (name == BLK_NAME_ARITH_DIV) {
            return create_arith_block<mt::stdlib::ArithType::DIV>(size);
        } else if (name == BLK_NAME_ARITH_MOD) {
            return create_arith_block<mt::stdlib::ArithType::MOD>(size);
        } else {
            std::ostringstream oss;
            oss << "unknown arith name \"" << name << "\" provided";
            throw block_error(oss.str());
        }
    }
};

template <template <mt::stdlib::DataType> class FCN, bool USE_INTEG, bool USE_FLOAT, bool USE_OTHER, typename... Args>
static std::unique_ptr<mt::stdlib::block_interface> create_block_with_type_inner(const mt::stdlib::DataType data_type, Args&&... args) {
    using namespace mt::stdlib;

    std::unique_ptr<block_interface> ptr = nullptr;

    if constexpr (USE_INTEG) {
        switch (data_type) {
            using enum DataType;
        case U8:
            ptr = FCN<U8>()(std::forward<Args>(args)...);
            break;
        case I8:
            ptr = FCN<I8>()(std::forward<Args>(args)...);
            break;
        case U16:
            ptr = FCN<U16>()(std::forward<Args>(args)...);
            break;
        case I16:
            ptr = FCN<I16>()(std::forward<Args>(args)...);
            break;
        case U32:
            ptr = FCN<U32>()(std::forward<Args>(args)...);
            break;
        case I32:
            ptr = FCN<I32>()(std::forward<Args>(args)...);
            break;
        case U64:
            ptr = FCN<U64>()(std::forward<Args>(args)...);
            break;
        case I64:
            ptr = FCN<I64>()(std::forward<Args>(args)...);
            break;
        default:
            break;
        }
    }

    if constexpr (USE_FLOAT) {
        switch (data_type) {
            using enum DataType;
        case F32:
            ptr = FCN<F32>()(std::forward<Args>(args)...);
            break;
        case F64:
            ptr = FCN<F64>()(std::forward<Args>(args)...);
            break;
        default:
            break;
        }
    }

    if constexpr (USE_OTHER) {
        switch (data_type) {
            using enum DataType;
        case BOOL:
            ptr = FCN<BOOL>()(std::forward<Args>(args)...);
            break;
        default:
            break;
        }
    }

    if (ptr == nullptr) {
        throw block_error("unknown data type provided");
    } else {
        return ptr;
    }
}

template <mt::stdlib::DataType DT1, mt::stdlib::DataType DT2>
struct DoubleTypeConstructor {
    std::unique_ptr<mt::stdlib::block_interface> operator()(std::string_view name) {
        if (name == mt::stdlib::BLK_NAME_CONVERSION) {
            return std::make_unique<mt::stdlib::conversion_block<DT1, DT2>>();
        } else {
            throw mt::stdlib::block_error((std::ostringstream{} << "unknown block name with multiple types with name '" << name << "' provided").str());
        }
    }
};

template <template <mt::stdlib::DataType, mt::stdlib::DataType> class FCN, mt::stdlib::DataType DT1, typename... Args>
static std::unique_ptr<mt::stdlib::block_interface> create_double_dt_block_inner(mt::stdlib::DataType dt2, Args&&... args) {
    switch (dt2) {
        using enum mt::stdlib::DataType;
    case F32:
        return FCN<DT1, F32>()(std::forward<Args>(args)...);
    case F64:
        return FCN<DT1, F64>()(std::forward<Args>(args)...);
    case U8:
        return FCN<DT1, U8>()(std::forward<Args>(args)...);
    case U16:
        return FCN<DT1, U16>()(std::forward<Args>(args)...);
    case U32:
        return FCN<DT1, U32>()(std::forward<Args>(args)...);
    case U64:
        return FCN<DT1, U64>()(std::forward<Args>(args)...);
    case I8:
        return FCN<DT1, I8>()(std::forward<Args>(args)...);
    case I16:
        return FCN<DT1, I16>()(std::forward<Args>(args)...);
    case I32:
        return FCN<DT1, I32>()(std::forward<Args>(args)...);
    case I64:
        return FCN<DT1, I64>()(std::forward<Args>(args)...);
    case BOOL:
        return FCN<DT1, BOOL>()(std::forward<Args>(args)...);
    default:
        throw mt::stdlib::block_error("unknown data type provided");
    }
}

template <template <mt::stdlib::DataType, mt::stdlib::DataType> class FCN, typename... Args>
static std::unique_ptr<mt::stdlib::block_interface> create_double_dt_block(mt::stdlib::DataType dt1, mt::stdlib::DataType dt2, Args&&... args) {
    switch (dt1) {
        using enum mt::stdlib::DataType;
    case F32:
        return create_double_dt_block_inner<FCN, F32>(dt2, std::forward<Args>(args)...);
    case F64:
        return create_double_dt_block_inner<FCN, F64>(dt2, std::forward<Args>(args)...);
    case U8:
        return create_double_dt_block_inner<FCN, U8>(dt2, std::forward<Args>(args)...);
    case U16:
        return create_double_dt_block_inner<FCN, U16>(dt2, std::forward<Args>(args)...);
    case U32:
        return create_double_dt_block_inner<FCN, U32>(dt2, std::forward<Args>(args)...);
    case U64:
        return create_double_dt_block_inner<FCN, U64>(dt2, std::forward<Args>(args)...);
    case I8:
        return create_double_dt_block_inner<FCN, I8>(dt2, std::forward<Args>(args)...);
    case I16:
        return create_double_dt_block_inner<FCN, I16>(dt2, std::forward<Args>(args)...);
    case I32:
        return create_double_dt_block_inner<FCN, I32>(dt2, std::forward<Args>(args)...);
    case I64:
        return create_double_dt_block_inner<FCN, I64>(dt2, std::forward<Args>(args)...);
    case BOOL:
        return create_double_dt_block_inner<FCN, BOOL>(dt2, std::forward<Args>(args)...);
    default:
        throw mt::stdlib::block_error("unknown data type provided");
    }
}

std::unique_ptr<mt::stdlib::block_interface> mt::stdlib::create_block(
    const std::string& name,
    std::span<const DataType> data_types,
    const Argument* argument) {
    using namespace mt::stdlib;

    const auto it = BLK_INFOS.find(name);
    if (it == BLK_INFOS.end()) {
        std::ostringstream oss;
        oss << "no block info for name \"" << name << "\" found";
        throw block_error(oss.str());
    }

    const auto& info = it->second;

    if (info.required_type_count != data_types.size()) {
        throw block_error("mismatch in required data type parameters");
    }

    if (info.required_type_count == 1) {
        const auto data_type = data_types[0];

        if (info.constructor_dynamic == BlockInformation::ConstructorOptions::VALUE || info.constructor_dynamic == BlockInformation::ConstructorOptions::TIMESTEP || info.constructor_dynamic == BlockInformation::ConstructorOptions::VALUE_PTR) {
            return SingleArgConstructor()(name, argument);
        } else if (info.constructor_dynamic == BlockInformation::ConstructorOptions::SIZE) {
            if (argument == nullptr) {
                throw block_error("must provide a valid size argument");
            }

            const size_t size = argument->as_size();

            return create_block_with_type_inner<ArithmeticBlockFunctor, true, true, false>(data_type, name, size);
        } else if (info.constructor_dynamic == BlockInformation::ConstructorOptions::NONE) {
            return StandardBlockFunctor()(data_type, name);
        } else {
            throw block_error("invalid constructor argument found");
        }
    } else if (info.required_type_count == 2) {
        if (info.name == BLK_NAME_CONVERSION) {
            return create_double_dt_block<DoubleTypeConstructor>(data_types[0], data_types[1], info.name);
        } else {
            throw block_error((std::ostringstream{} << "unknown block name with multiple types with name '" << info.name << "' provided").str());
        }
    } else {
        throw block_error("unsupported number of data types provided");
    }
}

std::unique_ptr<mt::stdlib::block_interface> mt::stdlib::create_block(
    const BlockInformation& info,
    std::span<const DataType> data_types,
    const Argument* argument) {
    return create_block(info.name, data_types, argument);
}

#endif // MT_STDLIB_USE_FULL_LIB
