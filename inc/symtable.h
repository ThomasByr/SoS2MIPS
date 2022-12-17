#pragma once

#include "dict.h"

/**
 * variable and return type
 * used for type checking
 */
enum vartype {
  VT_INT,
  VT_FLOAT,
  VT_STRING,
  VT_BOOL,
  VT_VOID,
  VT_ARRAY,
  VT_STRUCT,
  VT_FUNC,
  VT_UNKNOWN,
};

/**
 * the thing that the identifier identifies
 * not applicable in the string constant table
 */
enum nodetype {
  NT_VAR,
  NT_FUNC,
  NT_ARRAY,
};

/**
 * the type of the memory address
 */
enum memaddrtype {
  MAT_GLOBAL,
  MAT_LOCAL,
};

struct symnode {
  char *name; // name in this scope

  enum vartype type;      // the type of a variable or return type of a function
                          // (irrelevant for the string table)
  int var_addr;           // assembly index for functions
                          // irrelevant for string constants
  enum nodetype nodetype; // the thing that the identifier identifies
                          // (irrelevant for the string table)
  enum memaddrtype memaddrtype;
  int array_size; // only relevant for nodes of type array_node

  int num_vars;   // number of variables for a function
  int num_temps;  // number of temporary variables for a function
  int num_params; // number of formal parameters

  struct symnode *next; // next node in list
};

struct symhashtable {
  int size;                         // size of hash table
  dict_t table;                     // hash table
  struct symhashtable *outer_scope; // symhashtable in next outer scope
  int level;                        // level of scope, 0 is outermost
};

struct symtable {
  struct symhashtable *inner_scope; // pointer to hash table of innermost scope
  int num_nodes;                    // number of nodes in the innermost scope
};

/**
 * @brief new symtable
 */
struct symtable *symtable_new();

/**
 * @brief destroy and free a symtable
 */
void symtable_free(struct symtable *table);
