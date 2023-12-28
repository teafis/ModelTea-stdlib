// SPDX-License-Identifier: MIT

#include "tmdlstd_string.hpp"
#include "tmdlstd.hpp"
#include "tmdlstd_except.hpp"

#include <sstream>

const static std::string BASE_NAMESPACE = "tmdl::stdlib";

static std::string spec_to_name(tmdl::stdlib::SpecificationType spec,
                                const std::string& type,
                                const std::string& name) {
    std::ostringstream oss;

    switch (spec) {
    case tmdl::stdlib::SpecificationType::FULL:
        oss << BASE_NAMESPACE << "::";
    case tmdl::stdlib::SpecificationType::TYPE:
        oss << type << "::";
    case tmdl::stdlib::SpecificationType::NONE:
        oss << name;
        break;
    default:
        throw tmdl::stdlib::block_error("unknown specification type provided");
    }

    return oss.str();
}

std::string tmdl::stdlib::arith_to_string(const ArithType t,
                                          SpecificationType specification) {
    static const std::string TYPE_NAME = "ArithType";
    std::string name;

    switch (t) {
    case tmdl::stdlib::ArithType::ADD:
        name = "ADD";
        break;
    case tmdl::stdlib::ArithType::SUB:
        name = "SUB";
        break;
    case tmdl::stdlib::ArithType::MUL:
        name = "MUL";
        break;
    case tmdl::stdlib::ArithType::DIV:
        name = "DIV";
        break;
    case tmdl::stdlib::ArithType::MOD:
        name = "MOD";
        break;
    default:
        throw block_error(
            "unsupported arithmetic function provided for string conversion");
    }

    return spec_to_name(specification, TYPE_NAME, name);
}

std::string
tmdl::stdlib::relational_to_string(const RelationalOperator op,
                                   SpecificationType specification) {
    static const std::string TYPE_NAME = "RelationalOperator";
    std::string name;

    switch (op) {
    case tmdl::stdlib::RelationalOperator::EQUAL:
        name = "EQUAL";
        break;
    case tmdl::stdlib::RelationalOperator::NOT_EQUAL:
        name = "NOT_EQUAL";
        break;
    case tmdl::stdlib::RelationalOperator::GREATER_THAN:
        name = "GREATER_THAN";
        break;
    case tmdl::stdlib::RelationalOperator::GREATER_THAN_EQUAL:
        name = "GREATER_THAN_EQUAL";
        break;
    case tmdl::stdlib::RelationalOperator::LESS_THAN:
        name = "LESS_THAN";
        break;
    case tmdl::stdlib::RelationalOperator::LESS_THAN_EQUAL:
        name = "LESS_THAN_EQUAL";
        break;
    default:
        throw block_error(
            "unsupported relational function provided for string conversion");
    }

    return spec_to_name(specification, TYPE_NAME, name);
}

std::string tmdl::stdlib::trig_func_to_string(const TrigFunction fcn,
                                              SpecificationType specification) {
    const static std::string TYPE_NAME = "TrigFunction";
    std::string name;

    switch (fcn) {
    case tmdl::stdlib::TrigFunction::SIN:
        name = "SIN";
        break;
    case tmdl::stdlib::TrigFunction::COS:
        name = "COS";
        break;
    case tmdl::stdlib::TrigFunction::TAN:
        name = "TAN";
        break;
    case tmdl::stdlib::TrigFunction::ASIN:
        name = "ASIN";
        break;
    case tmdl::stdlib::TrigFunction::ACOS:
        name = "ACOS";
        break;
    case tmdl::stdlib::TrigFunction::ATAN:
        name = "ATAN";
        break;
    case tmdl::stdlib::TrigFunction::ATAN2:
        name = "ATAN2";
        break;
    default:
        throw block_error(
            "unsupported trig function provided for string conversion");
    }

    return spec_to_name(specification, TYPE_NAME, name);
}
