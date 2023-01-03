#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "symtable.h"

#define NOHASHSLOT -1

const char var_type_string[][8] = {
    "unknown", "int", "float", "string", "bool", "void", "array",
};

const char node_type_string[][11] = {
    "func_node",
    "val_node",
    "array_node",
};

const char mem_addr_type_string[][7] = {
    "local",
    "global",
};

/* symnode */

struct symnode *main_func_symnode;

static struct symnode *symnode_new(char *name, int num_nodes) {
  struct symnode *node = malloc(sizeof(struct symnode));
  node->name = strdup(name);
  node->next = NULL;

  int digit_counter_num = num_nodes / 10;
  int num_digits = 1;
  while (digit_counter_num > 0) {
    num_digits++;
    digit_counter_num /= 10;
  }
  int mangled_name_len = strlen(name) + num_digits + 1;
  node->mangled_name = calloc(mangled_name_len + 1, sizeof(char));
  snprintf_s(node->mangled_name, mangled_name_len + 1, "%d$%s", num_nodes,
             name);

  return node;
}

static void symnode_free(struct symnode *node) {
  free(node->name);
  free(node->mangled_name);
  free(node);
}

int name_is_equal(struct symnode *node, char *name) {
  return !strcmp(node->name, name);
}

/* symhashtable */

static struct symhashtable *symhashtable_new(int entries) {
  struct symhashtable *hashtable = malloc(sizeof(struct symhashtable));
  hashtable->size = entries;
  hashtable->table = calloc(entries, sizeof(struct symnode *));

  int i;
  for (i = 0; i < entries; i++) {
    hashtable->table[i] = NULL;
  }

  hashtable->outer_scope = NULL;
  hashtable->level = -1;

  return hashtable;
}

static void symhashtable_free(struct symhashtable *hashtable) {
  int i;
  for (i = 0; i < hashtable->size; i++) {
    struct symnode *node, *next;

    for (node = hashtable->table[i]; node != NULL; node = next) {
      next = node->next;
      symnode_free(node);
    }
  }

  free(hashtable->table);
  free(hashtable);
}

/**
 * @brief Peter Weinberger's hash function, from Aho, Sethi, & Ullman p. 436.
 *
 */
static int hashPJW(char *s, int size) {
  unsigned h = 0, g;
  char *p;

  for (p = s; *p != '\0'; p++) {
    h = (h << 4) + *p;
    if ((g = (h & 0xf0000000)) != 0) h ^= (g >> 24) ^ g;
  }

  return h % size;
}

/**
 * @brief Look up an entry in a symhashtable, returning either a pointer to
   the symnode for the entry or NULL.  slot is where to look;
   if slot == NOHASHSLOT, then apply the hash function to figure it out.
 *
 */
static struct symnode *symhashtable_lookup(struct symhashtable *hashtable,
                                           char *name, int slot) {
  struct symnode *node;
  if (slot == NOHASHSLOT) {
    slot = hashPJW(name, hashtable->size);
  }
  // clang-format off
  for (node = hashtable->table[slot];
       node != NULL && !name_is_equal(node, name);
       node = node->next) ;
  // clang-format on
  return node;
}

static struct symnode *symhashtable_insert(struct symhashtable *hashtable,
                                           char *name, int *num_nodes_addr) {
  int slot = hashPJW(name, hashtable->size);
  struct symnode *node = symhashtable_lookup(hashtable, name, slot);

  if (node == NULL) {
    node = symnode_new(name, *num_nodes_addr);
    (*num_nodes_addr)++;
    node->next = hashtable->table[slot];
    hashtable->table[slot] = node;
  }
  return node;
}

/* symtable */

static const int HASHSIZE = 211;

struct symtable *symtable_new() {
  struct symtable *table = malloc(sizeof(struct symtable));
  table->num_nodes = 0;
  table->inner_scope = symhashtable_new(HASHSIZE);
  table->inner_scope->outer_scope = NULL;
  table->inner_scope->level = 0;
  return table;
}

void symtable_free(struct symtable *symtab) {
  struct symhashtable *table, *outer;

  for (table = symtab->inner_scope; table != NULL; table = outer) {
    outer = table->outer_scope;
    symhashtable_free(table);
  }

  free(symtab);
}

struct symnode *symtable_insert(struct symtable *symtab, char *name) {
  if (symtab->inner_scope == NULL) {
    panic("inserting into an empty symbol table");
  }

  struct symnode *node =
      symhashtable_lookup(symtab->inner_scope, name, NOHASHSLOT);
  if (node == NULL) {
    node = symhashtable_insert(symtab->inner_scope, name, &symtab->num_nodes);
  }
  return node;
}

struct symnode *symtable_lookup(struct symtable *symtab, char *name,
                                int *level) {
  struct symnode *node;
  struct symhashtable *hashtab;

  for (node = NULL, hashtab = symtab->inner_scope;
       node == NULL && hashtab != NULL; hashtab = hashtab->outer_scope) {
    node = symhashtable_lookup(hashtab, name, NOHASHSLOT);
    *level = hashtab->level;
  }
  return node;
}

void symtable_enter_scope(struct symtable *symtab) {
  struct symhashtable *table = symhashtable_new(HASHSIZE);
  table->outer_scope = symtab->inner_scope;
  table->level = symtab->inner_scope->level + 1;
  symtab->inner_scope = table;
}

void symtable_leave_scope(struct symtable *symtab) {
  struct symhashtable *table = symtab->inner_scope;
  symtab->inner_scope = table->outer_scope;
  symhashtable_free(table);
}

void symnode_display(struct symnode *node) {
  if (node) {
    info("Node: %s, type: %s, vartype: %s, addr: %d, addr type: %s, ptr: %p",
         node->name, node_type_string[node->node_type],
         var_type_string[node->var_type], node->var_addr,
         mem_addr_type_string[node->mem_addr_type], node);
  } else {
    info("Node is NULL");
  }
}

void symhashtable_display(struct symhashtable *hashtab) {
  int i;
  for (i = 0; i < hashtab->size; i++) {
    struct symnode *node;
    for (node = hashtab->table[i]; node != NULL; node = node->next) {
      symnode_display(node);
    }
  }
}

void symtable_display(struct symtable *symtab) {
  struct symhashtable *hashtab;
  for (hashtab = symtab->inner_scope; hashtab != NULL;
       hashtab = hashtab->outer_scope) {
    info("Table at level %d contains:", hashtab->level);
    symhashtable_display(hashtab);
  }
}

vec_t symtable_vec(struct symtable *symtab) {
  struct symhashtable *hashtab;
  vec_t vec = vec_new();

  for (hashtab = symtab->inner_scope; hashtab != NULL;
       hashtab = hashtab->outer_scope) {
    int i;
    for (i = 0; i < hashtab->size; i++) {
      struct symnode *node;
      for (node = hashtab->table[i]; node != NULL; node = node->next) {
        vec_push(vec, node);
      }
    }
  }
  return vec;
}
