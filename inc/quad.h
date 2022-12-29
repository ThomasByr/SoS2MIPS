#pragma once

#include "symtable.h"

enum global_quadop {
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
};

enum quadop {

  // assignation operations
  assn_int_to_var_op,
  assn_float_to_var_op,
  assn_float_from_arraysub_op,
  assn_string_to_var_op,
  assn_arg_to_var_op,
  assn_all_arg_to_var_op,
  assn_status_to_var_op,
  assn_expr_value_to_var_op,
  assn_elem_array_to_var_op,
  assn_array_to_var_ops,
  assn_cfun_to_var_op,
  // arithmetic operations
  add_ints_op,
  add_floats_op,
  sub_ints_op,
  sub_floats_op,
  mult_ints_op,
  mult_floats_op,
  div_ints_op,
  div_floats_op,
  mod_ints_op,
  lt_ints_op,
  lt_floats_op,
  lt_vec_op,
  leq_ints_op,
  leq_floats_op,
  leq_vec_op,
  gt_ints_op,
  gt_floats_op,
  gt_vec_op,
  geq_ints_op,
  geq_floats_op,
  geq_vec_op,
  eq_ints_op,
  eq_floats_op,
  eq_vec_op,
  neq_ints_op,
  neq_floats_op,
  neq_vec_op,
  // logical operations
  not_op,
  and_op,
  or_op,
  test_op,
  // condition operations
  testing_op,
  elif_op,
  else_op,
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
    struct quad *quad_value;
  } value;
  enum quadargtype type;
};

struct quad {
  enum quadop op;
  struct quadarg *arg1;
  struct quadarg *arg2;
  struct quadarg *arg3;

  int lineno;
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
