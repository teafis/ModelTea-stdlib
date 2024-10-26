// SPDX-License-Identifier: MIT

#include "mtea_math.hpp"

#include <cmath>

#define GENERATE_FNS_1(FN) \
float mtea::t_ ## FN(const float x) { return std::FN(x); } \
double mtea::t_ ## FN(const double x) { return std::FN(x); }

#define GENERATE_FNS_2(FN) \
float mtea::t_ ## FN(const float a, const float b) { return std::FN(a, b); } \
double mtea::t_ ## FN(const double a, const double b) { return std::FN(a, b); }

GENERATE_FNS_1(sin)
GENERATE_FNS_1(cos)
GENERATE_FNS_1(tan)
GENERATE_FNS_1(asin)
GENERATE_FNS_1(acos)
GENERATE_FNS_1(atan)
GENERATE_FNS_2(atan2)

uint32_t mtea::t_mod(uint32_t x, uint32_t y) { return x % y; }
int32_t mtea::t_mod(const int32_t x, const int32_t y) { return x % y; }
float mtea::t_mod(const float x, const float y) {
    return std::fmod(x, y);
}
double mtea::t_mod(const double x, const double y) {
    return std::fmod(x, y);
}
