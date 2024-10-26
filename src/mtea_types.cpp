// SPDX-License-Identifier: MIT

#ifdef MTEA_USE_FULL_LIB

#include "mtea_types.hpp"
#include "mtea_string.hpp"

#include <array>
#include <sstream>

static const mtea::type_info<mtea::DataType::U8> s_t_u8{};
static const mtea::type_info<mtea::DataType::U16> s_t_u16{};
static const mtea::type_info<mtea::DataType::U32> s_t_u32{};
static const mtea::type_info<mtea::DataType::U64> s_t_u64{};
static const mtea::type_info<mtea::DataType::I8> s_t_i8{};
static const mtea::type_info<mtea::DataType::I16> s_t_i16{};
static const mtea::type_info<mtea::DataType::I32> s_t_i32{};
static const mtea::type_info<mtea::DataType::I64> s_t_i64{};
static const mtea::type_info<mtea::DataType::F32> s_t_f32{};
static const mtea::type_info<mtea::DataType::F64> s_t_f64{};
static const mtea::type_info<mtea::DataType::BOOL> s_t_bool{};

const static auto s_type_array = std::to_array<const mtea::type_info_meta*>({
    &s_t_u8,
    &s_t_u16,
    &s_t_u32,
    &s_t_u64,
    &s_t_i8,
    &s_t_i16,
    &s_t_i32,
    &s_t_i64,
    &s_t_f32,
    &s_t_f64,
    &s_t_bool,
});

const mtea::type_info_meta* mtea::get_meta_type(DataType dt) {
    for (const auto& t : s_type_array) {
        if (t->get_data_type() == dt) {
            return t;
        }
    }

    return nullptr;
}

const mtea::type_info_meta* mtea::get_meta_type(std::string_view s) {
    for (const auto& t : s_type_array) {
        if (t->get_name() == s) {
            return t;
        }
    }

    return nullptr;
}

std::vector<const mtea::type_info_meta*> mtea::get_meta_types() {
    return std::vector<const mtea::type_info_meta*>(s_type_array.begin(), s_type_array.end());
}

void mtea::block_interface::reset() noexcept {}

void mtea::block_interface::step() noexcept {}

bool mtea::block_interface::outputs_are_delayed() const noexcept { return false; }

std::string mtea::block_interface::get_type_name(bool use_codegen_name) const {
    std::ostringstream oss;
    oss << BASE_NAMESPACE << "::";
    if (use_codegen_name) {
        oss << get_class_name_codegen();
    } else {
        oss << get_class_name();
    }
    return oss.str();
}

std::string mtea::block_interface::get_class_name_codegen() const { return get_class_name(); }

#endif // MTEA_USE_FULL_LIB
