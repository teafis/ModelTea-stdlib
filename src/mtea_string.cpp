// SPDX-License-Identifier: MIT

#ifdef MTEA_USE_FULL_LIB

#include "mtea_string.hpp"
#include "mtea_except.hpp"

#include <sstream>

constinit std::string mtea::BASE_NAMESPACE = "mtea";

constinit std::string mtea::BLK_NAME_CLOCK = "clock";
constinit std::string mtea::BLK_NAME_CONST = "constant";
constinit std::string mtea::BLK_NAME_CONST_PTR = "constant_ptr";
constinit std::string mtea::BLK_NAME_CONVERSION = "conversion";
constinit std::string mtea::BLK_NAME_DELAY = "delay";
constinit std::string mtea::BLK_NAME_DERIV = "derivative";
constinit std::string mtea::BLK_NAME_INTEG = "integrator";
constinit std::string mtea::BLK_NAME_SWITCH = "switch";
constinit std::string mtea::BLK_NAME_LIMITER = "limiter";
constinit std::string mtea::BLK_NAME_ARITH_ADD = "add";
constinit std::string mtea::BLK_NAME_ARITH_SUB = "sub";
constinit std::string mtea::BLK_NAME_ARITH_MUL = "mul";
constinit std::string mtea::BLK_NAME_ARITH_DIV = "div";
constinit std::string mtea::BLK_NAME_ARITH_MOD = "mod";
constinit std::string mtea::BLK_NAME_REL_GT = "greater";
constinit std::string mtea::BLK_NAME_REL_GEQ = "greater_eq";
constinit std::string mtea::BLK_NAME_REL_LT = "less";
constinit std::string mtea::BLK_NAME_REL_LEQ = "less_eq";
constinit std::string mtea::BLK_NAME_REL_EQ = "equal";
constinit std::string mtea::BLK_NAME_REL_NEQ = "not_equal";
constinit std::string mtea::BLK_NAME_TRIG_SIN = "sin";
constinit std::string mtea::BLK_NAME_TRIG_COS = "cos";
constinit std::string mtea::BLK_NAME_TRIG_TAN = "tan";
constinit std::string mtea::BLK_NAME_TRIG_ASIN = "asin";
constinit std::string mtea::BLK_NAME_TRIG_ACOS = "acos";
constinit std::string mtea::BLK_NAME_TRIG_ATAN = "atan";
constinit std::string mtea::BLK_NAME_TRIG_ATAN2 = "atan2";

static std::string to_enum_name(mtea::SpecificationType spec,
                                const std::string& type,
                                const std::string& name) {
    std::ostringstream oss;

    switch (spec) {
    case mtea::SpecificationType::FULL:
        oss << mtea::BASE_NAMESPACE << "::";
    case mtea::SpecificationType::TYPE:
        oss << type << "::";
    case mtea::SpecificationType::NONE:
        oss << name;
        break;
    default:
        throw mtea::block_error("unknown specification type provided");
    }

    return oss.str();
}

std::string mtea::datatype_to_string(
    DataType dt,
    SpecificationType specification) {
    static const std::string TYPE_NAME = "DataType";
    std::string name;

    switch (dt) {
        using enum mtea::DataType;
    case F64:
        name = "F64";
        break;
    case F32:
        name = "F32";
        break;
    case U8:
        name = "U8";
        break;
    case U16:
        name = "U16";
        break;
    case U32:
        name = "U32";
        break;
    case U64:
        name = "U64";
        break;
    case I8:
        name = "I8";
        break;
    case I16:
        name = "I16";
        break;
    case I32:
        name = "I32";
        break;
    case I64:
        name = "I64";
        break;
    case BOOL:
        name = "BOOL";
        break;
    default:
        throw block_error(
            "unsupported data type function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string mtea::arith_to_string(const ArithType t,
                                        SpecificationType specification) {
    static const std::string TYPE_NAME = "ArithType";
    std::string name;

    switch (t) {
    case mtea::ArithType::ADD:
        name = "ADD";
        break;
    case mtea::ArithType::SUB:
        name = "SUB";
        break;
    case mtea::ArithType::MUL:
        name = "MUL";
        break;
    case mtea::ArithType::DIV:
        name = "DIV";
        break;
    case mtea::ArithType::MOD:
        name = "MOD";
        break;
    default:
        throw block_error(
            "unsupported arithmetic function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string
mtea::relational_to_string(const RelationalOperator op,
                                 SpecificationType specification) {
    static const std::string TYPE_NAME = "RelationalOperator";
    std::string name;

    switch (op) {
    case mtea::RelationalOperator::EQUAL:
        name = "EQUAL";
        break;
    case mtea::RelationalOperator::NOT_EQUAL:
        name = "NOT_EQUAL";
        break;
    case mtea::RelationalOperator::GREATER_THAN:
        name = "GREATER_THAN";
        break;
    case mtea::RelationalOperator::GREATER_THAN_EQUAL:
        name = "GREATER_THAN_EQUAL";
        break;
    case mtea::RelationalOperator::LESS_THAN:
        name = "LESS_THAN";
        break;
    case mtea::RelationalOperator::LESS_THAN_EQUAL:
        name = "LESS_THAN_EQUAL";
        break;
    default:
        throw block_error(
            "unsupported relational function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string mtea::trig_func_to_string(const TrigFunction fcn,
                                            SpecificationType specification) {
    const static std::string TYPE_NAME = "TrigFunction";
    std::string name;

    switch (fcn) {
    case mtea::TrigFunction::SIN:
        name = "SIN";
        break;
    case mtea::TrigFunction::COS:
        name = "COS";
        break;
    case mtea::TrigFunction::TAN:
        name = "TAN";
        break;
    case mtea::TrigFunction::ASIN:
        name = "ASIN";
        break;
    case mtea::TrigFunction::ACOS:
        name = "ACOS";
        break;
    case mtea::TrigFunction::ATAN:
        name = "ATAN";
        break;
    case mtea::TrigFunction::ATAN2:
        name = "ATAN2";
        break;
    default:
        throw block_error(
            "unsupported trig function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string mtea::with_namespace(const std::string& name) {
    std::ostringstream oss;
    oss << BASE_NAMESPACE << ":: " << name;
    return oss.str();
}

#endif // MTEA_USE_FULL_LIB
