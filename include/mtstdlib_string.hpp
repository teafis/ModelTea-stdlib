// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_STRING_H
#define MT_STDLIB_STRING_H

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstdlib_string_common.hpp"
#include "mtstdlib_types.hpp"

#include <string>

namespace mt {
namespace stdlib {

extern constinit std::string BLK_NAME_CLOCK;
extern constinit std::string BLK_NAME_CONST;
extern constinit std::string BLK_NAME_DELAY;
extern constinit std::string BLK_NAME_DERIV;
extern constinit std::string BLK_NAME_INTEG;
extern constinit std::string BLK_NAME_SWITCH;
extern constinit std::string BLK_NAME_LIMITER;
extern constinit std::string BLK_NAME_ARITH_ADD;
extern constinit std::string BLK_NAME_ARITH_SUB;
extern constinit std::string BLK_NAME_ARITH_MUL;
extern constinit std::string BLK_NAME_ARITH_DIV;
extern constinit std::string BLK_NAME_ARITH_MOD;
extern constinit std::string BLK_NAME_REL_GT;
extern constinit std::string BLK_NAME_REL_GEQ;
extern constinit std::string BLK_NAME_REL_LT;
extern constinit std::string BLK_NAME_REL_LEQ;
extern constinit std::string BLK_NAME_REL_EQ;
extern constinit std::string BLK_NAME_REL_NEQ;
extern constinit std::string BLK_NAME_TRIG_SIN;
extern constinit std::string BLK_NAME_TRIG_COS;
extern constinit std::string BLK_NAME_TRIG_TAN;
extern constinit std::string BLK_NAME_TRIG_ASIN;
extern constinit std::string BLK_NAME_TRIG_ACOS;
extern constinit std::string BLK_NAME_TRIG_ATAN;
extern constinit std::string BLK_NAME_TRIG_ATAN2;

enum class SpecificationType {
    NONE,
    TYPE,
    FULL,
};

std::string datatype_to_string(
    DataType dt,
    SpecificationType specification = SpecificationType::FULL);

std::string arith_to_string(
    ArithType t,
    SpecificationType specification = SpecificationType::FULL);

std::string relational_to_string(
    RelationalOperator op,
    SpecificationType specification = SpecificationType::FULL);

std::string trig_func_to_string(
    TrigFunction fcn,
    SpecificationType specification = SpecificationType::FULL);

std::string with_namespace(const std::string& name);

}
}

#endif // MT_STDLIB_USE_FULL_LIB

#endif // MT_STDLIB_STRING_H
