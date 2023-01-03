#include <stdio.h>
#include <stdarg.h>

#include "assstack.h"
#include "vec.h"
#include "lib.h"

#define MSG_SIZE 1 << 7

struct astack_s {
  vec_t data;
  vec_t text;
};

astack_t _astack_new_args(struct astack_args_s args) {
  astack_t stack = malloc(sizeof(struct astack_s));
  stack->data = vec_new(args._data_size ? args._data_size : 1);
  stack->text = vec_new(args._text_size ? args._text_size : 1);
  return stack;
}

astack_t _astack_new(size_t data_size, size_t text_size) {
  return _astack_new_args((struct astack_args_s){data_size, text_size});
}

void astack_free(astack_t stack) {
  // free vectors of strings
  char *el;
  vec_foreach(stack->data, i, el) {free(el);}
  vec_foreach(stack->text, i, el) {free(el);}

  // free vectors
  vec_free(stack->data);
  vec_free(stack->text);

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

void astack_push_text(astack_t stack, const char *restrict fmt, ...) {
  char *buf = malloc(MSG_SIZE);
  if (buf == NULL) panic("malloc failed");
  va_list args;
  va_start(args, fmt);
  vsnprintf_s(buf, MSG_SIZE, fmt, args);
  va_end(args);
  vec_push(stack->text, buf);
}

void astack_fprintf(astack_t stack, FILE *stream) {
  char *el;
  fprintf(stream, "\n.data\n");
  vec_foreach(stack->data, i, el) {fprintf(stream, "%s\n", el);}
  fprintf(stream, "\n.text\n.globl main\n\nmain:\n");
  vec_foreach(stack->text, i, el) {fprintf(stream, "%s\n", el);}

}
