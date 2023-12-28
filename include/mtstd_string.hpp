// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_STRING_H
#define MT_STDLIB_STRING_H

#ifdef MT_STDLIB_USE_STRING_FUNCS

#include "mtstd_types.hpp"

#include <string>

namespace mt {
namespace stdlib {

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

extern const std::string BASE_NAMESPACE;

}
}

#endif // MT_STDLIB_USE_STRING_FUNCS

#endif // MT_STDLIB_STRING_H
