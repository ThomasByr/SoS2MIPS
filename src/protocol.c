#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "assstack.h"
#include "protocol.h"
#include "quad.h"
#include "symtable.h"
#include "vec.h"

#define ARG (void *)0x42

extern int opt_lvl;
extern bool verbose;
extern char *output;
extern struct symtable *id_name_table;
extern vec_t quad_array;

extern char *quad_op_string[];

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

/**
 * @brief Generate MIPS assembly code from the quad array
 *
 */
void generate_asm(FILE *out) {

  size_t i;
  struct quad *quad;
  // char substring[BUFSIZ];
  // char *strstr_string = NULL;
  // size_t str_count = 0;
  int msg_count = 0;
  astack_t stack = astack_new(2, 10); // random preallocation

  // fprintf(out, ".data\n\n");
  // fprintf(out, ".text\n");

  for (i = 0; i < vec_size(quad_array); i++) {
    quad = vec_get(quad_array, i);
    switch (quad->op) {

    case assn_int_to_var_op:

      quad->arg3->type = int_arg;
      quad->arg3->reg_arg = find_free_reg();
      astack_push_text(stack, "li %s, %d", reg_name(quad->arg3->reg_arg),
                       quad->arg1->value.int_value);

      break;

    case assn_arg_to_var_op:
      break;

    case assn_string_to_var_op:

      quad->arg3->type = str_arg;
      quad->arg3->reg_arg = find_free_reg();
      astack_push_data(stack, "msg%d: .asciiz \"%s\"", msg_count,
                       quad->arg1->value.str_value);

      break;

    case plus_op:

      // sum int rule
      if (quad->arg1->type == int_arg && quad->arg2->type == int_arg) {

        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, "add %s, %s, %s", reg_name(quad->arg3->reg_arg),
                         reg_name(quad->arg1->reg_arg),
                         reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      // plus_minus integer rule
      else if (quad->arg1->type == int_arg && quad->arg2 == NULL) {

        quad->arg1->reg_arg = find_free_reg();
        astack_push_text(stack, "li %s, %d", reg_name(quad->arg1->reg_arg),
                         quad->arg1->value.int_value);
      }

      // plus_minus '$' integer rule
      else if (quad->arg1->type == int_arg && quad->arg2 == ARG) {
      }

      //  plus_minus '$' '{' ID '}' rule
      else if (quad->arg1->type == id_arg && quad->arg2 == NULL) {
      }

      // plus_minus '$' '{' ID '[' op_int ']' '}' rule
      else if (quad->arg1->type == id_arg && quad->arg2 == ARG) {
      }

      break;

    case minus_op:

      // sum int rule
      if (quad->arg1->type == int_arg && quad->arg2->type == int_arg) {

        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, "sub %s, %s, %s", reg_name(quad->arg3->reg_arg),
                         reg_name(quad->arg1->reg_arg),
                         reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      // plus_minus integer rule
      else if (quad->arg1->type == int_arg && quad->arg2 == NULL) {

        quad->arg1->reg_arg = find_free_reg();
        astack_push_text(stack, "li %s, %d", reg_name(quad->arg1->reg_arg),
                         quad->arg1->value.int_value);
      }

      // plus_minus '$' integer rule
      else if (quad->arg1->type == int_arg && quad->arg2 == ARG) {
      }

      //  plus_minus '$' '{' ID '}' rule
      else if (quad->arg1->type == id_arg && quad->arg2 == NULL) {
      }

      // plus_minus '$' '{' ID '[' op_int ']' '}' rule
      else if (quad->arg1->type == id_arg && quad->arg2 == ARG) {
      }

      break;

    case mult_op:

      // prod int rule
      if (quad->arg1->type == int_arg && quad->arg2->type == int_arg) {

        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, "mul %s, %s, %s", reg_name(quad->arg3->reg_arg),
                         reg_name(quad->arg1->reg_arg),
                         reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case div_op:

      // prod int rule
      if (quad->arg1->type == int_arg && quad->arg2->type == int_arg) {

        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, "div %s, %s, %s", reg_name(quad->arg3->reg_arg),
                         reg_name(quad->arg1->reg_arg),
                         reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case mod_op:

      if (quad->arg1->type == int_arg && quad->arg2->type == int_arg) {

        astack_push_text(stack, "div %s, %s", reg_name(quad->arg1->reg_arg),
                         reg_name(quad->arg2->reg_arg));

        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, "mfhi %s", reg_name(quad->arg3->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case echo_instr_op:

      // fseek(out, 0, SEEK_SET);

      // str_count = 0;
      // while (fgets(substring, BUFSIZ, out) != NULL) {
      //   if ((strstr_string = strstr(substring, ".text")) != NULL)
      //   break; str_count++;
      // }

      // fseek(out, str_count * BUFSIZ + (size_t)(substring -
      // strstr_string) - 1,
      //       SEEK_SET);
      // astack_push_data(stack, "msg%d: .asciiz \"%s\"", msg_count,
      //                  quad->arg1->value.str_value);

      // fseek(out, 0, SEEK_END);

      if (quad->arg1->type == int_arg) {

        astack_push_text(stack, "la %s, %s", reg_name(reg_a0),
                         reg_name(quad->arg1->reg_arg));
        free_reg(quad->arg1->reg_arg);
        astack_push_text(stack, "li $v0, %d", sc_print_int);

      } else if (quad->arg1->type == str_arg) {

        astack_push_text(stack, "la %s, msg%d", reg_name(reg_a0), msg_count);
        astack_push_text(stack, "li $v0, %d", sc_print_string);
        msg_count++;
      }

      astack_push_text(stack, "syscall");

      break;

    case exit_void_op:

      astack_push_text(stack, "li $v0, %d", sc_exit);
      astack_push_text(stack, "syscall");

      break;

    case exit_int_op:

      astack_push_text(stack, "li $v0, %d", sc_exit);
      astack_push_text(stack, "move $a0, %s", reg_name(quad->arg1->reg_arg));
      astack_push_text(stack, "syscall");

      break;
    default:
      break;
    }
  }

  astack_fprintf(stack, out);
  astack_free(stack);
}
