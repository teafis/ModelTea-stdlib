// SPDX-License-Identifier: MIT

#ifndef TMDL_STDLIB_STRING_H
#define TMDL_STDLIB_STRING_H

#include "tmdlstd.hpp"

#include <string>

namespace tmdl::stdlib {

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

}

#endif // TMDL_STDLIB_STRING_H
