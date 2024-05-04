// SPDX-License-Identifier: MIT

#ifdef MT_STDLIB_USE_FULL_LIB

#include "mtstdlib_types.hpp"

#include <array>
#include <sstream>

static const mt::stdlib::type_info<mt::stdlib::DataType::U8> s_t_u8{};
static const mt::stdlib::type_info<mt::stdlib::DataType::U16> s_t_u16{};
static const mt::stdlib::type_info<mt::stdlib::DataType::U32> s_t_u32{};
static const mt::stdlib::type_info<mt::stdlib::DataType::I8> s_t_i8{};
static const mt::stdlib::type_info<mt::stdlib::DataType::I16> s_t_i16{};
static const mt::stdlib::type_info<mt::stdlib::DataType::I32> s_t_i32{};
static const mt::stdlib::type_info<mt::stdlib::DataType::F32> s_t_f32{};
static const mt::stdlib::type_info<mt::stdlib::DataType::F64> s_t_f64{};
static const mt::stdlib::type_info<mt::stdlib::DataType::BOOL> s_t_bool{};

const static auto s_type_array = std::to_array<const mt::stdlib::type_info_meta*>({
    &s_t_u8,
    &s_t_u16,
    &s_t_u32,
    &s_t_i8,
    &s_t_i16,
    &s_t_i32,
    &s_t_f32,
    &s_t_f64,
    &s_t_bool,
});

const mt::stdlib::type_info_meta* mt::stdlib::get_meta_type(DataType dt) {
    for (const auto& t : s_type_array) {
        if (t->get_data_type() == dt) {
            return t;
        }
    }

    return nullptr;
}

const mt::stdlib::type_info_meta* mt::stdlib::get_meta_type(std::string_view s) {
    for (const auto& t : s_type_array) {
        if (t->get_name() == s) {
            return t;
        }
    }

    return nullptr;
}

std::vector<const mt::stdlib::type_info_meta*> mt::stdlib::get_meta_types() {
    return std::vector<const mt::stdlib::type_info_meta*>(s_type_array.begin(), s_type_array.end());
}

void mt::stdlib::block_interface::reset() noexcept {}

void mt::stdlib::block_interface::step() noexcept {}

std::string mt::stdlib::block_interface::get_type_name(bool include_namespace) const {
    if (include_namespace) {
        std::ostringstream oss;
        oss << BASE_NAMESPACE << "::" << get_class_name();
        return oss.str();
    } else {
        return get_class_name();
    }
}

#endif // MT_STDLIB_USE_FULL_LIB
