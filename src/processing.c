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
int children_fill_id_types(struct pnode *node) {
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

  int saved_entered_func_scope;

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
    if (num_params < 0) {
      mark_error(node->lineno, "A function must have a body");
      return 1;
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
        printf("\t--> id: %s\n", id_astnode->value.symnode->name);
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
      } else if (child->node_type == ARRAY_SUB ||
                 child->node_type == OP_ASSIGN) {
        id_astnode = child->left_child;

#ifdef DEBUG
        printf("\t--> id: %s\n", id_astnode->value.symnode->name);
#endif

        // look up the name in the ID name table and make sure it doesn't
        // already exist in this scope
        basename = id_astnode->value.symnode->name;
        prev_scoped_id_symnode =
            symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
        if (prev_scoped_id_symnode != NULL &&
            scoped_id_table_level == scoped_id_table->inner_scope->level) {
          mark_error(node->lineno,
                     "An ID of the same name already exists in this scope");
          return 1;
        }

        // add the param ID to the scoped ID table, then add it with the mangled
        // name to the flat ID table
        scoped_id_symnode = symtable_insert(scoped_id_table, basename);
        flat_id_symnode =
            symtable_insert(flat_id_table, scoped_id_symnode->mangled_name);

        // set the node and var_type of the sym_node, and point to the symnode
        // from the id astnode
        if (child->node_type == ARRAY_SUB) {
          flat_id_symnode->node_type = array_node;
        } else {
          flat_id_symnode->node_type = val_node;
        }
        flat_id_symnode->var_type = var_type;
        id_astnode->value.symnode = flat_id_symnode;

        // record size if the subscript is an int literal
        if (id_astnode->right_sibling->node_type == INT_LITERAL) {
          flat_id_symnode->array_size =
              id_astnode->right_sibling->value.int_val;
        }

        // recurse on the subscript expression
        if (fill_id_types(id_astnode->right_sibling) != 0) {
          return 1;
        }

        // set the memory address of this variable and increment the number of
        // variables in the current function or global variables
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
      }
    }
    return 0;
  }

  case ARRAY_NONSUB: {
    ret_node_type = node->left_child->node_type;
    if (ret_node_type == INT_TYPE) {
      var_type = inttype;
    } else if (ret_node_type == DBL_TYPE) {
      var_type = doubletype;
    } else {
      return 1; // error
    }

    // get the param ID's ast node
    id_astnode = node->left_child->right_sibling;

    // look up the name in the ID name table and make sure it doesn't already
    // exist in this scope
    basename = id_astnode->value.symnode->name;
    prev_scoped_id_symnode =
        symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
    if (prev_scoped_id_symnode != NULL &&
        scoped_id_table_level == scoped_id_table->inner_scope->level) {
      mark_error(node->lineno, "A formal parameter of the same name already "
                               "exists in this function declaration");
      return 1;
    }

    // add the param ID to the scoped ID table, then add it with the mangled
    // name to the flat ID table
    scoped_id_symnode = symtable_insert(scoped_id_table, basename);
    flat_id_symnode =
        symtable_insert(flat_id_table, scoped_id_symnode->mangled_name);

    // set the node and var_type of the sym_node, and point to the symnode from
    // the id astnode
    flat_id_symnode->node_type = array_node;
    flat_id_symnode->var_type = var_type;
    id_astnode->value.symnode = flat_id_symnode;

    // insert the param symnode into the proper bucket of the current function's
    // param array, and set the param's address
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
  case ROOT: {
    for (child = node->left_child; child != NULL;
         child = child->right_sibling) {
      if (fill_id_types(child) != 0) return 1;
    }

    return 0;
  }
  case ID: {
    // we already have the ID's ast_node
    id_astnode = node;

    // look up the name in the ID name table and make sure it is accessible in
    // the current scope
    basename = id_astnode->value.symnode->name;
    prev_scoped_id_symnode =
        symtable_lookup(scoped_id_table, basename, &scoped_id_table_level);
    if (prev_scoped_id_symnode == NULL) {
      mark_error(node->lineno, "A variable or function is referenced but not "
                               "yet declared in an accessible scope");
      return 1;
    }

    // find the correct mangled symnode
    flat_id_symnode =
        symtable_lookup(flat_id_table, prev_scoped_id_symnode->mangled_name,
                        &scoped_id_table_level);

    // the ast node points to the mangled sym_node of the proper declaration
    id_astnode->value.symnode = flat_id_symnode;

    return 0;
  }

  case ARRAY_SUB:
    return children_fill_id_types(node);
  case OP_ASSIGN:
    return children_fill_id_types(node);
  case OP_ADD:
    return children_fill_id_types(node);
  case OP_SUB:
    return children_fill_id_types(node);
  case OP_MULT:
    return children_fill_id_types(node);
  case OP_DIV:
    return children_fill_id_types(node);
  case OP_MOD:
    return children_fill_id_types(node);
  case OP_LT:
    return children_fill_id_types(node);
  case OP_LEQ:
    return children_fill_id_types(node);
  case OP_GT:
    return children_fill_id_types(node);
  case OP_GEQ:
    return children_fill_id_types(node);
  case OP_EQ:
    return children_fill_id_types(node);
  case OP_NEQ:
    return children_fill_id_types(node);
  case OP_AND:
    return children_fill_id_types(node);
  case OP_OR:
    return children_fill_id_types(node);
  case OP_BANG:
    return children_fill_id_types(node);
  case OP_NEG:
    return children_fill_id_types(node);
  case OP_INC:
    return children_fill_id_types(node);
  case OP_DEC:
    return children_fill_id_types(node);

  case SEQ: {
    // enter a new scope for the SEQ (unless a function scope has just been
    // entered)
    saved_entered_func_scope = entered_func_scope;
    entered_func_scope = 0;
    if (saved_entered_func_scope == 0) {
      symtable_enter_scope(scoped_id_table);
    }

    // recurse on children
    for (child = node->left_child; child != NULL;
         child = child->right_sibling) {
      if (fill_id_types(child) != 0) return 1;
    }

    // exit the SEQ's scope (but only if it entered one at the beginning)
    if (saved_entered_func_scope == 0) {
      symtable_leave_scope(scoped_id_table);
    }

    return 0;
  }
  case IF_STMT:
    return children_fill_id_types(node);
  case WHILE_LOOP:
    return children_fill_id_types(node);
  case DO_WHILE_LOOP:
    return children_fill_id_types(node);
  case FOR_STMT:
    return children_fill_id_types(node);
  case RETURN_STMT:
    return children_fill_id_types(node);
  case READ_STMT:
    return children_fill_id_types(node);
  case PRINT_STMT:
    return children_fill_id_types(node);
  case FUNC_CALL:
    return children_fill_id_types(node);
  case STRING_LITERAL:
    return 0;
  case INT_LITERAL:
    return 0;
  case DOUBLE_LITERAL:
    return 0;
  case EMPTY_EXPR:
    return 0;
  case INT_TYPE:
    return 0;
  case DBL_TYPE:
    return 0;
  case VOID_TYPE:
    return 0;

  default: // this is to make -Wall happy
    return 0;
  }
}

int type_check(struct pnode *node) {
  struct pnode *child;
  for (child = node->left_child; child != NULL; child = child->right_sibling) {
    if (type_check(child) != 0) return 1;
  }

  node->data_type = no_type;
  node->return_type = no_type;

  enum vartype sub_type;
  enum vartype ltype;
  enum vartype rtype;
  enum vartype dtype;

  enum vartype decl_return_type, body_return_type;
  struct pnode *seq_node;

  struct symnode *func_sym_node;
  int i;
  struct pnode *actual_param_node;

  enum vartype actual_type;
  enum vartype formal_type;

  int shouldWiden;

  switch (node->node_type) {
  case ROOT:
    return 0;
  case ID:
    node->data_type = node->value.symnode->var_type;
    return 0;
  case INT_TYPE:
    return 0;
  case DBL_TYPE:
    return 0;
  case VOID_TYPE:
    return 0;

  case ARRAY_SUB: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Array subscript expression is missing");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Array subscript expression is missing");
      return 1;
    }
    sub_type = node->left_child->right_sibling->data_type;
    if (sub_type != inttype) {
      mark_error(node->lineno, "Array index is not an int");
      return 1;
    } else if (is_array_node(node->left_child->right_sibling)) {
      mark_error(node->lineno,
                 "Array subscript expression cannot be an array identifier");
      return 1;
    } else if (node->left_child->value.symnode->node_type != array_node) {
      mark_error(node->lineno, "Subscripted variable is not an array");
      return 1;
    }

    node->data_type = node->left_child->value.symnode->var_type;

    return 0;
  }

  case ARRAY_NONSUB: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Array subscript expression is missing");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Array subscript expression is missing");
      return 1;
    }
    node->data_type = node->left_child->right_sibling->value.symnode->var_type;
    return 0;
  }

  case OP_ASSIGN: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Left-hand side of assignment is missing");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Right-hand side of assignment is missing");
      return 1;
    }
    if (is_array_node(node->left_child)) {
      mark_error(node->lineno, "Cannot assign to an array pointer");
      return 1;
    } else if (is_array_node(node->left_child->right_sibling)) {
      mark_error(node->lineno, "Cannot assign an array pointer to a variable");
      return 1;
    }

    ltype = node->left_child->data_type;
    rtype = node->left_child->right_sibling->data_type;

    // widen if assigning int to double
    shouldWiden = (ltype == doubletype) && (rtype == inttype);
    if (rtype != ltype && !shouldWiden) {
      mark_error(node->lineno, "Cannot assign a double to an int");
      return 1;
    }
    node->data_type = ltype;
    return 0;
  }

  case OP_ADD:
    return standard_binary_op_typecheck_widening(node);
  case OP_SUB:
    return standard_binary_op_typecheck_widening(node);
  case OP_MULT:
    return standard_binary_op_typecheck_widening(node);
  case OP_DIV:
    return standard_binary_op_typecheck_widening(node);

  case OP_MOD: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Left-hand side of mod is missing");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Right-hand side of mod is missing");
      return 1;
    }

    ltype = node->left_child->data_type;
    rtype = node->left_child->right_sibling->data_type;

    if (ltype != inttype || rtype != inttype) {
      mark_error(node->lineno, "An operand of mod is not of type int");
      return 1;
    } else if (is_array_node(node->left_child) ||
               is_array_node(node->left_child->right_sibling)) {
      mark_error(node->lineno,
                 "The mod operator cannot act on an array identifier operand");
      return 1;
    }

    node->data_type = inttype;
    return 0;
  }

  case OP_LT:
    return standard_binary_op_typecheck_int(node);
  case OP_LEQ:
    return standard_binary_op_typecheck_int(node);
  case OP_GT:
    return standard_binary_op_typecheck_int(node);
  case OP_GEQ:
    return standard_binary_op_typecheck_int(node);
  case OP_EQ:
    return standard_binary_op_typecheck_int(node);
  case OP_NEQ:
    return standard_binary_op_typecheck_int(node);

  case OP_AND: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Left-hand side of && is missing");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Right-hand side of && is missing");
      return 1;
    }

    ltype = node->left_child->data_type;
    rtype = node->left_child->right_sibling->data_type;

    if (ltype != inttype || rtype != inttype) {
      mark_error(node->lineno, "An operand of && is not an int");
      return 1;
    } else if (is_array_node(node->left_child) ||
               is_array_node(node->left_child->right_sibling)) {
      mark_error(node->lineno,
                 "The && operator cannot act on an array identifier operand");
      return 1;
    }

    node->data_type = inttype;
    return 0;
  }

  case OP_OR: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Left-hand side of || is missing");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Right-hand side of || is missing");
      return 1;
    }

    ltype = node->left_child->data_type;
    rtype = node->left_child->right_sibling->data_type;

    if (ltype != inttype || rtype != inttype) {
      mark_error(node->lineno, "An operand of || is not an int");
      return 1;
    } else if (is_array_node(node->left_child) ||
               is_array_node(node->left_child->right_sibling)) {
      mark_error(node->lineno,
                 "The || operator cannot act on an array identifier operand");
      return 1;
    }

    node->data_type = inttype;
    return 0;
  }

  case OP_BANG: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Operand of ! is missing");
      return 1;
    }

    if (node->left_child->data_type != inttype) {
      mark_error(node->lineno, "The operand of ! is not an int");
      return 1;
    } else if (is_array_node(node->left_child)) {
      mark_error(node->lineno,
                 "The ! operator cannot act on an array identifier operand");
      return 1;
    }

    node->data_type = inttype;
    return 0;
  }

  case OP_NEG: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Operand of - (unary minus) is missing");
      return 1;
    }

    dtype = node->left_child->data_type;

    if (dtype != inttype && dtype != doubletype) {
      mark_error(node->lineno,
                 "The operand of - (unary minus) is not an int or double");
      return 1;
    } else if (is_array_node(node->left_child)) {
      mark_error(
          node->lineno,
          "The negation operator cannot act on an array identifier operand");
      return 1;
    }

    node->data_type = dtype;
    return 0;
  }

  case OP_INC: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Operand of ++ is missing");
      return 1;
    }

    dtype = node->left_child->data_type;

    if (dtype != inttype) {
      mark_error(node->lineno, "The operand of ++ is not an int");
      return 1;
    } else if (is_array_node(node->left_child)) {
      mark_error(node->lineno,
                 "The ++ operator cannot act on an array pointer");
      return 1;
    }

    node->data_type = inttype;
    return 0;
  }

  case OP_DEC: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Operand of -- is missing");
      return 1;
    }

    dtype = node->left_child->data_type;

    if (dtype != inttype) {
      mark_error(node->lineno, "The operand of -- is not an int");
      return 1;
    } else if (is_array_node(node->left_child)) {
      mark_error(node->lineno,
                 "The -- operator cannot act on an array identifier operand");
      return 1;
    }

    node->data_type = inttype;
    return 0;
  }

  case FUNC_DECL: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Function declaration is missing a name");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "Function declaration is missing a body");
      return 1;
    }

    decl_return_type = node->left_child->right_sibling->value.symnode->var_type;

    // clang-format off
    for (seq_node = node->left_child; seq_node->right_sibling != NULL;
         seq_node = seq_node->right_sibling); // bitchin'
    // clang-format on

    body_return_type = seq_node->return_type;

    if (!((body_return_type == no_type && decl_return_type == voidtype) ||
          (body_return_type == decl_return_type) ||
          (body_return_type == inttype && decl_return_type == doubletype))) {
      mark_error(
          node->lineno,
          "The return types of the function declaration and body do not match");
      return 1;
    }

    node->return_type = decl_return_type;

    return 0;
  }

  case VAR_DECL:
    return 0;
  case FORMAL_PARAM:
    return 0;

  case SEQ: {
    for (child = node->left_child; child != NULL;
         child = child->right_sibling) {
      if (node->return_type == no_type) {
        node->return_type = child->return_type;
      } else if (node->return_type == inttype &&
                 child->return_type == doubletype) { // Widen to doubletype
        node->return_type = doubletype;
      } else if (child->return_type != no_type &&
                 node->return_type != child->return_type) {
        mark_error(node->lineno,
                   "The type of this return statement conflicts with that of a "
                   "previous return statement in this function");
        return 1;
      }
    }

    return 0;
  }

  case IF_STMT:
    if (node->left_child == NULL) {
      mark_error(node->lineno, "if statement is missing a condition");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "if statement is missing a body");
      return 1;
    }

    if (node->left_child->right_sibling->right_sibling == NULL) {
      node->return_type = node->left_child->right_sibling->return_type;
    } else {
      if (node->left_child->right_sibling->return_type ==
              node->left_child->right_sibling->right_sibling->return_type &&
          node->left_child->right_sibling->return_type != no_type) {
        node->return_type = node->left_child->right_sibling->return_type;
      } else if (node->left_child->right_sibling->return_type == no_type &&
                 node->left_child->right_sibling->right_sibling->return_type !=
                     no_type) {
        node->return_type =
            node->left_child->right_sibling->right_sibling->return_type;
      } else if (node->left_child->right_sibling->return_type != no_type &&
                 node->left_child->right_sibling->right_sibling->return_type ==
                     no_type) {
        node->return_type = node->left_child->right_sibling->return_type;
      } else {
        node->return_type = no_type;
      }
    }
    node->return_type = node->left_child->right_sibling->return_type;
    return 0;
  case WHILE_LOOP:
    if (node->left_child == NULL) {
      mark_error(node->lineno, "while loop is missing a condition");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "while loop is missing a body");
      return 1;
    }
    node->return_type = node->left_child->right_sibling->return_type;
    return 0;
  case DO_WHILE_LOOP:
    if (node->left_child == NULL) {
      mark_error(node->lineno, "do-while loop is missing a condition");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "do-while loop is missing a body");
      return 1;
    }
    node->return_type = node->left_child->return_type;
    return 0;
  case FOR_STMT:
    if (node->left_child == NULL) {
      mark_error(node->lineno, "for loop is missing an initialization");
      return 1;
    }
    if (node->left_child->right_sibling == NULL) {
      mark_error(node->lineno, "for loop is missing a condition");
      return 1;
    }
    if (node->left_child->right_sibling->right_sibling == NULL) {
      mark_error(node->lineno, "for loop is missing an increment");
      return 1;
    }
    if (node->left_child->right_sibling->right_sibling->right_sibling == NULL) {
      mark_error(node->lineno, "for loop is missing a body");
      return 1;
    }

    node->return_type = node->left_child->right_sibling->right_sibling
                            ->right_sibling->return_type;
    return 0;

  case RETURN_STMT: {
    if (node->left_child != NULL) {
      if (is_array_node(node->left_child)) {
        mark_error(node->lineno, "Cannot return an array variable");
        return 1;
      } else {
        node->return_type = node->left_child->data_type;
      }
    } else {
      node->return_type = voidtype;
    }

    return 0;
  }

  case READ_STMT: {
    if (is_array_node(node->left_child)) {
      mark_error(node->lineno,
                 "Cannot read into an array variable that is not subscripted");
    }

    return 0;
  }

  case PRINT_STMT: {
    if (is_array_node(node->left_child)) {
      mark_error(node->lineno,
                 "Cannot print an array variable that is not subscripted");
    }

    return 0;
  }

  case STRING_LITERAL:
    return 0;
  case INT_LITERAL:
    node->data_type = inttype;
    return 0;
  case DOUBLE_LITERAL:
    node->data_type = doubletype;
    return 0;

  case FUNC_CALL: {
    if (node->left_child == NULL) {
      mark_error(node->lineno, "Function call is missing a name");
      return 1;
    }

    // get function's sym_node
    func_sym_node = node->left_child->value.symnode;

    // set data_type equal to return type of function
    node->data_type = func_sym_node->var_type;

    // check that there are the correct number of actual parameters
    for (actual_param_node = node->left_child->right_sibling, i = 0;
         actual_param_node != NULL;
         actual_param_node = actual_param_node->right_sibling, i++)
      ;
    if (i != func_sym_node->num_params) {
      mark_error(node->lineno,
                 "The number of actual parameters passed does not match the "
                 "number of formal parameters in the function declaration");
      return 1;
    }

    // check parameter types
    for (actual_param_node = node->left_child->right_sibling, i = 0;
         actual_param_node != NULL;
         actual_param_node = actual_param_node->right_sibling, i++) {
      // if the function wants an array pointer
      if (func_sym_node->param_symnode_array[i]->node_type == array_node &&
          (actual_param_node->node_type != ID ||
           actual_param_node->value.symnode->node_type != array_node)) {
        mark_error(node->lineno,
                   "A formal parameter is an array pointer, but the "
                   "corresponding actual parameter passed is not");
        return 1;
      }

      // if the function doesn't want an array pointer but gets one
      if (func_sym_node->param_symnode_array[i]->node_type != array_node &&
          (actual_param_node->node_type == ID &&
           actual_param_node->value.symnode->node_type == array_node)) {
        mark_error(node->lineno,
                   "A formal parameter is not an array pointer, but the "
                   "corresponding actual parameter passed is");
      }

      actual_type = actual_param_node->data_type;
      formal_type = func_sym_node->param_symnode_array[i]->var_type;

      // error if you are expecting an int and get a double
      if (formal_type == inttype && actual_type == doubletype) {
        mark_error(node->lineno,
                   "An actual parameter passed is a double, but the "
                   "corresponding formal parameter is an int");
        return 1;
      } else if (actual_type ==
                 voidtype) { // error if a parameter passed is of type void
        mark_error(node->lineno, "You can't pass a parameter of type void");
        return 1;
      }
    }

    return 0;
  }

  case EMPTY_EXPR:
    return 0;
  }
#ifdef DEBUG
  pnode_display(node);
#endif
  return 0;
}

int standard_binary_op_typecheck_widening(struct pnode *node) {
  if (node->left_child == NULL || node->left_child->right_sibling == NULL) {
    mark_error(node->lineno, "Binary operator is missing an operand");
    return 1;
  }

  enum vartype ltype = node->left_child->data_type;
  enum vartype rtype = node->left_child->right_sibling->data_type;

  if ((ltype != inttype && ltype != doubletype) ||
      (rtype != inttype && rtype != doubletype)) {
    mark_error(node->lineno, "An operand is not of type int or double");
    return 1;
  } else if (is_array_node(node->left_child) ||
             is_array_node(node->left_child->right_sibling)) {
    mark_error(node->lineno, "One of the operands is an array variable");
    return 1;
  } else if ((ltype == doubletype) || (rtype == doubletype)) {
    node->data_type = doubletype;
  } else {
    node->data_type = inttype;
  }

  return 0;
}

int standard_binary_op_typecheck_int(struct pnode *node) {
  if (node->left_child == NULL || node->left_child->right_sibling == NULL) {
    mark_error(node->lineno, "Binary operator is missing an operand");
    return 1;
  }

  enum vartype ltype = node->left_child->data_type;
  enum vartype rtype = node->left_child->right_sibling->data_type;

  if ((ltype != inttype && ltype != doubletype) ||
      (rtype != inttype && rtype != doubletype)) {
    mark_error(node->lineno, "An operand is not of type int or float");
    return 1;
  } else if (is_array_node(node->left_child) ||
             is_array_node(node->left_child->right_sibling)) {
    mark_error(node->lineno, "One of the operands is an array variable");
    return 1;
  }

  node->data_type = inttype;

  return 0;
}

int is_array_node(struct pnode *node) {
  if (node == NULL)
    return 0;

  return node->node_type == ID && node->value.symnode->node_type == array_node;
}
