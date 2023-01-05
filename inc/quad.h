#pragma once

#include "protocol.h"
#include "symtable.h"

enum quadop {
  // assignation operations
  assn_int_to_var_op,
  assn_float_to_var_op,
  assn_float_from_arraysub_op,
  assn_string_to_var_op,
  assn_arg_to_var_op,
  assn_id_to_var_op,
  assn_all_arg_to_var_op,
  assn_status_to_var_op,
  assn_expr_value_to_var_op,
  assn_elem_array_to_var_op,
  assn_array_to_var_ops,
  assn_cfun_to_var_op,
  // arithmetic operations
  plus_op,
  minus_op,
  mult_op,
  div_op,
  mod_op,
  lt_op,
  le_op,
  gt_op,
  ge_op,
  eq_op,
  neq_op,
  null_op,
  nnull_op,
  // logical operations
  not_op,
  and_op,
  or_op,
  test_op,
  // condition operations
  testing_op,
  elif_op,
  else_op,
  else_end_op,
  empty_op,
  filter_instr,
  cases_op,
  // instruction operations
  concat_op,
  assn_instr_op,
  array_instr_op,
  assn_array_instr_op,
  declare_array_instr_op,
  test_instr_op,
  maybe_else_instr_op,
  if_instr_op,
  in_instr_op,
  for_instr_op,
  while_instr_op,
  until_instr_op,
  case_instr_op,
  echo_instr_op,
  read_instr_op,
  read_array_instr_op,
  return_void_op,
  return_int_op,
  exit_void_op,
  exit_int_op,
  instr_op,
  // function operations
  cont_func_op,
  dfun_op,
  local_decl_op,
  decl_op,
  cfun_ops,
  cfun_op,
};

enum quadargtype {
  no_arg_type,
  int_arg,
  dbl_arg,
  str_arg,
  array_str_arg,
  int_array_arg,
  id_arg,
  quad_arg,
};

struct quadarg {
  union {
    int int_value;
    double dbl_value;
    char *str_value;
    char **array_str_value;
    struct symnode *id_value;
  } value;
  enum quadargtype type;
  enum reg reg_arg;
};

struct quad {
  enum quadop op;
  struct quadarg *arg1;
  struct quadarg *arg2;
  struct quadarg *arg3;

  int lineno;
  int index;
};

/**
 * @brief Initialize the quad vector & store it in quad_array global
 * variable.
 *
 * @param size the size of the quad vector (use `-1` for default size)
 * @note There can only be one quad array at a time since variables
 * are static and the array is global.
 */
void quad_vec_init(int size);

/**
 * @brief Create a new quad and add it to the quad vector.
 *
 */
struct quad *quad_new(int lineno, enum quadop, struct quadarg *,
                      struct quadarg *, struct quadarg *);

/**
 * @brief Take a global quad and assign the good operation within
 * quadarg types
 *
 * @param quad
 * @param op
 * @return struct quad*
 */
struct quad *quad_modify_op(struct quad *quad);

/**
 * @brief Retroactively change the arguments of a quad.
 *
 */
void quad_patch(struct quad *, int arg_index, struct quadarg *new_quadarg);

/**
 * @brief Create a new quadarg of type int, dbl, id or str
 *
 */
struct quadarg *quadarg_new_int(int value);
struct quadarg *quadarg_new_dbl(double value);
struct quadarg *quadarg_new_id(char *value);
struct quadarg *quadarg_new_str(char *value);
struct quadarg *quadarg_new_array_str(char **value);
struct quadarg *quadarg_new_reg(void);

/**
 * @brief Get a new temp (current function name as prefix)
 *
 */
struct quadarg *quadarg_new_tmp(struct symtable *symtab, enum vartype type);

// todo: how tf do we do this
// struct quadarg *quad_gencode(struct astnode *node);

/**
 * @brief Display the quad vector.
 *
 */
void quad_vec_display();

void quad_display(struct quad *quad);

void quadarg_display(struct quadarg *quadarg);

vec_t quadarray_new();

vec_t quadarray_add(vec_t quad_subarray, struct quad *quad);

vec_t quadarray_append(vec_t quad_array1, vec_t quad_array2);
