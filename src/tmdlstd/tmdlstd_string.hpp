// SPDX-License-Identifier: MIT

#ifndef TMDL_STDLIB_STRING_H
#define TMDL_STDLIB_STRING_H

#include "tmdlstd.hpp"

namespace tmdl::stdlib {

const char* arith_to_string(ArithType t);

const char* relational_to_string(RelationalOperator op);

const char* trig_func_to_string(TrigFunction fcn);

const char* trig_func_to_string(TrigFunction2 fcn);

}

#endif // TMDL_STDLIB_STRING_H
