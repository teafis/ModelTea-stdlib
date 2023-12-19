// SPDX-License-Identifier: MIT

#include "tmdlstd.hpp"

tmdl::stdlib::clock_block::clock_block(const double dt) : time_step{dt} {
    init();
}

void tmdl::stdlib::clock_block::init() { s_out.val = 0.0; }

void tmdl::stdlib::clock_block::step() { s_out.val += time_step; }

void tmdl::stdlib::clock_block::reset() { s_out.val = 0.0; }
