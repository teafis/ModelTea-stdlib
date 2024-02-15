// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstd_creation.hpp"

#include "mtstd_except.hpp"
#include "mtstd_string.hpp"

#include "mtstd.hpp"

#include <sstream>
#include <unordered_map>

const static std::vector<mt::stdlib::BlockInformation> BLK_LIST = []() {
    using namespace mt::stdlib;

    std::vector<BlockInformation> blks = {
        BlockInformation(BLK_NAME_CLOCK, "", BlockInformation::ConstructorOptions::VALUE, BlockInformation::TypeOptions::NUMERIC),
        BlockInformation(BLK_NAME_CONST, "", BlockInformation::ConstructorOptions::VALUE, BlockInformation::TypeOptions::ALL),
        BlockInformation(BLK_NAME_DELAY, "", BlockInformation::ConstructorOptions::DEFAULT, BlockInformation::TypeOptions::ALL),
        BlockInformation(BLK_NAME_DERIV, "", BlockInformation::ConstructorOptions::VALUE, BlockInformation::TypeOptions::FLOAT),
        BlockInformation(BLK_NAME_INTEG, "", BlockInformation::ConstructorOptions::VALUE, BlockInformation::TypeOptions::FLOAT),
        BlockInformation(BLK_NAME_SWITCH, "", BlockInformation::ConstructorOptions::DEFAULT, BlockInformation::TypeOptions::ALL),
        BlockInformation(BLK_NAME_LIMITER, "", BlockInformation::ConstructorOptions::DEFAULT, BlockInformation::TypeOptions::NUMERIC),

    };

    for (const auto& a_name : { BLK_SUB_NAME_ARITH_ADD, BLK_SUB_NAME_ARITH_SUB, BLK_SUB_NAME_ARITH_MUL, BLK_SUB_NAME_ARITH_DIV, BLK_SUB_NAME_ARITH_MOD })
    {
        blks.emplace_back(BLK_NAME_ARITH, a_name, BlockInformation::ConstructorOptions::SIZE, BlockInformation::TypeOptions::NUMERIC);
    }

    for (const auto& r_name : { BLK_SUB_NAME_REL_EQ, BLK_SUB_NAME_REL_NEQ })
    {
        blks.emplace_back(BLK_NAME_REL, r_name, BlockInformation::ConstructorOptions::DEFAULT, BlockInformation::TypeOptions::ALL);
    }

    for (const auto& r_name : { BLK_SUB_NAME_REL_GT, BLK_SUB_NAME_REL_GEQ, BLK_SUB_NAME_REL_LT, BLK_SUB_NAME_REL_LEQ })
    {
        blks.emplace_back(BLK_NAME_REL, r_name, BlockInformation::ConstructorOptions::DEFAULT, BlockInformation::TypeOptions::NUMERIC);
    }

    for (const auto& t_name : { BLK_SUB_NAME_TRIG_SIN, BLK_SUB_NAME_TRIG_COS, BLK_SUB_NAME_TRIG_TAN, BLK_SUB_NAME_TRIG_ASIN, BLK_SUB_NAME_TRIG_ACOS, BLK_SUB_NAME_TRIG_ATAN, BLK_SUB_NAME_TRIG_ATAN2 })
    {
        blks.emplace_back(BLK_NAME_TRIG, t_name, BlockInformation::ConstructorOptions::DEFAULT, BlockInformation::TypeOptions::FLOAT);
    }

    return blks;
}();

const static std::unordered_map<std::string, std::unordered_map<std::string, mt::stdlib::BlockInformation>> BLK_INFOS = []() {
    std::unordered_map<std::string, std::unordered_map<std::string, mt::stdlib::BlockInformation>> blk_map;

    for (const auto& b : BLK_LIST)
    {
        blk_map[b.base_name].emplace(b.sub_name, b);
    }

    return blk_map;
}();

const std::span<const mt::stdlib::BlockInformation> mt::stdlib::get_available_blocks()
{
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
    }
    else {
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

    std::unique_ptr<mt::stdlib::block_interface> operator()(const mt::stdlib::DataType data_type, const std::string& name, const std::string& sub_name) {
        mt::stdlib::block_interface* inter = nullptr;

        using namespace mt::stdlib;

        if (name == mt::stdlib::BLK_NAME_DELAY) {
            return create_block_of_type<delay_block, delay_block_types>(data_type);
        }
        else if (name == BLK_NAME_SWITCH) {
            return create_block_of_type<switch_block, switch_block_types>(data_type);
        }
        else if (name == BLK_NAME_LIMITER) {
            return create_block_of_type<limiter_block, limiter_block_types>(data_type);
        }
        else if (name == BLK_NAME_REL && sub_name == BLK_SUB_NAME_REL_EQ) {
            return create_block_of_type<blk_rel_eq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::EQUAL>>(data_type);
        }
        else if (name == BLK_NAME_REL && sub_name == BLK_SUB_NAME_REL_NEQ) {
            return create_block_of_type<blk_rel_neq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::NOT_EQUAL>>(data_type);
        }
        else if (name == BLK_NAME_REL && sub_name == BLK_SUB_NAME_REL_GT) {
            return create_block_of_type<blk_rel_gt, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::GREATER_THAN>>(data_type);
        }
        else if (name == BLK_NAME_REL && sub_name == BLK_SUB_NAME_REL_GEQ) {
            return create_block_of_type<blk_rel_geq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::GREATER_THAN_EQUAL>>(data_type);
        }
        else if (name == BLK_NAME_REL && sub_name == BLK_SUB_NAME_REL_LT) {
            return create_block_of_type<blk_rel_lt, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::LESS_THAN>>(data_type);
        }
        else if (name == BLK_NAME_REL && sub_name == BLK_SUB_NAME_REL_LEQ) {
            return create_block_of_type<blk_rel_leq, mt::stdlib::relational_block_types<mt::stdlib::RelationalOperator::LESS_THAN_EQUAL>>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_SIN) {
            return create_block_of_type<blk_trig_sin, mt::stdlib::trig_block_types>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_COS) {
            return create_block_of_type<blk_trig_cos, mt::stdlib::trig_block_types>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_TAN) {
            return create_block_of_type<blk_trig_tan, mt::stdlib::trig_block_types>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_ASIN) {
            return create_block_of_type<blk_trig_asin, mt::stdlib::trig_block_types>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_ACOS) {
            return create_block_of_type<blk_trig_acos, mt::stdlib::trig_block_types>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_ATAN) {
            return create_block_of_type<blk_trig_atan, mt::stdlib::trig_block_types>(data_type);
        }
        else if (name == BLK_NAME_TRIG && sub_name == BLK_SUB_NAME_TRIG_ATAN2) {
            return create_block_of_type<blk_trig_atan2, mt::stdlib::trig_block_types>(data_type);
        }
        else {
            std::ostringstream oss;
            oss << "Unknown block provided with \"" << name << "\"";
            if (!sub_name.empty())
            {
                oss << " - \"" << sub_name << "\"";
            }

            throw block_error(oss.str());
        }
    }
};

struct SingleArgConstructor {
    std::unique_ptr<mt::stdlib::block_interface> operator()(const std::string& name, [[maybe_unused]] const std::string& sub_name, const mt::stdlib::ArgumentValue* val) {
        using namespace mt::stdlib;

        if (val == nullptr) {
            throw block_error("null argument provided");
        }

        if (name == BLK_NAME_DERIV) {
            return create_block_of_type<mt::stdlib::derivative_block, mt::stdlib::derivative_block_types>(val->get_type(), val);
        }
        else if (name == BLK_NAME_INTEG) {
            return create_block_of_type<mt::stdlib::integrator_block, mt::stdlib::integrator_block_types>(val->get_type(), val);
        }
        else if (name == BLK_NAME_CONST) {
            return create_block_of_type<mt::stdlib::const_block, mt::stdlib::const_block_types>(val->get_type(), val);
        }
        else if (name == BLK_NAME_CLOCK) {
            return create_block_of_type<mt::stdlib::clock_block, mt::stdlib::clock_block_types>(val->get_type(), val);
        }
        else {
            std::ostringstream oss;
            oss << "unknown block name \"" << name << "\" provided";
            throw mt::stdlib::block_error(oss.str());
        }
    }
};

template <mt::stdlib::DataType DT>
struct ArithmeticBlockFunctor {
    template <mt::stdlib::ArithType AT>
    class arith_wrapper final : public mt::stdlib::arith_block_dynamic<DT, AT>
    {
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
        }
        else {
            throw mt::stdlib::block_error("data type provided is not arithmetic type");
        }
    }

    std::unique_ptr<mt::stdlib::block_interface> operator()(const std::string& name, const std::string& sub_name, const size_t size) {
        using namespace mt::stdlib;

        if (name == BLK_NAME_ARITH)
        {
            if (sub_name == BLK_SUB_NAME_ARITH_ADD) {
                return create_arith_block<mt::stdlib::ArithType::ADD>(size);
            }
            else if (sub_name == BLK_SUB_NAME_ARITH_SUB) {
                return create_arith_block<mt::stdlib::ArithType::SUB>(size);
            }
            else if (sub_name == BLK_SUB_NAME_ARITH_MUL) {
                return create_arith_block<mt::stdlib::ArithType::MUL>(size);
            }
            else if (sub_name == BLK_SUB_NAME_ARITH_DIV) {
                return create_arith_block<mt::stdlib::ArithType::DIV>(size);
            }
            else if (sub_name == BLK_SUB_NAME_ARITH_MOD) {
                return create_arith_block<mt::stdlib::ArithType::MOD>(size);
            }
            else {
                std::ostringstream oss;
                oss << "unknown arith sub-name \"" << sub_name << "\" provided";
                throw block_error(oss.str());
            }
        }
        else
        {
            std::ostringstream oss;
            oss << "unknown block name \"" << name << "\" provided";
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
    }
    else {
        return ptr;
    }

}

std::unique_ptr<mt::stdlib::block_interface> mt::stdlib::create_block(const std::string& name, const std::string& sub_name, DataType data_type, const ArgumentValue* argument)
{
    using namespace mt::stdlib;

    const auto it = BLK_INFOS.find(name);
    if (it == BLK_INFOS.end())
    {
        std::ostringstream oss;
        oss << "no block info for name \"" << name << "\" found";
        throw block_error(oss.str());
    }

    const auto& blk_cat = it->second;

    const auto itt = blk_cat.find(sub_name);
    if (itt == blk_cat.end())
    {
        std::ostringstream oss;
        oss << "no block info for name \"" << name << "::" << sub_name << "\" found";
        throw block_error(oss.str());
    }

    const auto& info = itt->second;

    if (info.constructor == BlockInformation::ConstructorOptions::VALUE)
    {
        return SingleArgConstructor()(name, sub_name, argument);
    }
    else if (info.constructor == BlockInformation::ConstructorOptions::SIZE)
    {
        if (argument == nullptr)
        {
            throw block_error("must provide a valid size argument");
        }

        const size_t size = argument->as_size();

        return create_block_with_type_inner<ArithmeticBlockFunctor, true, true, false>(data_type, name, sub_name, size);
    }
    else if (info.constructor == BlockInformation::ConstructorOptions::DEFAULT)
    {
        return StandardBlockFunctor()(data_type, name, sub_name);
    }
    else
    {
        throw block_error("invalid constructor argument found");
    }
}

#endif // MT_STDLIB_USE_FULL_LIB
