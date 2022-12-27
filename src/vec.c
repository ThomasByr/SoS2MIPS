#include <pthread.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "vec.h"

struct vec_s {
  void **data;
  size_t size;
  size_t capacity;

  pthread_mutex_t *lock;
};

vec_t _vec_new_args(vec_args_t args) {
  size_t size = args.size ? args.size : 1;
  return _vec_new(size);
}

vec_t _vec_new(size_t size) {
  vec_t vec = malloc(sizeof(struct vec_s));
  vec->data = calloc(size, sizeof(void *) * size);
  vec->size = 0;
  vec->capacity = size;
  vec->lock = malloc(sizeof(pthread_mutex_t));
  TCHK(pthread_mutex_init(vec->lock, NULL));
  return vec;
}

vec_t vec_from(void *data, ...) {
  int n = 0;
  va_list args;
  va_start(args, data);
  while (va_arg(args, void *) != NULL) {
    n++;
  }

  vec_t vec = _vec_new(n + 1);
  vec->data[0] = data;
  vec->size = n + 1;

  va_start(args, data);
  for (int i = 1; i < n + 1; i++) {
    vec->data[i] = va_arg(args, void *);
  }

  return vec;
}

void vec_free(vec_t vec) {
  free(vec->data);

  TCHK(pthread_mutex_destroy(vec->lock));
  free(vec->lock);

  free(vec);
}

size_t vec_size(vec_t vec) { return vec->size; }

size_t vec_capacity(vec_t vec) { return vec->capacity; }

void *vec_get(vec_t vec, size_t index) {
  if (index >= vec->size) {
    return NULL;
  }
  return vec->data[index];
}

void vec_set(vec_t vec, size_t index, void *data) {
  if (index >= vec->size) {
    return;
  }
  TCHK(pthread_mutex_lock(vec->lock));
  vec->data[index] = data;
  TCHK(pthread_mutex_unlock(vec->lock));
}

void vec_push(vec_t vec, void *data) {
  if ((double)vec->size >= (double)vec->capacity * 0.8) {
    TCHK(pthread_mutex_lock(vec->lock));
    vec->capacity *= 2;
    vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    TCHK(pthread_mutex_unlock(vec->lock));
  }
  TCHK(pthread_mutex_lock(vec->lock));
  vec->data[vec->size] = data;
  vec->size++;
  TCHK(pthread_mutex_unlock(vec->lock));
}

void vec_pop(vec_t vec) {
  if (vec->size == 0) {
    return;
  }
  TCHK(pthread_mutex_lock(vec->lock));
  vec->size--;
  TCHK(pthread_mutex_unlock(vec->lock));
}

void vec_insert(vec_t vec, size_t index, void *data) {
  if (index >= vec->size) {
    return;
  }
  if ((double)vec->size >= (double)vec->capacity * 0.8) {
    TCHK(pthread_mutex_lock(vec->lock));
    vec->capacity *= 2;
    vec->data = realloc(vec->data, sizeof(void *) * vec->capacity);
    TCHK(pthread_mutex_unlock(vec->lock));
  }
  TCHK(pthread_mutex_lock(vec->lock));
  for (size_t i = vec->size; i > index; i--) {
    vec->data[i] = vec->data[i - 1];
  }
  vec->data[index] = data;
  vec->size++;
  TCHK(pthread_mutex_unlock(vec->lock));
}

vec_t vec_append(vec_t vec1, vec_t vec2) {

  for (size_t i = 0; i < vec2->size; i++) {
    vec_push(vec1, vec2->data[i]);
  }

  return vec1;
}

void vec_remove(vec_t vec, size_t index) {
  if (index >= vec->size) {
    return;
  }
  TCHK(pthread_mutex_lock(vec->lock));
  for (size_t i = index; i < vec->size - 1; i++) {
    vec->data[i] = vec->data[i + 1];
  }
  vec->size--;
  TCHK(pthread_mutex_unlock(vec->lock));
}

void vec_clear(vec_t vec) {
  TCHK(pthread_mutex_lock(vec->lock));
  vec->size = 0;
  TCHK(pthread_mutex_unlock(vec->lock));
}

void *vec_first(vec_t vec) {
  if (vec->size == 0) {
    return NULL;
  }
  return vec->data[0];
}

void *vec_last(vec_t vec) {
  if (vec->size == 0) {
    return NULL;
  }
  return vec->data[vec->size - 1];
}
