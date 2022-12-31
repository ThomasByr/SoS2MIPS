#include "protocol.h"
#include "quad.h"
#include "symtable.h"
#include "vec.h"
#include <stdbool.h>
#include <stdio.h>

extern int opt_lvl;
extern bool verbose;
extern char *output;
extern struct symtable *id_name_table;
extern vec_t quad_array;

static const char sys_call_names[][16] = {
    "print_int", "print_float", "print_double",    "print_string",
    "read_int",  "read_float",  "read_double",     "read_string",
    "sbrk",      "exit",        "print_character", "read_character",
    "open",      "read",        "write",           "close",
    "exit2",
};

static const char reg_names[][4] = {
    "$0",  "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
    "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
    "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
};

bool reg_use[32] = {false};

void reg_display(void) {
  int i;
  for (i = reg_t0; i <= reg_t7; i++) {
    printf("%d ", reg_use[i]);
  }
  printf("\n");
}

const char *reg_name(enum reg reg) { return reg_names[reg]; }

const char *sys_call_name(enum sys_call sys_call) {
  return sys_call_names[sys_call];
}

/**
 * @brief function to find the first free temporary register
 *
 * @return enum reg
 */
enum reg find_free_reg(void) {

  int i;
  for (i = reg_t0; i <= reg_t7; i++) {
    if (!reg_use[i]) {
      reg_use[i] = true;
      return i;
    }
  }
  panic("no free registers");
}

/**
 * @brief function to free a temporary register
 *
 * @param reg
 */
void free_reg(enum reg reg) { reg_use[reg] = false; }

enum reg load_reg_for_type(FILE *file, struct quadarg *quadarg) {

  if (quadarg->type == int_arg || quadarg->type == 0) {

    quadarg->reg_arg = find_free_reg();
    fprintf(file, "li %s, %d\n", reg_name(quadarg->reg_arg),
            quadarg->value.int_value);
    return quadarg->reg_arg;
  }

  else if (quadarg->type == id_arg)
    return quadarg->value.id_value->reg_temp;

  return -1;
}

/**
 * @brief Generate MIPS assembly code from the quad array
 *
 */
void generate_asm(void) {

  FILE *out = fopen(output, "w");
  if (out == NULL) panic("fopen");

  int i;
  struct quad *quad;

  for (i = vec_size(quad_array) - 1; i >= 0; i--) {
    quad = vec_get(quad_array, i);
    switch (quad->op) {
    case plus_op:

      quad->arg1->reg_arg = load_reg_for_type(out, quad->arg1);
      quad->arg2->reg_arg = load_reg_for_type(out, quad->arg2);

      quad->arg3->reg_arg = find_free_reg();
      fprintf(out, "add %s, %s, %s\n", reg_name(quad->arg3->reg_arg),
              reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

      free_reg(quad->arg1->reg_arg);
      free_reg(quad->arg2->reg_arg);

      break;

    default:
      break;
    }
  }
  fclose(out);
}