#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processing.h"
#include "quad.h"
#include "vec.h"

#define DEFAULT_QUAD_ARRAY_SIZE 1 << 12

int next_quad_index, quad_array_size;
vec_t quad_array;

// number of global temp variables
int global_temp_count = 0;

struct symnode *curr_func_symnode_quad;

// number of global variables, from preprocessing
extern int num_global_vars;
extern struct symtable *flat_id_table;
extern struct symtable *stringconst_table;

int temp_count = 0;

// symnode of the main function (declared in symtab.c)
extern struct symnode *main_func_symnode;

char *quad_op_string[] = {
    "call_func_op",
    "print_int_op",
    "print_float_op",
    "print_string_op",
    "int_to_float_op",
    "assn_int_to_var_op",
    "assn_int_to_arraysub_op",
    "assn_int_from_arraysub_op",
    "assn_float_to_var_op",
    "assn_float_to_arraysub_op",
    "assn_float_from_arraysub_op",
    "add_ints_op", //
    "add_floats_op",
    "sub_ints_op", //
    "sub_floats_op",
    "mult_ints_op", //
    "mult_floats_op",
    "div_ints_op", //
    "div_floats_op",
    "mod_op",
    "lt_ints_op",
    "lt_floats_op",
    "leq_ints_op",
    "leq_floats_op",
    "gt_ints_op",
    "gt_floats_op",
    "geq_ints_op",
    "geq_floats_op",
    "eq_ints_op",
    "eq_floats_op",
    "neq_ints_op",
    "neq_floats_op",
    "int_neg_op",
    "float_neg_op",
    "bang_op",
    "var_inc_op",
    "array_inc_op",
    "var_dec_op",
    "array_dec_op",
    "if_false_op",
    "if_true_op",
    "goto_op",
    "read_int_op",
    "read_double_op",
    "func_decl_op",
    "push_param_op",
    "pop_params_op",
    "alloc_array_op",
    "return_op",
    "assign_int_literal",
    "assign_double_literal",
    "initial_main_call",
    "store_string_op",
};

/* some more prototypes */

// Generates code for the standard binary operation with widening
// : +, -, *, /
struct quadarg *
generate_binary_op_with_widening(struct pnode *node,
                                 enum quadop quad_op_ints,
                                 enum quadop quad_op_floats);

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
    switch (quadarg->type) {
    case int_arg:
        printf("int: %d", quadarg->value.int_value);
        break;
    case dbl_arg:
        printf("dbl: %f", quadarg->value.dbl_value);
        break;
    case id_arg:
        printf("id: %s", quadarg->value.varnode->name);
        break;
    default:
        printf("unknown");
        break;
    }
}

void quad_vec_init(int size) {
    if (size <= 0)
        size = DEFAULT_QUAD_ARRAY_SIZE;
    quad_array = vec_new(size);
}

struct quad *quad_new(int lineno, enum quadop op,
                      struct quadarg *arg1, struct quadarg *arg2,
                      struct quadarg *arg3) {

    struct quad *quad = malloc(sizeof(struct quad));
    quad->op = op;
    quad->arg1 = arg1;
    quad->arg2 = arg2;
    quad->arg3 = arg3;
    quad->lineno = lineno;

    if (arg2->type != arg3->type)
        alert("quad_new: arg2 and arg3 have different types");

    quad_add(quad);

    return quad;
}

void quad_patch(struct quad *q, int arg_index,
                struct quadarg *new_quadarg) {
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

struct quad *quad_modify_op(struct quad *quad,
                            enum global_quadop op) {
    switch (op) {
    case plus:
        if (quad->arg1->type == int_arg &&
            quad->arg2->type == int_arg &&
            quad->arg3->type == int_arg) {
            quad->op = add_ints_op;
        } else if (quad->arg1->type == dbl_arg &&
                   quad->arg2->type == dbl_arg &&
                   quad->arg3->type == dbl_arg) {
            quad->op = add_floats_op;
        } else {
            alert("quad_modify_op: invalid types for +");
        }
        break;
    case minus:
        if (quad->arg1->type == int_arg &&
            quad->arg2->type == int_arg &&
            quad->arg3->type == int_arg) {
            quad->op = sub_ints_op;
        } else if (quad->arg1->type == dbl_arg &&
                   quad->arg2->type == dbl_arg &&
                   quad->arg3->type == dbl_arg) {
            quad->op = sub_floats_op;
        } else {
            alert("quad_modify_op: invalid types for -");
        }
        break;
    case mult:
        if (quad->arg1->type == int_arg &&
            quad->arg2->type == int_arg &&
            quad->arg3->type == int_arg) {
            quad->op = mult_ints_op;
        } else if (quad->arg1->type == dbl_arg &&
                   quad->arg2->type == dbl_arg &&
                   quad->arg3->type == dbl_arg) {
            quad->op = mult_floats_op;
        } else {
            alert("quad_modify_op: invalid types for *");
        }
        break;
    case divi:
        if (quad->arg1->type == int_arg &&
            quad->arg2->type == int_arg &&
            quad->arg3->type == int_arg) {
            quad->op = div_ints_op;
        } else if (quad->arg1->type == dbl_arg &&
                   quad->arg2->type == dbl_arg &&
                   quad->arg3->type == dbl_arg) {
            quad->op = div_floats_op;
        } else {
            alert("quad_modify_op: invalid types for /");
        }
        break;
    case mod:
        if (quad->arg1->type == int_arg &&
            quad->arg2->type == int_arg &&
            quad->arg3->type == int_arg) {
            quad->op = mod_op;
        } else {
            alert("quad_modify_op: invalid types for %%");
        }
        break;
    default:
        alert("quad_modify_op: invalid op");
        break;
    }

    return quad;
}

void quad_add(struct quad *quad) { vec_push(quad_array, quad); }

vec_t quad_append(vec_t quad_array1, vec_t quad_array2) {
    return vec_append(quad_array1, quad_array2);
}

struct quadarg *quadarg_new(enum quadargtype type, void *value) {
    struct quadarg *quadarg = calloc(1, sizeof(struct quadarg));
    quadarg->type = type;

    // if no value enter, return the quadarg
    if (!value)
        return quadarg;

    switch (type) {
    case int_arg:
        quadarg->value.int_value = *(int *)value;
        break;
    case dbl_arg:
        quadarg->value.dbl_value = *(double *)value;
        break;
    case id_arg:
        quadarg->value.varnode = (struct symnode *)value;
        break;
    default:
        alert("quadarg_new: invalid type");
    }

    return quadarg;
}

struct quadarg *quadarg_new_tmp(struct symtable *symtab,
                                enum vartype type) {
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
        temp_symnode->var_addr =
            -8 * (curr_func_symnode_quad->num_vars +
                  curr_func_symnode_quad->num_temps);
    } else {
        temp_symnode->mem_addr_type = global;
        global_temp_count++;
        temp_symnode->var_addr =
            -8 * (num_global_vars + global_temp_count);
    }

    struct quadarg *quadarg = quadarg_new(id_arg, NULL);
    quadarg->value.varnode = temp_symnode;

    temp_count++;
    return quadarg;
}