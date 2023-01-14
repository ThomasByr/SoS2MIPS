#pragma once

#include "protocol.h"
#include "vec.h"

/**
 * variable and return type
 * used for type checking
 */
enum vartype {
  no_type,
  inttype,
  doubletype,
  strtype,
  arrayinttype,
  voidtype,
};

/**
 * the thing that the identifier identifies
 * not applicable in the string constant table
 */
enum nodetype {
  func_node,
  val_node,
  array_node,
};

/**
 * the type of the memory address
 */
enum memaddrtype {
  off_fp,
  global,
};

struct symnode {
  char *name; // name in this scope

  enum vartype var_type; // the type of a variable or return type of a function
                         // (irrelevant for the string table)
  int var_size;          // size of a variable or return type of a function
                         // (irrelevant for the string table)

  int var_addr;      // assembly index for functions
                     // irrelevant for string constants
  enum reg reg_temp; // register for the result of assignment

  enum nodetype node_type; // the thing that the identifier identifies
                           // (irrelevant for the string table)
  enum memaddrtype mem_addr_type;
  int array_size; // only relevant for nodes of type array_node

  char *mangled_name; // irrelevant for the string table

  int num_vars;   // number of variables for a function
  int num_temps;  // number of temporary variables for a function
  int num_params; // number of formal parameters

  struct symnode *next;                 // next node in list
  struct symnode **param_symnode_array; // array of pointers to param symnodes
};

/**
 * @brief Does the identifier in this node equal name?
 *
 */
int name_is_equal(struct symnode *node, char *name);

struct symhashtable {
  int size;                         // size of hash table
  struct symnode **table;           // hash table
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
void symtable_free(struct symtable *symtab);

/**
 * @brief Insert an entry into the innermost scope of symbol table. First
   make sure it's not already in that scope. Return a pointer to the
   entry.
 * @param symtab the symbol table
 * @param name the name of the entry
 * @return struct symnode* - pointer to the entry
 */
struct symnode *symtable_insert(struct symtable *symtab, char *name);

/**
 * @brief Lookup an entry in a symbol table. If found return a pointer to it
   and fill in level. Otherwise, return NULL and level is
   undefined.
 * @param symtab the symbol table
 * @param name the name of the entry
 * @param level the level of the entry
 * @return struct symnode*
 */
struct symnode *symtable_lookup(struct symtable *symtab, char *name,
                                int *level);

/**
 * @brief Enter a new scope
 * @param symtab the symbol table
 */
void symtable_enter_scope(struct symtable *symtab);

/**
 * @brief Leave the innermost scope
 * @param symtab the symbol table
 */
void symtable_leave_scope(struct symtable *symtab);

/**
 * @brief Display the symbol table to a human readable format
 * @param symtab the symbol table
 */
void symtable_display(struct symtable *symtab);

/**
 * @brief Construct a new vec object of symnodes pointing to every symnode in
 * the symboltable.
 *
 * @param symtab the symbol table
 * @return vec_t - vector of symnodes
 */
vec_t symtable_vec(struct symtable *symtab);
