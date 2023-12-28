// SPDX-License-Identifier: MIT

#include "tmdlstd.hpp"

mt::stdlib::clock_block::clock_block(const double dt) : time_step{dt} {
    reset();
}

void mt::stdlib::clock_block::reset() { s_out.val = 0.0; }

void mt::stdlib::clock_block::step() { s_out.val += time_step; }
