#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#include "assstack.h"
#include "protocol.h"
#include "quad.h"
#include "symtable.h"
#include "vec.h"

#define ARG (void *)0x42
#define ALL (void *)0x43

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
  vec_t blocks = vec_from("main", NULL);
  struct quad *quad;
  int msg_count = 0;
  astack_t stack = astack_new(2, 10); // random preallocation
  char *buf;
  struct symnode *node;
  unsigned int jmp_count = 0;
  char *jmp_name_if, *jmp_name_else, *jmp_name_next;
  enum reg reg1, reg2, reg3;

#define asblock ((const char *)vec_last(blocks)) // current block

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

    case assn_arg_to_var_op:
      break;

    case assn_string_to_var_op:

      quad->arg3->type = str_arg;
      quad->arg3->reg_arg = find_free_reg();
      astack_push_data(stack, "msg%d: .asciiz \"%s\"", msg_count,
                       quad->arg1->value.str_value);

      break;

    case assn_id_to_var_op:

      if (quad->arg2 == NULL) {
        // assign type int to id
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

        // TODO: check in C if the index is less than the size of the array

        // assign type int to id
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

        quad->arg1->reg_arg = find_free_reg();
        astack_push_text(stack, asblock, "li %s, %d",
                         reg_name(quad->arg1->reg_arg),
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

        quad->arg1->reg_arg = find_free_reg();
        astack_push_text(stack, asblock, "li %s, %d",
                         reg_name(quad->arg1->reg_arg),
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

      quad->arg2->reg_arg = find_free_reg();
      astack_push_text(stack, asblock, "not %s, %s",
                       reg_name(quad->arg2->reg_arg),
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

      jmp_name_if = malloc(100);
      snprintf(jmp_name_if, 100, "instr%d", jmp_count);
      jmp_count++;

      astack_push_text(stack, asblock, "beq %s, 1, %s",
                       reg_name(quad->arg1->reg_arg), jmp_name_if);
      quad->arg3->reg_arg = quad->arg1->reg_arg;

      jmp_name_else = malloc(100);
      snprintf(jmp_name_else, 100, "instr%d", jmp_count);
      jmp_count++;

      astack_push_text(stack, asblock, "j %s", jmp_name_else);

      jmp_name_next = malloc(100);
      snprintf(jmp_name_next, 100, "instr%d", jmp_count);
      jmp_count++;

      vec_push(blocks, jmp_name_next);
      vec_push(blocks, jmp_name_else);
      vec_push(blocks, jmp_name_if);

      break;

    case elif_op:

      break;

    case else_op:

      vec_pop(blocks);
      break;

    case else_end_op:

      jmp_name_else = vec_last(blocks);
      vec_pop(blocks);
      jmp_name_next = vec_last(blocks);
      vec_push(blocks, jmp_name_else);
      astack_push_text(stack, asblock, "j %s", jmp_name_next);

      break;

    case if_instr_op:

      vec_pop(blocks);
      break;

    case while_instr_op:

      astack_push_text(stack, asblock, "test");
      break;

    case echo_instr_op:

      if (quad->arg1->type == int_arg) {
        astack_push_text(stack, asblock, "move %s, %s", reg_name(reg_a0),
                         reg_name(quad->arg1->reg_arg));
        astack_push_text(stack, asblock, "li $v0, %d", sc_print_int);
        free_reg(quad->arg1->reg_arg);

        astack_push_text(stack, asblock, "syscall");

      } else if (quad->arg1->type == str_arg) {
        astack_push_text(stack, asblock, "la %s, msg%d", reg_name(reg_a0),
                         msg_count);
        astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
        msg_count++;

        astack_push_text(stack, asblock, "syscall");
      }
      // only print one value
      else if ((quad->arg1->type == id_arg ||
                quad->arg1->type == int_array_arg) &&
               quad->arg2 != ALL) {
        astack_push_text(stack, asblock, "move %s, %s", reg_name(reg_a0),
                         reg_name(quad->arg1->reg_arg));
        astack_push_text(stack, asblock, "li $v0, %d", sc_print_int);
        free_reg(quad->arg1->reg_arg);

        astack_push_text(stack, asblock, "syscall");
      }
      // print all the array
      else if (quad->arg2 == ALL) {
        reg1 = find_free_reg();
        reg2 = find_free_reg();
        reg3 = find_free_reg();

        // load the array address
        astack_push_text(stack, asblock, "la %s, %s", reg_name(reg1),
                         quad->arg3->value.id_value->name);

        buf = malloc(BUFSIZ);
        snprintf_s(buf, BUFSIZ, "%s_size", quad->arg3->value.id_value->name);

        // load the size of the array
        astack_push_text(stack, asblock, "lw %s, %s", reg_name(reg2), buf);

        // load the index
        astack_push_text(stack, asblock, "li %s, 0", reg_name(reg3));

        // the loop
        jmp_name_next = malloc(100);
        snprintf(jmp_name_next, 100, "instr%d", jmp_count);
        jmp_count++;

        jmp_name_else = malloc(100);
        snprintf(jmp_name_else, 100, "instr%d", jmp_count);
        jmp_count++;

        vec_push(blocks, jmp_name_else);

        astack_push_text(stack, asblock, "bge %s, %s, %s", reg_name(reg3),
                         reg_name(reg2), jmp_name_next);

        // load the value
        astack_push_text(stack, asblock, "lw %s, 0(%s)", reg_name(reg_a0),
                         reg_name(reg1));
        astack_push_text(stack, asblock, "li $v0, %d", sc_print_int);

        astack_push_text(stack, asblock, "syscall");

        // add 4 to the address
        astack_push_text(stack, asblock, "addi %s, %s, 4", reg_name(reg1),
                         reg_name(reg1));

        // add 1 to the index
        astack_push_text(stack, asblock, "addi %s, %s, 1", reg_name(reg3),
                         reg_name(reg3));

        // avoid print ", " on the last element
        astack_push_text(stack, asblock, "beq %s, %s, %s", reg_name(reg3),
                         reg_name(reg2), jmp_name_else);

        // print ", "
        astack_push_data(stack, "msg%d: .asciiz \", \"", msg_count);
        astack_push_text(stack, asblock, "la %s, msg%d", reg_name(reg_a0),
                         msg_count);
        msg_count++;
        astack_push_text(stack, asblock, "li $v0, %d", sc_print_string);
        astack_push_text(stack, asblock, "syscall");

        astack_push_text(stack, asblock, "j %s", jmp_name_else);

        vec_pop(blocks);
        vec_push(blocks, jmp_name_next);
      }

      break;

    case read_instr_op:

      node = quad->arg1->value.id_value;

      if (node->var_type != inttype) {
        panic("Cannot read into non-int variable");
      }

      if (node->var_addr == 0) {
        astack_push_data(stack, "%s: .word 0", node->name);
        node->var_addr = 1;
      }

      astack_push_text(stack, asblock, "li $v0, %d", sc_read_int);
      astack_push_text(stack, asblock, "syscall");

      astack_push_text(stack, asblock, "sw $v0, %s", node->name);

      break;

    case read_array_instr_op:

      node = quad->arg1->value.id_value;

      if (node->var_type != arrayinttype) {
        panic("Cannot read into non-int variable");
      }

      if (node->var_addr == 0) {
        panic("Array %s not declared", node->name);
      }

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

      node = quad->arg1->value.id_value;
      if (node->var_addr == 0) {
        astack_push_data(stack, "%s: .word 0", node->name);
        node->var_type = inttype;
        node->var_addr = 1;
      }

      astack_push_text(stack, asblock, "sw %s, %s",
                       reg_name(quad->arg2->reg_arg), node->name);
      free_reg(quad->arg2->reg_arg);

      break;

    case declare_array_instr_op:

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

      quad->arg1->value.id_value->var_type = arrayinttype;
      quad->arg1->value.id_value->var_addr = 1;

      astack_push_data(stack, buf);

      snprintf_s(buf, BUFSIZ, "%s_size: .word %d",
                 quad->arg1->value.id_value->name, quad->arg2->value.int_value);

      astack_push_data(stack, buf);

      free(buf);

      break;

    case assn_array_instr_op:

      node = quad->arg1->value.id_value;

      if (node->var_type != arrayinttype) {
        panic("Cannot assign to non-array variable");
      }

      if (node->var_addr == 0) {
        panic("Array %s not declared", node->name);
      }

      astack_push_text(stack, asblock, "mul %s, %s, %d",
                       reg_name(quad->arg2->reg_arg),
                       reg_name(quad->arg2->reg_arg), 4);
      astack_push_text(
          stack, asblock, "sw %s, %s(%s)", reg_name(quad->arg3->reg_arg),
          quad->arg1->value.id_value->name, reg_name(quad->arg2->reg_arg));

      free_reg(quad->arg1->reg_arg);
      free_reg(quad->arg2->reg_arg);
      free_reg(quad->arg3->reg_arg);

      break;

    case exit_void_op:

      astack_push_text(stack, asblock, "li $v0, %d", sc_exit);
      astack_push_text(stack, asblock, "syscall");

      break;

    case exit_int_op:

      astack_push_text(stack, asblock, "li $v0, %d", sc_exit);
      astack_push_text(stack, asblock, "move $a0, %s",
                       reg_name(quad->arg1->reg_arg));
      astack_push_text(stack, asblock, "syscall");

      break;

    default:
      break;
    }
  }

  astack_fprintf(stack, out);
  astack_fprintf(stack, stdout);
  astack_free(stack);
}
