// SPDX-License-Identifier: MIT

#include "mtstd_ext.hpp"

#include <cmath>

float mt::stdlib::t_sin(const float x) { return std::sin(x); }
double mt::stdlib::t_sin(const double x) { return std::sin(x); }

float mt::stdlib::t_cos(const float x) { return std::cos(x); }
double mt::stdlib::t_cos(const double x) { return std::cos(x); }

float mt::stdlib::t_tan(const float x) { return std::tan(x); }
double mt::stdlib::t_tan(const double x) { return std::tan(x); }

float mt::stdlib::t_asin(const float x) { return std::asin(x); }
double mt::stdlib::t_asin(const double x) { return std::asin(x); }

float mt::stdlib::t_acos(const float x) { return std::acos(x); }
double mt::stdlib::t_acos(const double x) { return std::acos(x); }

float mt::stdlib::t_atan(const float x) { return std::atan(x); }
double mt::stdlib::t_atan(const double x) { return std::atan(x); }

float mt::stdlib::t_atan2(const float y, const float x) {
    return std::atan2(y, x);
}
double mt::stdlib::t_atan2(const double y, const double x) {
    return std::atan2(y, x);
}

uint32_t mt::stdlib::t_mod(uint32_t x, uint32_t y) { return x % y; }
int32_t mt::stdlib::t_mod(const int32_t x, const int32_t y) { return x % y; }
float mt::stdlib::t_mod(const float x, const float y) {
    return std::fmod(x, y);
}
double mt::stdlib::t_mod(const double x, const double y) {
    return std::fmod(x, y);
}
