#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

typedef struct astack_s *astack_t;

struct astack_args_s {
  size_t _data_size;
  size_t _text_size;
};

astack_t _astack_new(size_t data_size, size_t text_size);

astack_t _astack_new_args(struct astack_args_s args);

/**
 * @brief new assembly stack
 * set size to number of elements to preallocate
 *
 */
#define astack_new(...) _astack_new_args((struct astack_args_s){__VA_ARGS__})

/**
 * @brief Free the assembly stack
 *
 * @param stack assembly stack to free
 */
void astack_free(astack_t stack);

/**
 * @brief push instruction inside data section
 *
 */
void astack_push_data(astack_t stack, const char *restrict fmt, ...);

/**
 * @brief push instruction inside text section
 * @param stack assembly stack
 * @param block block name (`main`, `loop`, ...)
 * @param fmt format string representing the instruction
 * @note if block is NULL, the instruction will be pushed in the global `main`
   block ;
 * if block is not NULL, the instruction will be pushed in the block ;
 * the block will be created if it does not exist ;
 * if the block already exists, the instruction will be pushed at the end of the
   block
 *
 */
void astack_push_text(astack_t stack, const char *restrict block,
                      const char *restrict fmt, ...);

/**
 * @brief print content to stream
 *
 * @param stack assembly stack
 * @param stream stream to print to
 */
void astack_fprintf(astack_t stack, FILE *stream);

#ifdef __cplusplus
}
#endif
