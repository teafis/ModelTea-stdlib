#include "mtstd_types.hpp"

#include <sstream>

#if MT_USE_C_COMPAT

void mt::stdlib::block_interface::reset() {}

void mt::stdlib::block_interface::step() {}

#ifdef MT_STDLIB_USE_STRING_FUNCS
std::string mt::stdlib::block_interface::get_type_name(bool include_namespace) {
    if (include_namespace) {
        std::ostringstream oss;
        oss << BASE_NAMESPACE << "::" << get_inner_type_name();
        return oss.str();
    } else {
        return get_inner_type_name();
    }
}
#endif

#endif
