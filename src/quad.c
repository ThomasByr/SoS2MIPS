#include "protocol.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "quad.h"
#include "vec.h"

#define DEFAULT_QUAD_ARRAY_SIZE 1 << 12

int quad_array_index, quad_array_size;
vec_t quad_array;

// number of global temp variables
int global_temp_count = 0;

struct symnode *curr_func_symnode_quad;

// number of global variables, from preprocessing
extern int num_global_vars;
extern struct symtable *flat_id_table;
extern struct symtable *stringconst_table;
extern struct symtable *id_name_table;

int temp_count = 0;

// symnode of the main function (declared in symtab.c)
extern struct symnode *main_func_symnode;

char *quad_op_string[] = {
    // assignation operations
    "assn_int_to_var_op",
    "assn_float_to_var_op",
    "assn_float_from_arraysub_op",
    "assn_string_to_var_op",
    "assn_arg_to_var_op",
    "assn_id_to_var_op",
    "assn_all_arg_to_var_op",
    "assn_status_to_var_op",
    "assn_expr_value_to_var_op",
    "assn_elem_array_to_var_op",
    "assn_array_to_var_ops",
    "assn_cfun_to_var_op",
    // arithmetic operations
    "plus_op",
    "minus_op",
    "mult_op",
    "div_op",
    "mod_op",
    "lt_op",
    "le_op",
    "gt_op",
    "ge_op",
    "eq_op",
    "neq_op",
    "null_op",
    "nnull_op",
    // logical operations
    "not_op",
    "and_op",
    "or_op",
    "test_op",
    "test_while_op",
    // condition operations
    "testing_op",
    "elif_op",
    "else_op",
    "else_end_op",
    "empty_op",
    "filter_instr",
    "cases_op",
    // instruction operations
    "concat_op",
    "ops_init_op",
    "ops_add_op",
    "ops_first_op",
    "ops_array_op",
    "assn_instr_op",
    "assn_array_instr_op",
    "declare_array_instr_op",
    "test_instr_op",
    "maybe_else_instr_op",
    "if_instr_op",
    "in_instr_op",
    "for_init_op",
    "for_assn_op",
    "for_instr_op",
    "while_init_op",
    "while_instr_op",
    "until_instr_op",
    "case_instr_op",
    "echo_instr_op",
    "read_instr_op",
    "read_array_instr_op",
    "return_void_op",
    "return_int_op",
    "exit_void_op",
    "exit_int_op",
    "instr_op",
    // function operations
    "cont_func_op",
    "dfun_op",
    "local_decl_op",
    "decl_op",
    "cfun_ops",
    "cfun_op",
};

/* variables */

/* functions */

void quad_vec_display() {
  struct quad *quad;
  vec_foreach(quad_array, i, quad) {
    printf("%zu\t", i);
    quad_display(quad);
  }
}

void quad_display(struct quad *quad) {
  printf("(%s, ", quad_op_string[quad->op]);
  quadarg_display(quad->arg1);
  printf(", ");
  quadarg_display(quad->arg2);
  printf(", ");
  quadarg_display(quad->arg3);
  printf(") (%d)\n", quad->lineno);
}

void quadarg_display(struct quadarg *quadarg) {
  if (!quadarg) {
    printf("null");
    return;
  }

  if (quadarg == ALL || quadarg == ARG) {
    printf("null");
    return;
  }

  switch (quadarg->type) {
  case int_arg:
    printf("int: %d", quadarg->value.int_value);
    break;
  case dbl_arg:
    printf("dbl: %f", quadarg->value.dbl_value);
    break;
  case id_arg:
    printf("id: %s", quadarg->value.id_value->name);
    break;
  case str_arg:
    printf("str: %s", quadarg->value.str_value);
    break;
  case no_arg_type:
    if (quadarg->reg_arg == 0)
      printf("reg");
    else
      printf("reg: %s", reg_name(quadarg->reg_arg));
    break;
  default:
    printf("unknown");
    break;
  }
}

void quad_vec_init(int size) {
  if (size <= 0) size = DEFAULT_QUAD_ARRAY_SIZE;
  quad_array = vec_new(size);
}

struct quad *quad_new_from_quadarg(int lineno, enum quadop op,
                                   struct quadarg *arg1, struct quadarg *arg2,
                                   struct quadarg *arg3) {

  struct quad *quad = malloc(sizeof(struct quad));
  quad->type = quadarg;
  quad->op = op;
  quad->arg1 = arg1;
  quad->arg2 = arg2;
  quad->arg3 = arg3;
  quad->lineno = lineno;

  quad->index = quad_array_index;
  quad_array = quadarray_add(quad_array, quad);

  return quad;
}

struct quad *quad_new_from_vec(int lineno, enum quadop op,
                               vec_t quadarg_array) {

  struct quad *quad = malloc(sizeof(struct quad));
  quad->type = quadarg;
  quad->op = op;
  quad->subarray = quadarg_array;
  quad->lineno = lineno;

  quad->index = quad_array_index;
  quad_array = quadarray_add(quad_array, quad);

  return quad;
}

void quad_patch(struct quad *q, int arg_index, struct quadarg *new_quadarg) {
  switch (arg_index) {
  case 1:
    q->arg1 = new_quadarg;
    break;
  case 2:
    q->arg2 = new_quadarg;
    break;
  case 3:
    q->arg3 = new_quadarg;
    break;
  default:
    alert("quad_patch: invalid arg_index");
  }
}

vec_t quadarray_new() {

  quad_array_index = 0;
  return vec_new();
}

vec_t quadarray_add(vec_t quad_subarray, struct quad *quad) {

  quad_array_index++;
  vec_push(quad_subarray, quad);
  return quad_subarray;
}

vec_t quadarray_append(vec_t quad_array1, vec_t quad_array2) {
  return vec_append(quad_array1, quad_array2);
}

struct quadarg *quadarg_new_int(int value) {
  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = int_arg;
  quadarg->value.int_value = value;
  return quadarg;
}

struct quadarg *quadarg_new_dbl(double value) {
  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = dbl_arg;
  quadarg->value.dbl_value = value;
  return quadarg;
}

struct quadarg *quadarg_new_str(char *value) {
  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = str_arg;
  quadarg->value.str_value = strdup(value);
  return quadarg;
}

struct quadarg *quadarg_new_id(char *value) {
  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = id_arg;
  quadarg->value.id_value = symtable_insert(id_name_table, value);
  quadarg->value.id_value->node_type = val_node;
  quadarg->value.id_value->var_type = inttype;
  quadarg->value.id_value->mem_addr_type = off_fp;
  quadarg->value.id_value->var_addr = 0;
  return quadarg;
}

struct quadarg *quadarg_new_array_str(char **value) {
  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = array_str_arg;
  quadarg->value.array_str_value = value;
  return quadarg;
}

struct quadarg *quadarg_new_reg(void) {
  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = no_arg_type;
  return quadarg;
}

struct quadarg *quadarg_new_tmp(struct symtable *symtab, enum vartype type) {
  int digits_counter_num = temp_count / 10;
  int num_digits = 1;
  while (digits_counter_num != 0) {
    num_digits++;
    digits_counter_num /= 10;
  }

  int tempname_len = num_digits + 4;
  char *tempname = calloc(tempname_len + 1, sizeof(char));
  snprintf_s(tempname, tempname_len + 1, "temp%d", temp_count);

  // symbol table node
  struct symnode *temp_symnode = symtable_insert(symtab, tempname);
  temp_symnode->var_type = type;
  temp_symnode->node_type = val_node;

  // set memory address + increment temp_count
  // todo: fix this shit to work with global-by-default
  if (curr_func_symnode_quad != NULL) {
    temp_symnode->mem_addr_type = off_fp;
    curr_func_symnode_quad->num_temps++;
    temp_symnode->var_addr = -8 * (curr_func_symnode_quad->num_vars +
                                   curr_func_symnode_quad->num_temps);
  } else {
    temp_symnode->mem_addr_type = global;
    global_temp_count++;
    temp_symnode->var_addr = -8 * global_temp_count;
  }

  struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
  quadarg->type = id_arg;
  quadarg->value.id_value = temp_symnode;

  temp_count++;
  return quadarg;
}

vec_t quadarg_array_new() { return vec_new(); }

void quadarg_array_add(vec_t quadarg_array, struct quadarg *quadarg) {
  vec_push(quadarg_array, quadarg);
}

struct quadarg *quadarg_array_get(vec_t quadarg_array, int index) {
  return vec_get(quadarg_array, index);
}