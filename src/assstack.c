#include <stdarg.h>
#include <stdio.h>

#include "assstack.h"
#include "dict.h"
#include "vec.h"

#include "lib.h"

#define MSG_SIZE 1 << 7

struct astack_s {
    vec_t data;         // data section
    vec_t text;         // text section
    vec_t name;         // name of the block
    dict_t text_blocks; // text section blocks
};

astack_t _astack_new_args(struct astack_args_s args) {
    astack_t stack = malloc(sizeof(struct astack_s));
    stack->data = vec_new(args._data_size ? args._data_size : 1);
    stack->text = vec_new(args._text_size ? args._text_size : 1);
    stack->name = vec_new(args._text_size ? args._text_size : 1);
    stack->text_blocks = dict_new(1);

    vec_t main = vec_new();
    vec_push(stack->text, main);

    char *main_name = malloc(sizeof(char) * 5);
    strlcpy(main_name, "main", 5);
    dict_push(stack->text_blocks, main_name, (void *)1);
    vec_push(stack->name, main_name);
    return stack;
}

astack_t _astack_new(size_t data_size, size_t text_size) {
    return _astack_new_args((struct astack_args_s){data_size, text_size});
}

void astack_free(astack_t stack) {
    // free vectors of strings
    char *el;
    vec_foreach(stack->data, i, el) { free(el); }

    dict_itr_t itr = dict_itr_new(stack->text_blocks);
    while (dict_itr_has_next(itr)) {
        char *key = (char *)dict_itr_key(itr);
        free(key);
        dict_itr_next(itr);
    }

    // free vectors
    vec_free(stack->data);
    dict_itr_free(itr);
    dict_free(stack->text_blocks);

    // free stack
    free(stack);

    // free vectors of vectors
    vec_t v;
    vec_foreach(stack->text, i, v) {
        vec_foreach(v, j, el) { free(el); }
        vec_free(v);
    }
    vec_free(stack->name);
    vec_free(stack->text);
}

void astack_push_data(astack_t stack, const char *restrict fmt, ...) {
    char *buf = malloc(MSG_SIZE);
    if (buf == NULL)
        panic("malloc failed");

    va_list args;
    va_start(args, fmt);
    vsnprintf_s(buf, MSG_SIZE, fmt, args);
    va_end(args);

    vec_push(stack->data, buf);
}

void astack_push_text(astack_t stack, const char *restrict block,
                      const char *restrict fmt, ...) {
    char *buf = malloc(MSG_SIZE);
    if (buf == NULL)
        panic("malloc failed");

    // get block
    vec_t v;
    ASSERT(block != NULL); // shouldn't be for debugging purposes
    size_t n = dict_get(stack->text_blocks, (void *)block);
    size_t size = vec_size(stack->text);
    if (n == 0) {
        v = vec_new();

        dict_push(stack->text_blocks, (void *)block, (void *)(size + 1ul));
        vec_push(stack->text, v);
        vec_push(stack->name, (void *)block);
    } else {
        v = vec_get(stack->text, n - 1);
    }
    ASSERT(v != NULL);

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

    vec_t v;
    vec_foreach(stack->text, i, v) {
        char *name = vec_get(stack->name, i);
        fprintf(stream, "\n%s:\n", name);
        vec_foreach(v, j, el) { fprintf(stream, "%s\n", el); }
    }
}
