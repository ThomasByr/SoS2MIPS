#pragma once

#include "symtable.h"

// clang-format off
enum pnodetype {
  ROOT,
  ID,
  INT_TYPE, DBL_TYPE, VOID_TYPE,
  ARRAY_SUB, ARRAY_NONSUB,
  OP_ASSIGN, OP_ADD, OP_SUB, OP_MULT, OP_DIV, OP_MOD, 
  OP_LT, OP_LEQ, OP_GT, OP_GEQ, OP_EQ, OP_NEQ, OP_AND, OP_OR, 
  OP_BANG, OP_NEG,
  OP_INC, OP_DEC,  
  FUNC_DECL, VAR_DECL, FORMAL_PARAM,
  SEQ,
  IF_STMT, WHILE_LOOP, DO_WHILE_LOOP, FOR_STMT,
  RETURN_STMT,
  READ_STMT, PRINT_STMT,
  STRING_LITERAL, INT_LITERAL, DOUBLE_LITERAL,
  FUNC_CALL,
  EMPTY_EXPR
};
// clang-format on

struct pnode {
  enum pnodetype node_type;
  struct pnode *left_child, *right_sibling;
  union {
    struct symnode *symnode; // for ID or STRING_LITERAL
    int int_val;             // for INT_LITERAL
    double dbl_val;          // for DOUBLE_LITERAL
  } value;
  int lineno;
  enum vartype data_type;
  enum vartype return_type;
};

/**
 * @brief Create a node with a given token type and return a pointer to the
   node.
 *
 */
struct pnode *pnode_new(enum pnodetype node_type);

/**
 * @brief Print the contents of a subtree of an abstract syntax tree, given
   the root of the subtree and the depth of the subtree root.
 *
 */
void p_display(struct pnode *root, int depth);

void pnode_display(struct pnode *node);

struct pnode *rightmost_sibling(struct pnode *node);

/**
 * @brief Performs a pre-order traversal of the syntax tree to set the types of
 identifiers. Uses a multi-level symbol table to keep track of scoping; then ID
 names are mangled and saved in a flat symbol table.
 *
 */
int fill_id_types(struct pnode *node);

/**
 * @brief Performs a post-order traversal of the syntax tree to check
   type compatibilities throughout the program.
 *
 */
int type_check(struct pnode *node);
