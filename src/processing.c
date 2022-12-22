#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processing.h"

#include "lib.h"

extern const char var_type_string[][8];
extern const char node_type_string[][11];
extern const char mem_addr_type_string[][7];

struct token_lookup {
  char *token;
  enum pnodetype node_type;
};

static struct token_lookup token_table[] = {
    {"ROOT", ROOT},
    {"ID", ID},
    {"INT_TYPE", INT_TYPE},
    {"DBL_TYPE", DBL_TYPE},
    {"VOID_TYPE", VOID_TYPE},
    {"ARRAY_SUB", ARRAY_SUB},
    {"ARRAY_NONSUB", ARRAY_NONSUB},
    {"OP_ASSIGN", OP_ASSIGN},
    {"OP_ADD", OP_ADD},
    {"OP_SUB", OP_SUB},
    {"OP_MULT", OP_MULT},
    {"OP_DIV", OP_DIV},
    {"OP_MOD", OP_MOD},
    {"OP_LT", OP_LT},
    {"OP_LEQ", OP_LEQ},
    {"OP_GT", OP_GT},
    {"OP_GEQ", OP_GEQ},
    {"OP_EQ", OP_EQ},
    {"OP_NEQ", OP_NEQ},
    {"OP_AND", OP_AND},
    {"OP_OR", OP_OR},
    {"OP_BANG", OP_BANG},
    {"OP_NEG", OP_NEG},
    {"OP_INC", OP_INC},
    {"OP_DEC", OP_DEC},
    {"FUNC_DECL", FUNC_DECL},
    {"VAR_DECL", VAR_DECL},
    {"FORMAL_PARAM", FORMAL_PARAM},
    {"SEQ", SEQ},
    {"IF_STMT", IF_STMT},
    {"WHILE_LOOP", WHILE_LOOP},
    {"DO_WHILE_LOOP", DO_WHILE_LOOP},
    {"FOR_STMT", FOR_STMT},
    {"RETURN_STMT", RETURN_STMT},
    {"READ_STMT", READ_STMT},
    {"PRINT_STMT", PRINT_STMT},
    {"STRING_LITERAL", STRING_LITERAL},
    {"INT_LITERAL", INT_LITERAL},
    {"DOUBLE_LITERAL", DOUBLE_LITERAL},
    {"FUNC_CALL", FUNC_CALL},
    {"EMPTY_EXPR", EMPTY_EXPR},
    {NULL, 0},
};

struct pnode *pnode_new(enum pnodetype node_type) {
  struct pnode *node = malloc(sizeof(struct pnode));
  node->node_type = node_type;
  node->left_child = node->right_sibling = NULL;
  return node;
}

void p_display(struct pnode *root, int depth) {
  int i;
  for (i = 0; i < depth; i++) {
    printf(" ");
  }

  pnode_display(root);
  printf("\n");

  struct pnode *child;
  for (child = root->left_child; child != NULL; child = child->right_sibling) {
    p_display(child, depth + 1);
  }
}

void pnode_display(struct pnode *node) {
  // node type
  printf("%s ", token_table[node->node_type].token);

  struct symnode *sym_node = node->value.symnode;
  // specific attributes
  switch (node->node_type) {
  case ID:
    printf(
        "%s (%p), node type: %s, var type: %s, address: %d (%s), num vars: %d",
        sym_node->name, (void *)sym_node, node_type_string[sym_node->node_type],
        var_type_string[sym_node->var_type], sym_node->var_addr,
        mem_addr_type_string[sym_node->mem_addr_type], sym_node->num_vars);
    break;
  case INT_LITERAL:
    printf("%d", node->value.int_val);
    break;
  case DOUBLE_LITERAL:
    printf("%f", node->value.dbl_val);
    break;
  case STRING_LITERAL:
    printf("%s (%p)", node->value.symnode->name, (void *)node->value.symnode);
    break;
  default:
    break;
  }
  printf("\t --> data type: %s, return type: %s",
         var_type_string[node->data_type], var_type_string[node->return_type]);
}

struct pnode *rightmost_sibling(struct pnode *node) {
  // clang-format off
  for (; node->right_sibling != NULL; node = node->right_sibling); // bitchin'
  // clang-format on
  return node;
}

/* some more prototypes */

// Standard code for checking the types of the operands of a binary operation
// and setting the type of the operation
int standard_binary_op_typecheck_widening(struct pnode *node);

// Standard code for checking the types of the operands
// of a binary operation that is of type int
// and setting the type of the operation (<, <=, >, >=, == , !=)
int standard_binary_op_typecheck_int(struct pnode *node);

// Returns true if the given AST node is an ID and is an array node
int is_array_node(struct pnode *node);

/* variables */

extern struct symtable *scoped_id_table;
extern struct symtable *flat_id_table;
int scoped_id_table_level;

extern int error_count;

// nonzero when a scope has already been entered into for a function
int entered_func_scope = 0;

// the symnode of the function currently being processed;
// NULL when outside of a function
struct symnode *curr_func_symnode_anp;

// the number of global variables
int num_global_vars = 0;

// The symnode of the main function (declared in symtab.c)
extern struct symnode *main_func_symnode;

/* definitions */

void mark_error(int line, const char *restrict msg) {
  alert("Error found at line %d: %s", line, msg);
  error_count++;

  debug("##### scoped_id_table #####");
#ifdef DEBUG
  symtable_display(scoped_id_table);
#endif
}

// Recursively call fill_id_types on children and
// return 1 if any of the recursive calls return nonzero;
// otherwise return 0
int childrend_fill_id_types(struct pnode *node) {
  struct pnode *child;
  for (child = node->left_child; child != NULL; child = child->right_sibling) {
    if (fill_id_types(child) != 0) return 1;
  }
  return 0;
}

int fill_id_types(struct pnode *node) {
#ifdef DEBUG
  pnode_display(node);
  printf("\n");
#endif

  // some variables used in the switch statement

  enum vartype var_type;
  enum pnodetype ret_node_type, var_node_type;

  struct pnode *id_astnode;

  char *basename;
  struct symnode *prev_scoped_id_symnode, *scoped_id_symnode, *flat_id_symnode;

  struct pnode *child;

  int num_params;

  // set types if we have a declaration (function or variable) or a
  // parameter
  switch (node->node_type) {
  case FUNC_DECL: {
    // find return type
    ret_node_type = node->left_child->node_type;
    if (ret_node_type == INT_TYPE) {
      var_type = inttype;
    } else if (ret_node_type == DBL_TYPE) {
      var_type = doubletype;
    } else if (ret_node_type == VOID_TYPE) {
      var_type = voidtype;
    } else {
      return 1; // error
    }
    id_astnode = node->left_child->right_sibling;

    // look up the name in the ID name table
    // and make sure it doesn't already exist in this scope
    basename = id_astnode->value.symnode->name;
    prev_scoped_id_symnode =
        symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
    if (prev_scoped_id_symnode != NULL &&
        scoped_id_table_level == scoped_id_table->inner_scope->level) {
      mark_error(node->lineno,
                 "An ID of the same name already exists in this scope");
      return 1;
    }

    scoped_id_symnode = symtable_insert(scoped_id_table, basename);
    flat_id_symnode =
        symtable_insert(flat_id_table, scoped_id_symnode->mangled_name);
    flat_id_symnode->node_type = func_node;
    flat_id_symnode->var_type = var_type;
    id_astnode->value.symnode = flat_id_symnode;

    if (strcmp(basename, "main") == 0) {
      // We have an error if the declared return type is not void
      if (flat_id_symnode->var_type != voidtype) {
        mark_error(node->lineno,
                   "The main function must have a void return type");
      }
      main_func_symnode = flat_id_symnode;
    }

    // enter into the function's scope
    symtable_enter_scope(scoped_id_table);
    curr_func_symnode_anp = flat_id_symnode;
    flat_id_symnode->num_vars = 0;

    // count the number of parameters
    // start at -1 because the function body will be counted
    num_params = -1;
    for (child = id_astnode->right_sibling; child != NULL;
         child = child->right_sibling) {
      num_params++;
    }
    flat_id_symnode->param_symnode_array =
        calloc(num_params, sizeof(struct symnode *));
    // will count up when recursing on formal params,
    // which use it for array indexing
    flat_id_symnode->num_params = 0;

    // recurse on the formal parameters and body
    entered_func_scope = 1;
    for (child = id_astnode->right_sibling; child != NULL;
         child = child->right_sibling) {
      if (fill_id_types(child) != 0) return 1;
    }

    // exit the function's scope
    symtable_leave_scope(scoped_id_table);
    curr_func_symnode_anp = NULL;

#ifdef DEBUG
    pnode_display(node->left_child->right_sibling);
    printf("\n");
#endif

    return 0;
  }

  case FORMAL_PARAM: {
    ret_node_type = node->left_child->node_type;
    if (ret_node_type == INT_TYPE) {
      var_type = inttype;
    } else if (ret_node_type == DBL_TYPE) {
      var_type = doubletype;
    } else {
      return 1; // error
    }

    id_astnode = node->left_child->right_sibling;

    basename = id_astnode->value.symnode->name;
    prev_scoped_id_symnode =
        symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
    if (prev_scoped_id_symnode != NULL &&
        scoped_id_table_level == scoped_id_table->inner_scope->level) {
      mark_error(node->lineno, "A formal parameter of the same name already "
                               "exists in this function declaration");
      return 1;
    }

    scoped_id_symnode = symtable_insert(scoped_id_table, basename);
    flat_id_symnode =
        symtable_insert(flat_id_table, scoped_id_symnode->mangled_name);

    flat_id_symnode->node_type = val_node;
    flat_id_symnode->var_type = var_type;
    id_astnode->value.symnode = flat_id_symnode;

    curr_func_symnode_anp
        ->param_symnode_array[curr_func_symnode_anp->num_params] =
        flat_id_symnode;
    flat_id_symnode->mem_addr_type = off_fp;
    flat_id_symnode->var_addr = (1 + curr_func_symnode_anp->num_params) * 8;
    curr_func_symnode_anp->num_params++;

#ifdef DEBUG
    printf("\t");
    pnode_display(node->left_child->right_sibling);
    printf("\n");
#endif

    return 0;
  }

  case VAR_DECL: {

    var_node_type = node->left_child->node_type;
    if (var_node_type == INT_TYPE) {
      var_type = inttype;
    } else if (var_node_type == DBL_TYPE) {
      var_type = doubletype;
    } else {
      return 1; // error
    }

    // make the symnodes and set the types for all of the declared variables
    for (child = node->left_child->right_sibling; child != NULL;
         child = child->right_sibling) {
      if (child->node_type == ID) {
        // already have the ID node, so just make the symnode
        id_astnode = child;
#ifdef DEBUG
        printf("\t--> id: %s\n", id_astnode->value.sym_node->name);
#endif

        // look up the name in the ID name table
        // and make sure it doesn't already exist in this scope
        basename = id_astnode->value.symnode->name;

        prev_scoped_id_symnode =
            symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
        if (prev_scoped_id_symnode != NULL &&
            scoped_id_table_level == scoped_id_table->inner_scope->level) {
          mark_error(node->lineno,
                     "An ID of the same name already exists in this scope");
          return 1;
        }

        scoped_id_symnode = symtable_insert(scoped_id_table, basename);
        flat_id_symnode =
            symtable_insert(flat_id_table, scoped_id_symnode->mangled_name);

        flat_id_symnode->node_type = val_node;
        flat_id_symnode->var_type = var_type;
        id_astnode->value.symnode = flat_id_symnode;

        // set the memory address of this variable and increment the number of
        // variables in the current function or global variables
        // todo: fix this shit to work with global-by-default
        if (curr_func_symnode_anp != NULL) {
          flat_id_symnode->mem_addr_type = off_fp;
          curr_func_symnode_anp->num_vars++;
          // the first variable is at -8(fp)
          flat_id_symnode->var_addr = -8 * curr_func_symnode_anp->num_vars;
        } else {
          flat_id_symnode->mem_addr_type = global;
          num_global_vars++;
          // the first global variable is at -8 off the global variable pointer
          flat_id_symnode->var_addr = -8 * num_global_vars;
        }
      } else if (child->node_type == ARRAY_SUB || child->node_type == OP_ASSIGN) {
          id_astnode = child->left_child;

          #ifdef DEBUG
            printf("\t--> id: %s\n", id_astnode->value.sym_node->name);
          #endif

          // Look up the name in the ID name table and make sure it doesn't already exist in this scope
          basename = id_astnode->value.symnode->name;
          prev_scoped_id_symnode = symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
          if (prev_scoped_id_symnode != NULL && scoped_id_table_level == scoped_id_table->inner_scope->level) {
            mark_error(node->lineno, "An ID of the same name already exists in this scope");
            return 1;
          }

          // Add the param ID to the scoped ID table, then add it with the mangled name to the flat ID table
          scoped_id_symnode = symtable_insert(scoped_id_table, basename);
          flat_id_symnode = symtable_insert(flat_id_table, scoped_id_symnode->mangled_name);

          // Set the node and var_type of the sym_node, and point to the symnode from the id astnode
          if (child->node_type == ARRAY_SUB) {
            flat_id_symnode->node_type = array_node;
          } else {
            flat_id_symnode->node_type = val_node;
          }
          flat_id_symnode->var_type = var_type;
          id_astnode->value.symnode = flat_id_symnode;

          // Record size if the subscript is an int literal
          if (id_astnode->right_sibling->node_type == INT_LITERAL) {
            flat_id_symnode->array_size = id_astnode->right_sibling->value.int_val;
          }

          // Recurse on the subscript expression
          if (fill_id_types(id_astnode->right_sibling) != 0) {
            return 1;
          }

          // Set the memory address of this variable and increment the number of variables in the current function or global variables
          if (curr_func_symnode_anp != NULL) {
            flat_id_symnode->mem_addr_type = off_fp;
            curr_func_symnode_anp->num_vars++;
            flat_id_symnode->var_addr = -8 * curr_func_symnode_anp->num_vars; // the first variable is at -8(fp)
          } else {
            flat_id_symnode->mem_addr_type = global;
            num_global_vars++;
            flat_id_symnode->var_addr = -8 * num_global_vars; // the first global variable is at -8 off the global variable pointer
          }
        }
      }
    return 0;
    }


  default: // this is to make -Wall happy
    return 0;
  }
}
