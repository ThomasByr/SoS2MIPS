#pragma once

#include "symtable.h"

enum quadop {
  call_func_op,
  print_int_op,
  print_float_op,
  print_string_op,
  int_to_float_op,
  assn_int_to_var_op,
  assn_int_to_arraysub_op,
  assn_int_from_arraysub_op,
  assn_float_to_var_op,
  assn_float_to_arraysub_op,
  assn_float_from_arraysub_op,
  add_ints_op,
  add_floats_op,
  sub_ints_op,
  sub_floats_op,
  mult_ints_op,
  mult_floats_op,
  div_ints_op,
  div_floats_op,
  mod_op,
  lt_ints_op,
  lt_floats_op,
  leq_ints_op,
  leq_floats_op,
  gt_ints_op,
  gt_floats_op,
  geq_ints_op,
  geq_floats_op,
  eq_ints_op,
  eq_floats_op,
  neq_ints_op,
  neq_floats_op,
  int_neg_op,
  float_neg_op,
  bang_op,
  var_inc_op,
  array_inc_op,
  var_dec_op,
  array_dec_op,
  if_false_op,
  if_true_op,
  goto_op,
  read_int_op,
  read_double_op,
  func_decl_op,
  push_param_op,
  pop_params_op,
  alloc_array_op,
  return_op,
  assign_int_literal,
  assign_double_literal,
  initial_main_call,
  store_string_op,
};

enum quadargtype {
  no_arg_type,
  int_arg,
  dbl_arg,
  id_arg,
  str_arg,
};

struct quadarg {
  union {
    int int_value;
    double dbl_value;
    struct symnode *varnode;
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
 * @brief Initialize the quad vector & store it in quad_array global variable.
 *
 * @param size the size of the quad vector (use `-1` for default size)
 * @note There can only be one quad array at a time since variables are static
 * and the array is global.
 */
void quad_vec_init(int size);

/**
 * @brief Create a new quad and add it to the quad vector.
 *
 */
struct quad *quad_new(int lineno, enum quadop, struct quadarg *,
                      struct quadarg *, struct quadarg *);

/**
 * @brief Retroactively change the arguments of a quad.
 *
 */
void quad_patch(struct quad *, int arg_index, struct quadarg *new_quadarg);

/**
 * @brief Create a new quad argument.
 * Value field must be set manually.
 *
 */
struct quadarg *quadarg_new(enum quadargtype type);

/**
 * @brief Get a new temp (current function name as prefix)
 *
 */
struct quadarg *quadarg_new_tmp(struct symtable *symtab, enum vartype type);

// todo: how tf do we do this
// struct quadarg *quadarg_gencode(struct astnode *node);

/**
 * @brief Display the quad vector.
 *
 */
void quad_vec_display();

void quad_display(struct quad *quad);

void quadarg_display(struct quadarg *quadarg);

void quad_add(struct quad *quad);

vec_t quad_append(vec_t quad_array1, vec_t quad_array2);
