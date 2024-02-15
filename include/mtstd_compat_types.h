// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_COMPAT_TYPES_H
#define MT_STDLIB_COMPAT_TYPES_H

#include <cstdint>

#ifdef MT_STDLIB_USE_FULL_LIB

#ifdef __cplusplus
extern "C" {
#endif

struct mt_block_t;

struct mt_value_t {
    uint32_t type;
    uint32_t size;
    void* data;
};

struct mt_value_list_t {
    mt_value_t value;
    mt_value_list_t* next;
};

struct mt_block_info_t {
    const char* name;
    const char* sub_name;
    int32_t create_flags;
    int32_t type_flags;
    const mt_block_info_t* next;
};

#ifdef __cplusplus
}
#endif

#endif // MT_STDLIB_USE_FULL_LIB

#endif // MT_STDLIB_COMPAT_TYPES_H
