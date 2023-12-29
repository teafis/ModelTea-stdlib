// SPDX-License-Identifier: MIT

#include "mtstd.hpp"
#include "mtstd_types.hpp"

mt::stdlib::clock_block::clock_block(const double dt) : time_step{dt} {
    reset();
}

void mt::stdlib::clock_block::reset() { s_out.value = 0.0; }

void mt::stdlib::clock_block::step() { s_out.value += time_step; }

#ifdef MT_USE_C_COMPAT
bool mt::stdlib::clock_block::set_input_type(const size_t port_num, const DataType dt, const void* input, const size_t data_size) {
    return false;
}

bool mt::stdlib::clock_block::get_output_type(const size_t port_num, DataType dt, void* output, const size_t data_size) {
    if (dt != DataType::F64 || port_num != 0) {
        return false;
    } else {
        return get_output_value<DataType::F64>(s_out.value, output, data_size);
    }
}

size_t mt::stdlib::clock_block::get_input_num() const {
    return 0;
}

size_t mt::stdlib::clock_block::get_output_num() const {
    return 1;
}

mt::stdlib::DataType mt::stdlib::clock_block::get_input_type(const size_t port_num) const {
    return DataType::NONE;
}

mt::stdlib::DataType mt::stdlib::clock_block::get_output_type(const size_t port_num) const {
    if (port_num == 0) {
        return DataType::F64;
    } else {
        return DataType::NONE;
    }
}
#endif
