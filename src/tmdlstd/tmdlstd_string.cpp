// SPDX-License-Identifier: MIT

#include "tmdlstd_string.hpp"
#include "tmdlstd/tmdlstd.hpp"

#include <fmt/format.h>

const static std::string BASE_NAMESPACE = "tmdl::stdlib";

static std::string spec_to_name(tmdl::stdlib::SpecificationType spec,
                                const std::string& type,
                                const std::string& name) {
    switch (spec) {
        using enum tmdl::stdlib::SpecificationType;
    case FULL:
        return fmt::format("{}::{}::{}", BASE_NAMESPACE, type, name);
    case TYPE:
        return fmt::format("{}::{}", type, name);
    case NONE:
        return name;
    default:
        throw tmdl::stdlib::block_error("unknown specification type provided");
    }
}

std::string tmdl::stdlib::arith_to_string(const ArithType t,
                                          SpecificationType specification) {
    static const std::string TYPE_NAME = "ArithType";
    std::string name;

    switch (t) {
        using enum ArithType;
    case ADD:
        name = "ADD";
        break;
    case SUB:
        name = "SUB";
        break;
    case MUL:
        name = "MUL";
        break;
    case DIV:
        name = "DIV";
        break;
    case MOD:
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
        using enum RelationalOperator;
    case EQUAL:
        name = "EQUAL";
        break;
    case NOT_EQUAL:
        name = "NOT_EQUAL";
        break;
    case GREATER_THAN:
        name = "GREATER_THAN";
        break;
    case GREATER_THAN_EQUAL:
        name = "GREATER_THAN_EQUAL";
        break;
    case LESS_THAN:
        name = "LESS_THAN";
        break;
    case LESS_THAN_EQUAL:
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
        using enum TrigFunction;
    case SIN:
        name = "SIN";
        break;
    case COS:
        name = "COS";
        break;
    case TAN:
        name = "TAN";
        break;
    case ASIN:
        name = "ASIN";
        break;
    case ACOS:
        name = "ACOS";
        break;
    case ATAN:
        name = "ATAN";
        break;
    case ATAN2:
        name = "ATAN2";
        break;
    default:
        throw block_error(
            "unsupported trig function provided for string conversion");
    }

    return spec_to_name(specification, TYPE_NAME, name);
}
