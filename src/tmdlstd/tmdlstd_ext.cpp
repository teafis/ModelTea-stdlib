// SPDX-License-Identifier: MIT

#include "tmdlstd_ext.hpp"

#include <cmath>

float tmdl::stdlib::t_sin(const float x) { return std::sin(x); }
double tmdl::stdlib::t_sin(const double x) { return std::sin(x); }

float tmdl::stdlib::t_cos(const float x) { return std::cos(x); }
double tmdl::stdlib::t_cos(const double x) { return std::cos(x); }

float tmdl::stdlib::t_tan(const float x) { return std::tan(x); }
double tmdl::stdlib::t_tan(const double x) { return std::tan(x); }

float tmdl::stdlib::t_asin(const float x) { return std::asin(x); }
double tmdl::stdlib::t_asin(const double x) { return std::asin(x); }

float tmdl::stdlib::t_acos(const float x) { return std::acos(x); }
double tmdl::stdlib::t_acos(const double x) { return std::acos(x); }

float tmdl::stdlib::t_atan(const float x) { return std::atan(x); }
double tmdl::stdlib::t_atan(const double x) { return std::atan(x); }

float tmdl::stdlib::t_atan2(const float y, const float x) {
    return std::atan2(y, x);
}
double tmdl::stdlib::t_atan2(const double y, const double x) {
    return std::atan2(y, x);
}

int32_t tmdl::stdlib::t_mod(const int32_t x, const int32_t y) { return x % y; }
float tmdl::stdlib::t_mod(const float x, const float y) {
    return std::fmod(x, y);
}
double tmdl::stdlib::t_mod(const double x, const double y) {
    return std::fmod(x, y);
}
