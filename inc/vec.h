/**
 * @file vec.h
 * @author Thomas Byr
 * @brief dynamic array implementation
 * @version 1.0
 * @date 2022-12-17
 *
 * @copyright Copyright (c) 2022 GNU GPL v3
 */

#pragma once

#include <stdlib.h>

#include "lib.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct vec_s *vec_t;

struct vec_args_s {
  size_t size;
};
typedef struct vec_args_s vec_args_t;

/**
 * @brief Create a new vector
 *
 * @return vec_t - the new vector
 */
vec_t _vec_new(size_t size);

vec_t _vec_new_args(vec_args_t args);

/**
 * @brief new vector
   set size to number of elements to preallocate
 *
 */
#define vec_new(...) _vec_new_args((vec_args_t){__VA_ARGS__})

/**
 * @brief Create a new vector from a list of elements
 *
 * @param data NULL terminated list of elements
 * @param ... list of elements, terminated by NULL
 * @return vec_t - the new vector
 */
vec_t vec_from(void *data, ...);

/**
 * @brief Free the vector
 *
 * @param vec vector to free
 */
void vec_free(vec_t vec);

/**
 * @brief Get the size of the vector
 *
 * @param vec vector
 * @return size_t - size of the vector
 */
size_t vec_size(vec_t vec);

/**
 * @brief Get the capacity of the vector
 *
 * @param vec vector
 * @return size_t - capacity of the vector
 */
size_t vec_capacity(vec_t vec);

/**
 * @brief Get the element at the given index
 *
 * @param vec vector
 * @param index index
 * @return void* - element at the given index
 */
void *vec_get(vec_t vec, size_t index);

/**
 * @brief Set the element at the given index
 *
 * @param vec vector
 * @param index index
 * @param data element to set
 */
void vec_set(vec_t vec, size_t index, void *data);

/**
 * @brief Add an element to the end of the vector
 *
 * @param vec vector
 * @param data element to add
 */
void vec_push(vec_t vec, void *data);

/**
 * @brief Remove the last element of the vector
 *
 * @param vec vector
 */
void vec_pop(vec_t vec);

/**
 * @brief Insert an element at the given index
 *
 * @param vec vector
 * @param index index
 * @param data element to insert
 */
void vec_insert(vec_t vec, size_t index, void *data);

/**
 * @brief Remove the element at the given index
 *
 * @param vec vector
 * @param index index
 */
void vec_remove(vec_t vec, size_t index);

/**
 * @brief Remove all elements from the vector
 *
 * @param vec vector
 */
void vec_clear(vec_t vec);

/**
 * @brief Get the first element of the vector
 *
 * @param vec vector
 * @return void* - first element of the vector
 */
void *vec_first(vec_t vec);

/**
 * @brief Get the last element of the vector
 *
 * @param vec vector
 * @return void* - last element of the vector
 */
void *vec_last(vec_t vec);

/**
 * @brief Append vector 2 to vector 1
 *
 * @param vec1 vector 1
 * @param vec2 vector 2
 * @return vec_t - concatenated vector
 */
vec_t vec_append(vec_t vec1, vec_t vec2);

// iterate over the vector
#define vec_foreach(vec, i, data) \
  for (size_t i = 0; i < vec_size(vec) && (data = vec_get(vec, i)); i++)

#ifdef __cplusplus
}
#endif
