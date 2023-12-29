// SPDX-License-Identifier: MIT

#ifndef MT_STDLIB_COMPAT_H
#define MT_STDLIB_COMPAT_H

#ifdef __cplusplus
extern "C" {
#endif

struct model_block;

const model_block* mt_stdlib_create_block(const char* name, int data_type);

void mt_stdlib_block_destory(const model_block* blk);

void mt_stdlib_block_step(model_block* blk);

void mt_stdlib_block_reset(model_block* blk);

#ifdef MT_STDLIB_USE_STRING_FUNCS

int mt_stdlib_block_get_class_name(const model_block* blk, char* str, int buffer_size);

int mt_stdlib_block_get_full_name(const model_block* blk, char* str, int buffer_size);

#endif

int mt_stdlib_set_input(model_block* blk, int port_num, int data_type, const void* data, int data_size);

int mt_stdlib_get_output(const model_block* blk, int port_num, int data_type, void* data, int data_size);

#ifdef __cplusplus
}
#endif

#endif // MT_STDLIB_COMPAT_H
