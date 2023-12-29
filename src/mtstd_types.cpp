// SPDX-License-Identifier: MIT

#if MT_STDLIB_USE_FULL_LIB

#include "mtstd_types.hpp"

#include <sstream>

void mt::stdlib::block_interface::reset() {}

void mt::stdlib::block_interface::step() {}

std::string mt::stdlib::block_interface::get_type_name(bool include_namespace) {
    if (include_namespace) {
        std::ostringstream oss;
        oss << BASE_NAMESPACE << "::" << get_class_name();
        return oss.str();
    } else {
        return get_class_name();
    }
}

#endif // MT_STDLIB_USE_FULL_LIB
