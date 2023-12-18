// SPDX-License-Identifier: MIT

#include "tmdlstd.hpp"

tmdl::stdlib::clock_block::clock_block(const double dt) : time_step{dt} {
    init();
}

void tmdl::stdlib::clock_block::init() { s_out.val = 0.0; }

void tmdl::stdlib::clock_block::step() { s_out.val += time_step; }

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
    default:
        return "";
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
        return "";
    }
}

const char* tmdl::stdlib::trig_func_to_string(const TrigFunction fcn) {
    switch (fcn) {
        using enum TrigFunction;
    case SIN:
        return "tmdl::stdlib::TrigFunction::SIN";
    case COS:
        return "tmdl::stdlib::TrigFunction::COS";
    default:
        return "";
    }
}
