#pragma once

#include "dict.h"
#include "vec.h"

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

/**
 * @brief Insert an entry into the innermost scope of symbol table. First
   make sure it's not already in that scope. Return a pointer to the
   entry.
 * @param table the symbol table
 * @param name the name of the entry
 * @return struct symnode* - pointer to the entry
 */
struct symnode *symtable_insert(struct symtable *table, char *name);

/**
 * @brief Lookup an entry in a symbol table. If found return a pointer to it
   and fill in level. Otherwise, return NULL and level is
   undefined.
 * @param table the symbol table
 * @param name the name of the entry
 * @param level the level of the entry
 * @return struct symnode*
 */
struct symnode *symtable_lookup(struct symtable *table, char *name, int *level);

/**
 * @brief Enter a new scope
 * @param table the symbol table
 */
void symtable_enter_scope(struct symtable *table);

/**
 * @brief Leave the innermost scope
 * @param table the symbol table
 */
void symtable_leave_scope(struct symtable *table);

/**
 * @brief Display the symbol table to a human readable format
 * @param table the symbol table
 */
void symtable_display(struct symtable *table);

/**
 * @brief Construct a new vec object of symnodes pointing to every symnode in
 * the symboltable.
 *
 * @param table the symbol table
 * @return vec_t - vector of symnodes
 */
vec_t symtable_vec(struct symtable *table);
