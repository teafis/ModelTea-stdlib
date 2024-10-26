// SPDX-License-Identifier: MIT

#ifndef MTEA_MATH_H
#define MTEA_MATH_H

#include <cstdint>

namespace mtea {

float t_sin(float x);
double t_sin(double x);

float t_cos(float x);
double t_cos(double x);

float t_tan(float x);
double t_tan(double x);

float t_asin(float x);
double t_asin(double x);

float t_acos(float x);
double t_acos(double x);

float t_atan(float x);
double t_atan(double x);

float t_atan2(float y, float x);
double t_atan2(double y, double x);

uint32_t t_mod(uint32_t x, uint32_t y);
int32_t t_mod(int32_t x, int32_t y);
float t_mod(float x, float y);
double t_mod(double x, double y);

}

#endif // MTEA_MATH_H
