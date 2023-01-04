#include <stdarg.h>
#include <stdio.h>

#include "assstack.h"
#include "dict.h"
#include "vec.h"

#include "lib.h"

#define MSG_SIZE 1 << 7

struct astack_s {
  vec_t data;  // data section
  dict_t text; // text section (dict_t of vec_t)
};

astack_t _astack_new_args(struct astack_args_s args) {
  astack_t stack = malloc(sizeof(struct astack_s));
  stack->data = vec_new(args._data_size ? args._data_size : 1);
  stack->text = dict_new(1);
  dict_push(stack->text, "main",
            vec_new(args._text_size ? args._text_size : 1));
  return stack;
}

astack_t _astack_new(size_t data_size, size_t text_size) {
  return _astack_new_args((struct astack_args_s){data_size, text_size});
}

void astack_free(astack_t stack) {
  // free vectors of strings
  char *el;
  vec_foreach(stack->data, i, el) { free(el); }

  dict_itr_t itr = dict_itr_new(stack->text);
  while (dict_itr_has_next(itr)) {
    vec_t v = (vec_t)dict_itr_value(itr);
    vec_foreach(v, i, el) { free(el); }
    vec_free(v);
    dict_itr_next(itr);
  }

  // free vectors
  vec_free(stack->data);
  dict_itr_free(itr);
  dict_free(stack->text);

  // free stack
  free(stack);
}

void astack_push_data(astack_t stack, const char *restrict fmt, ...) {
  char *buf = malloc(MSG_SIZE);
  if (buf == NULL) panic("malloc failed");
  va_list args;
  va_start(args, fmt);
  vsnprintf_s(buf, MSG_SIZE, fmt, args);
  va_end(args);
  vec_push(stack->data, buf);
}

void astack_push_text(astack_t stack, const char *restrict block,
                      const char *restrict fmt, ...) {
  char *buf = malloc(MSG_SIZE);
  if (buf == NULL) panic("malloc failed");

  // get block
  vec_t v;
  ASSERT(block != NULL); // shouldn't be for debugging purposes
  if (block == NULL) {
    v = (vec_t)dict_get(stack->text, "main");
  } else {
    v = (vec_t)dict_get(stack->text, (char *)block);
    if (v == NULL) {
      v = vec_new();
      int n = dict_push(stack->text, (char *)block, v);
      ASSERT(n == 1);
    }
  }

  // push instruction
  va_list args;
  va_start(args, fmt);
  vsnprintf_s(buf, MSG_SIZE, fmt, args);
  va_end(args);

  vec_push(v, buf);
}

void astack_fprintf(astack_t stack, FILE *stream) {
  char *el;
  fprintf(stream, "\n.data\n");
  vec_foreach(stack->data, i, el) { fprintf(stream, "%s\n", el); }
  fprintf(stream, "\n.text\n.globl main\n");

  dict_itr_t itr = dict_itr_new(stack->text);
  while (dict_itr_has_next(itr)) {
    const char *key = (char *)dict_itr_key(itr);
    fprintf(stream, "\n%s:\n", key);
    vec_t v = (vec_t)dict_itr_value(itr);
    vec_foreach(v, i, el) { fprintf(stream, "%s\n", el); }
    dict_itr_next(itr);
  }
  dict_itr_free(itr);
}
