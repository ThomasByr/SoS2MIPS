#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "assstack.h"
#include "protocol.h"
#include "quad.h"
#include "symtable.h"

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

static const char reg_names[][6] = {
    "$zero", "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3",
    "$t0",   "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
    "$s0",   "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
    "$t8",   "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
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
    if (reg_use[i] == false) {
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
// void free_reg(enum reg reg) {
//   if (reg < reg_t0 || reg > reg_t7) {
//     panic("trying to free a non-temporary register");
//   }
//   if (!reg_use[reg]) {
//     panic("trying to free an already free register");
//   }
//   reg_use[reg] = false;
// }

#define free_reg(reg)                                                       \
  do {                                                                      \
    if (reg < reg_t0 || reg > reg_t7) {                                     \
      fprintf(stderr,                                                       \
              FG_RED                                                        \
              "trying to free a non-temporary register at line %d\n " RST,  \
              __LINE__);                                                    \
      abort();                                                              \
    }                                                                       \
    if (!reg_use[reg]) {                                                    \
      fprintf(stderr,                                                       \
              FG_RED                                                        \
              "trying to free an already free register at line :%d\n " RST, \
              __LINE__);                                                    \
      abort();                                                              \
    }                                                                       \
    reg_use[reg] = false;                                                   \
  } while (0)

/**
 * @brief Generate MIPS assembly code from the quad array
 *
 */
void generate_asm(FILE *out) {

  size_t i, j;
  int k;

  vec_t blocks = vec_from("main", NULL);
  astack_t stack = astack_new(2, 10); // random preallocation

  struct quad *quad;
  struct quadarg *quadarg1, *quadarg2;
  struct symnode *node;

  char *buf, *jmp_name_if, *jmp_name_else, *jmp_name_next, *jmp_name_previous;
  int msg_count = 0, msg_print = 0, jmp_count = 0, ops_count = 0, ops_print = 0,
      ops_size = 0, error_count = 1, fct_count = 0;

  enum reg reg1, reg2, reg3,
      reg_ops = reg_t8, // note : reg_t8 is used for ops in the case if all
                        // other reg are used
      reg_index_for = reg_s0; // note : reg_s"i" is used for index in for loop

  int index_alloc_count = 0;

#define asblock ((const char *)vec_last(blocks)) // current block
#define sbrk_size 0x100                          // 256 bytes

  astack_push_data(stack, "error_msg0: .asciiz \"\"");
  astack_push_data(stack, "msg_space: .asciiz \" \"");

  for (i = 0; i < vec_size(quad_array); i++) {
    quad = vec_get(quad_array, i);
    switch (quad->op) {

    case assn_int_to_var_op:

      quad->arg3->type = int_arg;
      quad->arg3->reg_arg = find_free_reg();
      astack_push_text(stack, asblock, "li %s, %d",
                       reg_name(quad->arg3->reg_arg),
                       quad->arg1->value.int_value);

      break;

    case assn_string_to_var_op:

      quad->arg3->type = str_arg;
      quad->arg3->reg_arg = find_free_reg();
      astack_push_data(stack, "msg%d: .asciiz \"%s\"", msg_count,
                       quad->arg1->value.str_value);

      msg_count++;

      break;

    case assn_id_to_var_op:

      if (quad->arg2 == NULL) {

        quad->arg1->value.id_value->var_type = inttype;
        quad->arg3->type = id_arg;
        quad->arg3->reg_arg = find_free_reg();
        node = quad->arg1->value.id_value;

        if (node->var_addr == 0) {
          panic("variable not declared");
        }

        astack_push_text(stack, asblock, "lw %s, %s",
                         reg_name(quad->arg3->reg_arg), node->name);
      } else {

        // check if the index is less than the size of the array
        reg1 = find_free_reg();
        buf = malloc(BUFSIZ);
        snprintf_s(buf, BUFSIZ, "%s_size", quad->arg1->value.id_value->name);
        astack_push_text(stack, asblock, "lw %s, %s", reg_name(reg1), buf);
        free(buf);
        astack_push_text(stack, asblock, "bge %s, %s, %s",
                         reg_name(quad->arg2->reg_arg), reg_name(reg1),
                         "_error");
        free_reg(reg1);
        free_reg(quad->arg2->reg_arg);

        // setup the error message
        astack_push_data(
            stack,
            "error_msg%d: .asciiz \"Index out of array bounds (sos:%d)\\n\"",
            error_count, quad->lineno);
        error_count++;

        // assign the value in the id given by quad->arg1 at the index given by
        // quad->arg2
        quad->arg1->value.id_value->var_type = arrayinttype;
        quad->arg3->type = int_array_arg;
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, asblock, "mul %s, %s, 4",
                         reg_name(quad->arg2->reg_arg),
                         reg_name(quad->arg2->reg_arg));
        astack_push_text(
            stack, asblock, "lw %s, %s(%s)", reg_name(quad->arg3->reg_arg),
            quad->arg1->value.id_value->name, reg_name(quad->arg2->reg_arg));
      }
      break;

    case assn_expr_value_to_var_op:

      quad->arg3->type = int_arg;
      quad->arg3->reg_arg = quad->arg1->reg_arg;

      break;

    case assn_arg_to_var_op:

      astack_push_text(stack, asblock, "beqz $t9, _error");
      astack_push_data(
          stack,
          "error_msg%d: .asciiz \"$i used outside of a function (sos:%d)\\n\"",
          error_count, quad->lineno);
      error_count++;

      // get the argument from the stack
      quad->arg3->type = int_arg;
      quad->arg3->reg_arg = find_free_reg();

      astack_push_text(stack, asblock, "lw %s, %d($sp)",
                       reg_name(quad->arg3->reg_arg),
                       (quad->arg1->value.int_value + 1) * 4);

      break;

    case assn_all_arg_to_var_op:

      astack_push_text(stack, asblock, "beqz $t9, _error");
      astack_push_data(
          stack,
          "error_msg%d: .asciiz \"$i used outside of a function (sos:%d)\\n\"",
          error_count, quad->lineno);
      error_count++;

      break;

    case assn_status_to_var_op:

      // get the return from a function store in $v1
      quad->arg3->type = int_arg;
      quad->arg3->reg_arg = find_free_reg();

      astack_push_text(stack, asblock, "move %s, $v1",
                       reg_name(quad->arg3->reg_arg));

      break;

    case assn_cfun_to_var_op:

      // get the return from a function store in $v1
      quad->arg3->type = int_arg;
      quad->arg3->reg_arg = find_free_reg();

      astack_push_text(stack, asblock, "move %s, $v1",
                       reg_name(quad->arg3->reg_arg));

      break;

    case plus_op:

      // sum int rule
      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(
            stack, asblock, "add %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      // plus_minus integer rule
      else if (quad->arg1->type == int_arg && quad->arg2 == NULL) {
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, asblock, "li %s, %d",
                         reg_name(quad->arg3->reg_arg),
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
      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(
            stack, asblock, "sub %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      // plus_minus integer rule
      else if (quad->arg1->type == int_arg && quad->arg2 == NULL) {
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, asblock, "li %s, %d",
                         reg_name(quad->arg3->reg_arg),
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
      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(
            stack, asblock, "mul %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case div_op:

      // prod int rule
      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(
            stack, asblock, "div %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case mod_op:

      // prod int rule
      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        astack_push_text(stack, asblock, "div %s, %s",
                         reg_name(quad->arg1->reg_arg),
                         reg_name(quad->arg2->reg_arg));

        quad->arg3->reg_arg = find_free_reg();
        astack_push_text(stack, asblock, "mfhi %s",
                         reg_name(quad->arg3->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case eq_op:

      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();

        astack_push_text(
            stack, asblock, "seq %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case neq_op:

      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();

        astack_push_text(
            stack, asblock, "sne %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case lt_op:

      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();

        astack_push_text(
            stack, asblock, "slt %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case gt_op:

      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();

        astack_push_text(
            stack, asblock, "sgt %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case le_op:

      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();

        astack_push_text(
            stack, asblock, "sle %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case ge_op:

      if (quad->arg1->type != str_arg && quad->arg2->type != str_arg) {
        quad->arg3->reg_arg = find_free_reg();

        astack_push_text(
            stack, asblock, "sge %s, %s, %s", reg_name(quad->arg3->reg_arg),
            reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

        free_reg(quad->arg1->reg_arg);
        free_reg(quad->arg2->reg_arg);
      }

      break;

    case not_op:

      quad->arg3->reg_arg = find_free_reg();
      astack_push_text(stack, asblock, "not %s, %s",
                       reg_name(quad->arg3->reg_arg),
                       reg_name(quad->arg1->reg_arg));

      free_reg(quad->arg1->reg_arg);

      break;

    case and_op:

      quad->arg3->reg_arg = find_free_reg();
      astack_push_text(
          stack, asblock, "and %s, %s, %s", reg_name(quad->arg3->reg_arg),
          reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

      free_reg(quad->arg1->reg_arg);
      free_reg(quad->arg2->reg_arg);

      break;

    case or_op:

      quad->arg3->reg_arg = find_free_reg();
      astack_push_text(
          stack, asblock, "or %s, %s, %s", reg_name(quad->arg3->reg_arg),
          reg_name(quad->arg1->reg_arg), reg_name(quad->arg2->reg_arg));

      free_reg(quad->arg1->reg_arg);
      free_reg(quad->arg2->reg_arg);

      break;

    case test_op:

      // block for if instruction
      jmp_name_if = malloc(100);
      snprintf_s(jmp_name_if, 100, "_instr%d", jmp_count);
      jmp_count++;

      // test if the result of test_instr is true
      astack_push_text(stack, asblock, "beq %s, 1, %s",
                       reg_name(quad->arg1->reg_arg), jmp_name_if);
      quad->arg3->reg_arg = quad->arg1->reg_arg;

      // block for else instruction
      jmp_name_else = malloc(100);
      snprintf_s(jmp_name_else, 100, "_instr%d", jmp_count);
      jmp_count++;

      astack_push_text(stack, asblock, "j %s", jmp_name_else);

      // block for the rest of the program
      jmp_name_next = malloc(100);
      snprintf_s(jmp_name_next, 100, "_instr%d", jmp_count);
      jmp_count++;

      vec_push(blocks, jmp_name_next);
      vec_push(blocks, jmp_name_else);
      vec_push(blocks, jmp_name_if);

      break;

    case elif_op:

      break;

    case else_op:

      jmp_name_next = vec_get(blocks, vec_size(blocks) - 3);
      astack_push_text(stack, asblock, "j %s", jmp_name_next);
      vec_pop(blocks);
      break;

    case if_instr_op:

      free_reg(quad->arg1->reg_arg);
      vec_pop(blocks);

      break;

    case ops_init_op:

      ops_count = 0;

      reg_ops = find_free_reg();

      // initialiaze ops array with sbrk of 256 bytes
      astack_push_text(stack, asblock, "li $v0, %d", sc_sbrk);
      astack_push_text(stack, asblock, "li $a0, %d", sbrk_size);
      astack_push_text(stack, asblock, "syscall");
      astack_push_text(stack, asblock, "move %s, $v0", reg_name(reg_ops));

      break;

    case ops_add_op:

      // add the value to the ops array
      if (quad->arg1 != ALL_ARG && quad->arg1->type != str_arg) {
        astack_push_text(stack, asblock, "sw %s, %d(%s)",
                         reg_name(quad->arg1->reg_arg), ops_count * 4,
                         reg_name(reg_ops));

        ops_count++;
      }

      if (quad->arg1 != ALL_ARG) free_reg(quad->arg1->reg_arg);

      break;

    case ops_array_op:

      // add all the array to the ops array
      reg1 = find_free_reg();
      reg2 = find_free_reg();

      for (j = 0; j < (size_t)quad->arg1->value.id_value->var_size; j++) {
        astack_push_text(stack, asblock, "la %s, %s", reg_name(reg2),
                         quad->arg1->value.id_value->name);

        astack_push_text(stack, asblock, "lw %s, %zu(%s)", reg_name(reg1),
                         j * 4, reg_name(reg2));

        if (reg1 == reg_ops) panic("reg1 == reg_ops");
        astack_push_text(stack, asblock, "sw %s, %d(%s)", reg_name(reg1),
                         ops_count * 4, reg_name(reg_ops));

        ops_count++;
      }

      free_reg(reg1);
      free_reg(reg2);

      break;

    case for_init_op:

      // declare the variable
      buf = malloc(BUFSIZ);
      snprintf_s(buf, BUFSIZ, "%s: .word 0", quad->arg1->value.id_value->name);
      astack_push_data(stack, buf);
      free(buf);

      // initialize the index
      astack_push_text(stack, asblock, "li %s, 0",
                       reg_name(reg_index_for + index_alloc_count));
      index_alloc_count++;

      quad->arg1->value.id_value->var_type = inttype;
      quad->arg1->value.id_value->var_addr = 1;

      break;

    case for_assn_op:

      // check if there is a string in the vector
      for (j = 0; j < vec_size(quad->subarray); j++) {
        quadarg1 = vec_get(quad->subarray, j);
        if (quadarg1 != ALL && quadarg1 != ALL_ARG &&
            quadarg1->type == str_arg) {
          j = 0;
          break;
        }
      }

      if (j == 0) {
        astack_push_text(stack, asblock, "j _error");
        astack_push_data(
            stack,
            "error_msg%d: .asciiz \"Do not put strings in for (sos:%d)\\n\"",
            error_count, quad->lineno);
        error_count++;
      }

      // block for each "for" instruction
      jmp_name_next = malloc(BUFSIZ);
      snprintf_s(jmp_name_next, BUFSIZ, "_for%d", index_alloc_count - 1);
      jmp_count++;

      vec_push(blocks, jmp_name_next);

      // assign the value of reg_ops first element to the variable
      quadarg1 = vec_get(quad->subarray, vec_size(quad->subarray) - 1);

      reg1 = find_free_reg();

      // get the current element
      astack_push_text(stack, asblock, "lw %s, 0(%s)", reg_name(reg1),
                       reg_name(reg_ops));

      // get the next element
      astack_push_text(stack, asblock, "addi %s, %s, 4", reg_name(reg_ops),
                       reg_name(reg_ops));

      astack_push_text(stack, asblock, "sw %s, %s", reg_name(reg1),
                       quadarg1->value.id_value->name);

      astack_push_text(stack, asblock, "addi %s, %s, 1",
                       reg_name(reg_index_for + index_alloc_count - 1),
                       reg_name(reg_index_for + index_alloc_count - 1));

      free_reg(reg1);

      break;

    case for_instr_op:

      ops_size = 0;

      for (j = 0; j < vec_size(quad->subarray) - 1; j++) {
        if ((quadarg1 = vec_get(quad->subarray, j)) == NULL)
          panic("quadarg1 is NULL");
        if (j != vec_size(quad->subarray) - 1) {
          if ((quadarg2 = vec_get(quad->subarray, j + 1)) == NULL)
            panic("quadarg2 is NULL");
        } else
          quadarg2 = NULL;

        if (quadarg1->type == int_arg ||
            (quadarg1->type == id_arg && quadarg2 != ALL)) {
          ops_size++;
        } else if (quadarg1->type == id_arg && quadarg2 == ALL) {
          ops_size += quadarg1->value.id_value->var_size;
          j++;
        }
      }

      // go back to the good "for" block
      buf = malloc(BUFSIZ);
      snprintf_s(buf, BUFSIZ, "_for%d", index_alloc_count - 1);

      astack_push_text(stack, asblock, "ble %s, %d, %s",
                       reg_name(reg_index_for + index_alloc_count - 1),
                       ops_size - 1, buf);
      index_alloc_count--;

      astack_push_text(stack, asblock, "\n_instr%d:", jmp_count);
      jmp_count++;

      break;

    case while_init_op:

      /// put the instruction name to define the while condition section
      jmp_name_next = malloc(BUFSIZ);
      snprintf_s(jmp_name_next, BUFSIZ, "_instr%d", jmp_count);
      jmp_count++;

      vec_push(blocks, jmp_name_next);

      break;

    case while_instr_op:

      // go back to the previous while condition
      jmp_name_next = vec_last(blocks);
      vec_pop(blocks);
      jmp_name_else = vec_last(blocks);
      vec_pop(blocks);
      jmp_name_if = vec_last(blocks);
      vec_pop(blocks);
      jmp_name_previous = vec_last(blocks);

      vec_push(blocks, jmp_name_if);
      vec_push(blocks, jmp_name_else);
      vec_push(blocks, jmp_name_next);

      astack_push_text(stack, asblock, "j %s", jmp_name_previous);

      vec_pop(blocks);

      break;

    case echo_instr_op:

      ops_print = 0;

      // make echo instruction according to the type of the arguments
      for (j = 0; j < vec_size(quad->subarray); j++) {

        if ((quadarg1 = vec_get(quad->subarray, j)) == NULL)
          panic("quadarg1 is NULL");
        if (j != vec_size(quad->subarray) - 1) {
          if ((quadarg2 = vec_get(quad->subarray, j + 1)) == NULL)
            panic("quadarg2 is NULL");
        } else
          quadarg2 = NULL;

        // int value
        if (quadarg1 != ALL_ARG && quadarg1 != ALL && quadarg2 != ALL &&
            (quadarg1->type == int_arg || quadarg1->type == id_arg ||
             quadarg1->type == int_array_arg)) {

          // print the value store in sbrk
          astack_push_text(stack, asblock, "lw $a0, %d(%s)", (ops_print)*4,
                           reg_name(reg_ops));
          astack_push_text(stack, asblock, "li $v0, %d", sc_print_int);
          astack_push_text(stack, asblock, "syscall");
          ops_print++;

          // display
          if (j != vec_size(quad->subarray) - 1) {

            astack_push_text(stack, asblock, "la $a0, msg_space");
            astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
            astack_push_text(stack, asblock, "syscall");
          }
          // string value
        } else if (quadarg1 != ALL_ARG && quadarg1 != ALL && quadarg2 != ALL &&
                   quadarg1->type == str_arg) {

          astack_push_text(stack, asblock, "la $a0, msg%d", msg_print);
          astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
          astack_push_text(stack, asblock, "syscall");
          msg_print++;

          // display
          if (j != vec_size(quad->subarray) - 1) {

            astack_push_text(stack, asblock, "la $a0, msg_space");
            astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
            astack_push_text(stack, asblock, "syscall");
          }
        }
        // int array
        else if (quadarg1 != ALL_ARG && quadarg1 != ALL && quadarg1 != ARG &&
                 (quadarg1->type == id_arg &&
                  quadarg1->value.id_value != NULL &&
                  quadarg1->value.id_value->var_type == arrayinttype &&
                  quadarg2 == ALL)) {

          for (k = ops_print; k < ops_count; k++) {

            // print each value of sbrk array
            astack_push_text(stack, asblock, "lw $a0, %d(%s)", k * 4,
                             reg_name(reg_ops));
            astack_push_text(stack, asblock, "li $v0, %d", sc_print_int);
            astack_push_text(stack, asblock, "syscall");

            if (ops_print != ops_count - 1) {

              astack_push_text(stack, asblock, "la $a0, msg_space");
              astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
              astack_push_text(stack, asblock, "syscall");
            }
          }

          ops_print = ops_count;
          j++;
        }
        // $* instruction
        else if (quadarg1 == ALL_ARG) {

          // skip the return address
          astack_push_text(stack, asblock, "addi $sp, $sp, 4");

          // load the number of arguments
          reg1 = find_free_reg();
          astack_push_text(stack, asblock, "lw %s, 0($sp)", reg_name(reg1));
          // skip the size element on the display
          astack_push_text(stack, asblock, "addi $sp, $sp, 4");

          // copy the arguments in the array of reg_ops
          reg2 = find_free_reg();
          astack_push_text(stack, asblock, "li %s, 0", reg_name(reg2));
          astack_push_text(stack, asblock, "\n_instr%d:", jmp_count);
          jmp_count++;

          // load the size + the argument in the array
          reg3 = find_free_reg();
          astack_push_text(stack, asblock, "lw $a0, 0($sp)", reg_name(reg3));
          astack_push_text(stack, asblock, "li $v0, %d", sc_print_int);
          astack_push_text(stack, asblock, "syscall");
          astack_push_text(stack, asblock, "la $a0, msg_space");
          astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
          astack_push_text(stack, asblock, "syscall");

          astack_push_text(stack, asblock, "addi $sp, $sp, 4");

          // increment the counter
          astack_push_text(stack, asblock, "addi %s, %s, 1", reg_name(reg2),
                           reg_name(reg2));

          // check if the counter is less than the number of arguments
          astack_push_text(stack, asblock, "blt %s, %s, _instr%d",
                           reg_name(reg2), reg_name(reg1), jmp_count - 1);

          // clear the stack with the number of arguments
          astack_push_text(stack, asblock, "li %s, 0", reg_name(reg2));
          astack_push_text(stack, asblock, "\n_instr%d:", jmp_count);
          jmp_count++;
          astack_push_text(stack, asblock, "addi $sp, $sp, -4");
          astack_push_text(stack, asblock, "addi %s, %s, 1", reg_name(reg2),
                           reg_name(reg2));
          astack_push_text(stack, asblock, "ble %s, %s, _instr%d",
                           reg_name(reg2), reg_name(reg1), jmp_count - 1);

          // get back the return address
          astack_push_text(stack, asblock, "addi $sp, $sp, -4");

          astack_push_text(stack, asblock, "\n_instr%d:", jmp_count);
          jmp_count++;

          // free the registers
          free_reg(reg1);
          free_reg(reg2);
          free_reg(reg3);
        }
      }

      free_reg(reg_ops);

      break;

    case read_instr_op:

      // id value
      node = quad->arg1->value.id_value;

      if (node->var_type != inttype) {
        panic("Cannot read into non-int variable");
      }

      if (node->var_addr == 0) {
        astack_push_data(stack, "%s: .word 0", node->name);
        node->var_addr = 1;
      }

      // get the value from the user
      astack_push_text(stack, asblock, "li $v0, %d", sc_read_int);
      astack_push_text(stack, asblock, "syscall");

      astack_push_text(stack, asblock, "sw $v0, %s", node->name);

      break;

    case read_array_instr_op:

      // id value
      node = quad->arg1->value.id_value;

      if (node->var_type != arrayinttype) {
        panic("Cannot read into non-int variable");
      }

      if (node->var_addr == 0) {
        panic("Array %s not declared", node->name);
      }

      // get the value from the user
      astack_push_text(stack, asblock, "li $v0, %d", sc_read_int);
      astack_push_text(stack, asblock, "syscall");

      astack_push_text(stack, asblock, "mul %s, %s, %d",
                       reg_name(quad->arg2->reg_arg),
                       reg_name(quad->arg2->reg_arg), 4);
      astack_push_text(stack, asblock, "sw $v0, %s(%s)", node->name,
                       reg_name(quad->arg2->reg_arg));

      free_reg(quad->arg2->reg_arg);

      break;

    case assn_instr_op:

      // declare id value
      node = quad->arg1->value.id_value;
      if (node->var_addr == 0) {
        astack_push_data(stack, "%s: .word 0", node->name);
        node->var_type = inttype;
        node->var_addr = 1;
      }

      // assign value
      astack_push_text(stack, asblock, "sw %s, %s",
                       reg_name(quad->arg2->reg_arg), node->name);
      free_reg(quad->arg2->reg_arg);

      break;

    case declare_array_instr_op:

      // write the array in the data section
      buf = malloc(BUFSIZ);
      snprintf_s(buf, BUFSIZ, "%s: .word ", quad->arg1->value.id_value->name);

      for (int j = 0; j < quad->arg2->value.int_value; j++) {
        buf[strlen(buf)] = '0';
        if (j != quad->arg2->value.int_value - 1) {
          buf[strlen(buf)] = ',';
          buf[strlen(buf)] = ' ';
        }
      }
      buf[strlen(buf)] = '\0';

      // store array informations in the symbol table
      quad->arg1->value.id_value->var_type = arrayinttype;
      quad->arg1->value.id_value->var_size = quad->arg2->value.int_value;
      quad->arg1->value.id_value->var_addr = 1;

      astack_push_data(stack, buf);

      // write the size in the data section
      snprintf_s(buf, BUFSIZ, "%s_size: .word %d",
                 quad->arg1->value.id_value->name, quad->arg2->value.int_value);

      astack_push_data(stack, buf);

      free(buf);

      break;

    case assn_array_instr_op:

      // id value
      node = quad->arg1->value.id_value;

      if (node->var_type != arrayinttype) {
        panic("Cannot assign to non-array variable");
      }

      if (node->var_addr == 0) {
        panic("Array %s not declared", node->name);
      }

      // check if the index is less than the size of the array
      reg1 = find_free_reg();
      buf = malloc(BUFSIZ);
      snprintf_s(buf, BUFSIZ, "%s_size", node->name);
      astack_push_text(stack, asblock, "lw %s, %s", reg_name(reg1), buf);
      free(buf);
      astack_push_text(stack, asblock, "bge %s, %s, %s",
                       reg_name(quad->arg2->reg_arg), reg_name(reg1), "_error");
      free_reg(reg1);

      // setup the error message
      astack_push_data(
          stack,
          "error_msg%d: .asciiz \"Index out of array bounds (sos:%d)\\n\"",
          error_count, quad->lineno);
      error_count++;

      // assign value
      astack_push_text(stack, asblock, "mul %s, %s, %d",
                       reg_name(quad->arg2->reg_arg),
                       reg_name(quad->arg2->reg_arg), 4);
      astack_push_text(
          stack, asblock, "sw %s, %s(%s)", reg_name(quad->arg3->reg_arg),
          quad->arg1->value.id_value->name, reg_name(quad->arg2->reg_arg));

      free_reg(quad->arg2->reg_arg);
      free_reg(quad->arg3->reg_arg);

      break;

    case dfun_init_op:

      // go to the next instruction that represents the left part of the
      // program
      astack_push_text(stack, asblock, "j end_fct%d", fct_count);

      // write the function in the text section
      astack_push_text(stack, asblock, "\n.globl %s",
                       quad->arg1->value.id_value->name);
      astack_push_text(stack, asblock, "%s:", quad->arg1->value.id_value->name);

      // write the prologue
      astack_push_text(stack, asblock, "sw $ra, 0($sp)");

      break;

    case dfun_op:

      // write the epilogue
      astack_push_text(stack, asblock, "lw $ra, 0($sp)");
      astack_push_text(stack, asblock, "jr $ra");

      // write the left part of the program in a new instruction
      astack_push_text(stack, asblock, "\nend_fct%d:", fct_count);
      fct_count++;

      break;

    case decl_op:

      break;

    case return_void_op:

      astack_push_text(stack, asblock, "beqz $t9, _error");
      astack_push_data(stack,
                       "error_msg%d: .asciiz \"$i used outside of a function "
                       "(sos:%d)\\n\"",
                       error_count, quad->lineno);
      error_count++;

      // set $v1 to 0
      astack_push_text(stack, asblock, "li $v1, 0");

      break;

    case return_int_op:

      astack_push_text(stack, asblock, "beqz $t9, _error");
      astack_push_data(stack,
                       "error_msg%d: .asciiz \"$i used outside of a function "
                       "(sos:%d)\\n\"",
                       error_count, quad->lineno);
      error_count++;

      // set $v1 to the return value
      astack_push_text(stack, asblock, "move $v1, %s",
                       reg_name(quad->arg1->reg_arg));

      break;

    case cfun_ops:

      // store the number of arguments in the stack
      reg1 = find_free_reg();
      astack_push_text(stack, asblock, "li %s, %zu", reg_name(reg1),
                       vec_size(quad->subarray) - 1);
      astack_push_text(stack, asblock, "sw %s, 4($sp)", reg_name(reg1));
      // load the arguments in the stack
      for (j = 0; j < vec_size(quad->subarray); j++) {
        astack_push_text(stack, asblock, "lw %s, %zu(%s)", reg_name(reg1),
                         j * 4, reg_name(reg_ops));
        astack_push_text(stack, asblock, "sw %s, %zu($sp)", reg_name(reg1),
                         (j + 2) * 4);
      }
      free_reg(reg1);

      // set the $t9 to true to know that we are in a function
      astack_push_text(stack, asblock, "li $t9, 1");

      // jump to the function code
      astack_push_text(stack, asblock, "jal %s",
                       ((struct quadarg *)vec_get(quad->subarray,
                                                  vec_size(quad->subarray) - 1))
                           ->value.id_value->name);

      // set the flag to false to know that we are not in a function
      astack_push_text(stack, asblock, "li $t9, 0");

      break;

    case cfun_op:

      // store the number of arguments in the stack
      reg1 = find_free_reg();
      astack_push_text(stack, asblock, "li %s, %d", reg_name(reg1), 0);
      astack_push_text(stack, asblock, "sw %s, 4($sp)", reg_name(reg1));
      free_reg(reg1);

      // set the flag to false to know that we are in a function
      astack_push_text(stack, asblock, "li $t9, 1");

      // jump to the function code
      astack_push_text(stack, asblock, "jal %s",
                       quad->arg1->value.id_value->name);

      // set the flag to false to know that we are not in a function
      astack_push_text(stack, asblock, "li $t9, 0");

      break;

    case exit_void_op:

      astack_push_text(stack, asblock, "j _exit");

      break;

    case exit_int_op:

      astack_push_text(stack, asblock, "move $a0, %s",
                       reg_name(quad->arg1->reg_arg));
      astack_push_text(stack, asblock, "j _exit2");

      break;

    default:
      break;
    }
  }

  astack_push_text(stack, asblock, "\n# End of program");

  // default exit
  buf = malloc(BUFSIZ);
  snprintf_s(buf, BUFSIZ, "_exit");
  vec_push(blocks, buf);
  astack_push_text(stack, asblock, "li $v0, %d", sc_exit);
  astack_push_text(stack, asblock, "syscall");

  // declare exit2
  buf = malloc(BUFSIZ);
  snprintf_s(buf, BUFSIZ, "_exit2");
  vec_push(blocks, buf);
  astack_push_text(stack, asblock, "li $v0, %d", sc_exit2);
  astack_push_text(stack, asblock, "syscall");

  // error section
  buf = malloc(BUFSIZ);
  snprintf_s(buf, BUFSIZ, "_error");
  vec_push(blocks, buf);
  astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
  // fix the print of the last error message
  astack_push_text(stack, asblock, "la $a0, error_msg%d", error_count - 1);
  astack_push_text(stack, asblock, "syscall");
  astack_push_text(stack, asblock, "li $v0, %d", sc_exit);
  astack_push_text(stack, asblock, "syscall");

  astack_fprintf(stack, out);
  astack_free(stack);
}
