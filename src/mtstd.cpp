// SPDX-License-Identifier: MIT

#include "mtstd.hpp"
#include "mtstd_types.hpp"

mt::stdlib::clock_block::clock_block(const double dt) : time_step{dt} {
    reset();
}

void mt::stdlib::clock_block::reset() { s_out.value = 0.0; }

void mt::stdlib::clock_block::step() { s_out.value += time_step; }

#ifdef MT_USE_C_COMPAT
bool mt::stdlib::clock_block::set_input_type(const int port_num, const DataType dt, const void* input, const int data_size) {
    return false;
}

bool mt::stdlib::clock_block::get_output_type(int port_num, DataType dt, void* output, int data_size) {
    if (dt != DataType::F64 || port_num != 0) {
        return false;
    } else {
        return get_output_value<DataType::F64>(s_out.value, output, data_size);
    }
}
#endif
