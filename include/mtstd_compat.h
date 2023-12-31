// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_COMPAT_H
#define MT_STDLIB_COMPAT_H

#ifdef MT_STDLIB_USE_FULL_LIB

#include <cstdint>

#include "mtstd_compat_types.h"

#ifdef __cplusplus
extern "C" {
#endif

extern const char* MT_TYPE_FLAGS_NUMERIC;
extern const char* MT_TYPE_FLAGS_FLOAT;
extern const char* MT_TYPE_FLAGS_ALL;

extern const char* MT_INIT_DEFAULT;
extern const char* MT_INIT_SIZE ;
extern const char* MT_INIT_VALUE;

const mt_block_info_t* mt_stdlib_info_init();

void mt_stdlib_info_destroy(const mt_block_info_t* info);

uint32_t mt_stdlib_type_size(uint32_t data_type);

mt_block_creation_t mt_stdlib_blk_create(const char* name, uint32_t data_type);

mt_block_creation_t mt_stdlib_blk_create_with_size(const char* name, uint32_t data_type, uint32_t size);

mt_block_creation_t mt_stdlib_blk_create_with_value(const char* name, const mt_value_t* value);

void mt_stdlib_blk_destroy(const mt_block_t* blk);

void mt_stdlib_blk_step(mt_block_t* blk);

void mt_stdlib_blk_reset(mt_block_t* blk);

int32_t mt_stdlib_blk_get_class_name(const mt_block_t* blk, char* str, uint32_t buffer_size);

int32_t mt_stdlib_blk_get_full_name(const mt_block_t* blk, char* str, uint32_t buffer_size);

int32_t mt_stdlib_blk_set_input(mt_block_t* blk, uint32_t port_num, const mt_value_t* value);

int32_t mt_stdlib_blk_get_output(const mt_block_t* blk, uint32_t port_num, mt_value_t* value);

int32_t mt_stdlib_blk_get_num_inputs(const mt_block_t* blk, uint32_t* num);

int32_t mt_stdlib_blk_get_num_outputs(const mt_block_t* blk, uint32_t* num);

int32_t mt_stdlib_blk_get_input_type(const mt_block_t* blk, uint32_t port_num, uint32_t* data_type);

int32_t mt_stdlib_blk_get_output_type(const mt_block_t* blk, const uint32_t port_num, uint32_t* data_type);

#ifdef __cplusplus
}
#endif

#endif // MT_STDLIB_USE_FULL_LIB

#endif // MT_STDLIB_COMPAT_H
