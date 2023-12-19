// SPDX-License-Identifier: MIT

#include "tmdlstd_string.hpp"
#include "tmdlstd/tmdlstd.hpp"

const char* tmdl::stdlib::arith_to_string(const ArithType t) {
    switch (t) {
        using enum ArithType;
    case ADD:
        return "tmdl::stdlib::ArithType::ADD";
    case SUB:
        return "tmdl::stdlib::ArithType::SUB";
    case MUL:
        return "tmdl::stdlib::ArithType::MUL";
    case DIV:
        return "tmdl::stdlib::ArithType::DIV";
    case MOD:
        return "tmdl::stdlib::ArithType::MOD";
    default:
        throw block_error("unsupported arithmetic function provided for string conversion");
    }
}

const char* tmdl::stdlib::relational_to_string(const RelationalOperator op) {
    switch (op) {
        using enum RelationalOperator;
    case EQUAL:
        return "tmdl::stdlib::RelationalOperator::EQUAL";
    case NOT_EQUAL:
        return "tmdl::stdlib::RelationalOperator::NOT_EQUAL";
    case GREATER_THAN:
        return "tmdl::stdlib::RelationalOperator::GREATER_THAN";
    case GREATER_THAN_EQUAL:
        return "tmdl::stdlib::RelationalOperator::GREATER_THAN_EQUAL";
    case LESS_THAN:
        return "tmdl::stdlib::RelationalOperator::LESS_THAN";
    case LESS_THAN_EQUAL:
        return "tmdl::stdlib::RelationalOperator::LESS_THAN_EQUAL";
    default:
        throw block_error("unsupported relational function provided for string conversion");
    }
}

const char* tmdl::stdlib::trig_func_to_string(const TrigFunction fcn) {
    switch (fcn) {
        using enum TrigFunction;
    case SIN:
        return "tmdl::stdlib::TrigFunction::SIN";
    case COS:
        return "tmdl::stdlib::TrigFunction::COS";
    case TAN:
        return "tmdl::stdlib::TrigFunction::TAN";
    case ASIN:
        return "tmdl::stdlib::TrigFunction::ASIN";
    case ACOS:
        return "tmdl::stdlib::TrigFunction::ACOS";
    case ATAN:
        return "tmdl::stdlib::TrigFunction::ATAN";
    default:
        throw block_error("unsupported trig function provided for string conversion");
    }
}

const char* tmdl::stdlib::trig_func_to_string(const TrigFunction2 fcn) {
    switch (fcn) {
        using enum TrigFunction2;
    case ATAN2:
        return "tmdl::stdlib::TrigFunction2::ATAN2";
    default:
        throw block_error("unsupported trig function provided for string conversion");
    }
}
