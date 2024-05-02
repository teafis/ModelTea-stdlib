// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_STRING_H
#define MT_STDLIB_STRING_H

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstd_types.hpp"

#include <string>

namespace mt {
namespace stdlib {

extern const std::string BASE_NAMESPACE;

extern const std::string BLK_NAME_ARITH;
extern const std::string BLK_NAME_CLOCK;
extern const std::string BLK_NAME_CONST;
extern const std::string BLK_NAME_DELAY;
extern const std::string BLK_NAME_DERIV;
extern const std::string BLK_NAME_INTEG;
extern const std::string BLK_NAME_SWITCH;
extern const std::string BLK_NAME_LIMITER;
extern const std::string BLK_NAME_REL;
extern const std::string BLK_NAME_RELEQ;
extern const std::string BLK_NAME_TRIG;

extern const std::string BLK_SUB_NAME_ARITH_ADD;
extern const std::string BLK_SUB_NAME_ARITH_SUB;
extern const std::string BLK_SUB_NAME_ARITH_MUL;
extern const std::string BLK_SUB_NAME_ARITH_DIV;
extern const std::string BLK_SUB_NAME_ARITH_MOD;
extern const std::string BLK_SUB_NAME_REL_GT;
extern const std::string BLK_SUB_NAME_REL_GEQ;
extern const std::string BLK_SUB_NAME_REL_LT;
extern const std::string BLK_SUB_NAME_REL_LEQ;
extern const std::string BLK_SUB_NAME_REL_EQ;
extern const std::string BLK_SUB_NAME_REL_NEQ;
extern const std::string BLK_SUB_NAME_TRIG_SIN;
extern const std::string BLK_SUB_NAME_TRIG_COS;
extern const std::string BLK_SUB_NAME_TRIG_TAN;
extern const std::string BLK_SUB_NAME_TRIG_ASIN;
extern const std::string BLK_SUB_NAME_TRIG_ACOS;
extern const std::string BLK_SUB_NAME_TRIG_ATAN;
extern const std::string BLK_SUB_NAME_TRIG_ATAN2;

enum class SpecificationType {
    NONE,
    TYPE,
    FULL,
};

std::string
arith_to_string(ArithType t,
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
