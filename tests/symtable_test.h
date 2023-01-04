#include <stdio.h>

#include "lib.h"
#include "m.h"

#include "symtable.h"

void symtable_test_0(void) {
  struct symtable *symtable = symtable_new();
  assert_neq(symtable, NULL);
  symtable_free(symtable);
}

void symtable_test_1(void) {
  struct symtable *symtable = symtable_new();
  symtable_insert(symtable, "a");
  symtable_insert(symtable, "b");

  int level;
  assert_neq(symtable_lookup(symtable, "a", &level), NULL);
  assert_eq(level, 0);
  assert_neq(symtable_lookup(symtable, "b", &level), NULL);
  assert_eq(level, 0);

  symtable_enter_scope(symtable);
  symtable_insert(symtable, "c");
  symtable_insert(symtable, "d");

  assert_neq(symtable_lookup(symtable, "a", &level), NULL);
  assert_eq(level, 0);
  assert_neq(symtable_lookup(symtable, "b", &level), NULL);
  assert_eq(level, 0);
  assert_neq(symtable_lookup(symtable, "c", &level), NULL);
  assert_eq(level, 1);
  assert_neq(symtable_lookup(symtable, "d", &level), NULL);
  assert_eq(level, 1);

  symtable_leave_scope(symtable);
  symtable_insert(symtable, "e");
  symtable_insert(symtable, "f");

  assert_neq(symtable_lookup(symtable, "a", &level), NULL);
  assert_eq(level, 0);
  assert_neq(symtable_lookup(symtable, "b", &level), NULL);
  assert_eq(level, 0);
  assert_neq(symtable_lookup(symtable, "e", &level), NULL);
  assert_eq(level, 0);
  assert_neq(symtable_lookup(symtable, "f", &level), NULL);
  assert_eq(level, 0);

  assert_eq(symtable_lookup(symtable, "c", &level), NULL);
  assert_eq(symtable_lookup(symtable, "d", &level), NULL);

  symtable_free(symtable);
}

void symtable_test_2(void) {
  struct symtable *symtable = symtable_new();

  struct symnode *node_a = symtable_insert(symtable, "a");
  node_a->node_type = val_node;
  node_a->var_type = inttype;
  node_a->mem_addr_type = global;
  node_a->var_addr = 0;

  symtable_enter_scope(symtable);

  struct symnode *node_b = symtable_insert(symtable, "b");
  node_b->node_type = val_node;
  node_b->var_type = inttype;
  node_b->mem_addr_type = off_fp;
  node_b->var_addr = 0;
  symtable_display(symtable); // to check if we have valid read/write

  assert_eq(symtable_lookup(symtable, "a", NULL), node_a);
  assert_eq(symtable_lookup(symtable, "b", NULL), node_b);

  symtable_leave_scope(symtable);

  assert_eq(symtable_lookup(symtable, "a", NULL), node_a);
  assert_eq(symtable_lookup(symtable, "b", NULL), NULL);

  symtable_free(symtable);
}

void symtable_test(void) {
  test_case(symtable_test_0);
  test_case(symtable_test_1);
  test_case(symtable_test_2);
}
