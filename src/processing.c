#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "processing.h"

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
        sym_node->name, sym_node, node_type_string[sym_node->nodetype],
        var_type_string[sym_node->type], sym_node->var_addr,
        mem_addr_type_string[sym_node->memaddrtype], sym_node->num_vars);
    break;
  case INT_LITERAL:
    printf("%d", node->value.int_val);
    break;
  case DOUBLE_LITERAL:
    printf("%f", node->value.dbl_val);
    break;
  case STRING_LITERAL:
    printf("%s (%p)", node->value.symnode->name, node->value.symnode);
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
