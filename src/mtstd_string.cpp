// SPDX-License-Identifier: MIT

#include "mtstd_string.hpp"
#include "mtstd.hpp"
#include "mtstd_except.hpp"

#include <sstream>

const std::string mt::stdlib::BASE_NAMESPACE = "mt::stdlib";

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