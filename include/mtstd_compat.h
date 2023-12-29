// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_COMPAT_H
#define MT_STDLIB_COMPAT_H

#ifdef MT_STDLIB_USE_FULL_LIB

#include <cstdint>

#ifdef __cplusplus
extern "C" {
#endif

struct model_block;

struct model_value {
    uint32_t type;
    uint32_t size;
    void* data;
};

struct block_info {
    const char* name;
    const char* display;
    const char* create_flags;
    const block_info* next;
};

block_info* mt_stdlib_info_init();

int32_t mt_stdlib_info_destroy(block_info* info);

model_block* mt_stdlib_create(const char* name, uint32_t data_type);

model_block* mt_stdlib_create_size(const char* name, uint32_t data_type, uint32_t size);

model_block* mt_stdlib_create_with_value(const char* name, const model_value* value);

model_block* mt_stdlib_create_with_dt(const char* name, uint32_t data_type, double dt);

void mt_stdlib_destroy(const model_block* blk);

void mt_stdlib_step(model_block* blk);

void mt_stdlib_reset(model_block* blk);

int32_t mt_stdlib_get_class_name(const model_block* blk, char* str, uint32_t buffer_size);

int32_t mt_stdlib_get_full_name(const model_block* blk, char* str, uint32_t buffer_size);

int32_t mt_stdlib_set_input(model_block* blk, uint32_t port_num, const model_value* value);

int32_t mt_stdlib_get_output(const model_block* blk, uint32_t port_num, model_value* value);

int32_t mt_stdlib_get_num_inputs(const model_block* blk, uint32_t* num);

int32_t mt_stdlib_get_num_outputs(const model_block* blk, uint32_t* num);

int32_t mt_stdlib_get_input_type(const model_block* blk, uint32_t port_num, uint32_t* data_type);

int32_t mt_stdlib_get_output_type(const model_block* blk, const uint32_t port_num, uint32_t* data_type);

#ifdef __cplusplus
}
#endif

#endif // MT_STDLIB_USE_FULL_LIB

#endif // MT_STDLIB_COMPAT_H
