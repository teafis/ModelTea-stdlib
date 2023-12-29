// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_COMPAT_H
#define MT_STDLIB_COMPAT_H

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

struct model_block;

model_block* mt_stdlib_create(const char* name, uint32_t data_type);

model_block* mt_stdlib_create_size(const char* name, uint32_t data_type, uint32_t size);

void mt_stdlib_destroy(const model_block* blk);

void mt_stdlib_step(model_block* blk);

void mt_stdlib_reset(model_block* blk);

#ifdef MT_STDLIB_USE_STRING_FUNCS

int32_t mt_stdlib_get_class_name(const model_block* blk, char* str, uint32_t buffer_size);

int32_t mt_stdlib_get_full_name(const model_block* blk, char* str, uint32_t buffer_size);

#endif

int32_t mt_stdlib_set_input(model_block* blk, uint32_t port_num, uint32_t data_type, const void* data, uint32_t data_size);

int32_t mt_stdlib_get_output(const model_block* blk, uint32_t port_num, uint32_t data_type, void* data, uint32_t data_size);

int32_t mt_stdlib_get_num_inputs(const model_block* blk, uint32_t* num);

int32_t mt_stdlib_get_num_outputs(const model_block* blk, uint32_t* num);

int32_t mt_stdlib_get_input_type(const model_block* blk, uint32_t* data_type);

int32_t mt_stdlib_get_output_type(const model_block* blk, uint32_t* data_type);

#ifdef __cplusplus
}
#endif

#endif // MT_STDLIB_COMPAT_H
