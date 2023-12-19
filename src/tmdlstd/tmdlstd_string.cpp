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
    case SUB:
        name = "SUB";
    case MUL:
        name = "MUL";
    case DIV:
        name = "DIV";
    case MOD:
        name = "MOD";
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
    case NOT_EQUAL:
        name = "NOT_EQUAL";
    case GREATER_THAN:
        name = "GREATER_THAN";
    case GREATER_THAN_EQUAL:
        name = "GREATER_THAN_EQUAL";
    case LESS_THAN:
        name = "LESS_THAN";
    case LESS_THAN_EQUAL:
        name = "LESS_THAN_EQUAL";
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
    case COS:
        name = "COS";
    case TAN:
        name = "TAN";
    case ASIN:
        name = "ASIN";
    case ACOS:
        name = "ACOS";
    case ATAN:
        name = "ATAN";
    default:
        throw block_error(
            "unsupported trig function provided for string conversion");
    }

    return spec_to_name(specification, TYPE_NAME, name);
}

std::string tmdl::stdlib::trig_func_to_string(const TrigFunction2 fcn,
                                              SpecificationType specification) {
    const static std::string TYPE_NAME = "TrigFunction2";
    std::string name;

    switch (fcn) {
        using enum TrigFunction2;
    case ATAN2:
        name = "ATAN2";
    default:
        throw block_error(
            "unsupported trig function provided for string conversion");
    }

    return spec_to_name(specification, TYPE_NAME, name);
}
