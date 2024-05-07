// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstdlib_string.hpp"
#include "mtstdlib_except.hpp"

#include <sstream>

constinit std::string mt::stdlib::BASE_NAMESPACE = "mt::stdlib";

constinit std::string mt::stdlib::BLK_NAME_CLOCK = "clock";
constinit std::string mt::stdlib::BLK_NAME_CONST = "constant";
constinit std::string mt::stdlib::BLK_NAME_DELAY = "delay";
constinit std::string mt::stdlib::BLK_NAME_DERIV = "derivative";
constinit std::string mt::stdlib::BLK_NAME_INTEG = "integrator";
constinit std::string mt::stdlib::BLK_NAME_SWITCH = "switch";
constinit std::string mt::stdlib::BLK_NAME_LIMITER = "limiter";
constinit std::string mt::stdlib::BLK_NAME_ARITH_ADD = "add";
constinit std::string mt::stdlib::BLK_NAME_ARITH_SUB = "sub";
constinit std::string mt::stdlib::BLK_NAME_ARITH_MUL = "mul";
constinit std::string mt::stdlib::BLK_NAME_ARITH_DIV = "div";
constinit std::string mt::stdlib::BLK_NAME_ARITH_MOD = "mod";
constinit std::string mt::stdlib::BLK_NAME_REL_GT = "greater";
constinit std::string mt::stdlib::BLK_NAME_REL_GEQ = "greater_eq";
constinit std::string mt::stdlib::BLK_NAME_REL_LT = "less";
constinit std::string mt::stdlib::BLK_NAME_REL_LEQ = "less_eq";
constinit std::string mt::stdlib::BLK_NAME_REL_EQ = "equal";
constinit std::string mt::stdlib::BLK_NAME_REL_NEQ = "not_equal";
constinit std::string mt::stdlib::BLK_NAME_TRIG_SIN = "sin";
constinit std::string mt::stdlib::BLK_NAME_TRIG_COS = "cos";
constinit std::string mt::stdlib::BLK_NAME_TRIG_TAN = "tan";
constinit std::string mt::stdlib::BLK_NAME_TRIG_ASIN = "asin";
constinit std::string mt::stdlib::BLK_NAME_TRIG_ACOS = "acos";
constinit std::string mt::stdlib::BLK_NAME_TRIG_ATAN = "atan";
constinit std::string mt::stdlib::BLK_NAME_TRIG_ATAN2 = "atan2";

static std::string to_enum_name(mt::stdlib::SpecificationType spec,
                                const std::string& type,
                                const std::string& name) {
    std::ostringstream oss;

    switch (spec) {
    case mt::stdlib::SpecificationType::FULL:
        oss << mt::stdlib::BASE_NAMESPACE << "::";
    case mt::stdlib::SpecificationType::TYPE:
        oss << type << "::";
    case mt::stdlib::SpecificationType::NONE:
        oss << name;
        break;
    default:
        throw mt::stdlib::block_error("unknown specification type provided");
    }

    return oss.str();
}

std::string mt::stdlib::datatype_to_string(
    DataType dt,
    SpecificationType specification) {
    static const std::string TYPE_NAME = "DataType";
    std::string name;

    switch (dt) {
        using enum mt::stdlib::DataType;
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
    case I8:
        name = "I8";
        break;
    case I16:
        name = "I16";
        break;
    case I32:
        name = "I32";
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

std::string mt::stdlib::arith_to_string(const ArithType t,
                                        SpecificationType specification) {
    static const std::string TYPE_NAME = "ArithType";
    std::string name;

    switch (t) {
    case mt::stdlib::ArithType::ADD:
        name = "ADD";
        break;
    case mt::stdlib::ArithType::SUB:
        name = "SUB";
        break;
    case mt::stdlib::ArithType::MUL:
        name = "MUL";
        break;
    case mt::stdlib::ArithType::DIV:
        name = "DIV";
        break;
    case mt::stdlib::ArithType::MOD:
        name = "MOD";
        break;
    default:
        throw block_error(
            "unsupported arithmetic function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string
mt::stdlib::relational_to_string(const RelationalOperator op,
                                 SpecificationType specification) {
    static const std::string TYPE_NAME = "RelationalOperator";
    std::string name;

    switch (op) {
    case mt::stdlib::RelationalOperator::EQUAL:
        name = "EQUAL";
        break;
    case mt::stdlib::RelationalOperator::NOT_EQUAL:
        name = "NOT_EQUAL";
        break;
    case mt::stdlib::RelationalOperator::GREATER_THAN:
        name = "GREATER_THAN";
        break;
    case mt::stdlib::RelationalOperator::GREATER_THAN_EQUAL:
        name = "GREATER_THAN_EQUAL";
        break;
    case mt::stdlib::RelationalOperator::LESS_THAN:
        name = "LESS_THAN";
        break;
    case mt::stdlib::RelationalOperator::LESS_THAN_EQUAL:
        name = "LESS_THAN_EQUAL";
        break;
    default:
        throw block_error(
            "unsupported relational function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string mt::stdlib::trig_func_to_string(const TrigFunction fcn,
                                            SpecificationType specification) {
    const static std::string TYPE_NAME = "TrigFunction";
    std::string name;

    switch (fcn) {
    case mt::stdlib::TrigFunction::SIN:
        name = "SIN";
        break;
    case mt::stdlib::TrigFunction::COS:
        name = "COS";
        break;
    case mt::stdlib::TrigFunction::TAN:
        name = "TAN";
        break;
    case mt::stdlib::TrigFunction::ASIN:
        name = "ASIN";
        break;
    case mt::stdlib::TrigFunction::ACOS:
        name = "ACOS";
        break;
    case mt::stdlib::TrigFunction::ATAN:
        name = "ATAN";
        break;
    case mt::stdlib::TrigFunction::ATAN2:
        name = "ATAN2";
        break;
    default:
        throw block_error(
            "unsupported trig function provided for string conversion");
    }

    return to_enum_name(specification, TYPE_NAME, name);
}

std::string mt::stdlib::with_namespace(const std::string& name) {
    std::ostringstream oss;
    oss << BASE_NAMESPACE << ":: " << name;
    return oss.str();
}

#endif // MT_STDLIB_USE_FULL_LIB
