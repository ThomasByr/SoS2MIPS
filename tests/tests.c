#include <stdlib.h>
#include <stdbool.h>

#include "dict_test.h"
#include "symtable_test.h"
#include "threadpool_test.h"
#include "vec_test.h"

unsigned long _no_asserts = 0;

int opt_lvl = 0;
bool verbose = false;
char *output = NULL;

int main(void) {
  thpool_test();
  dict_test();
  vec_test();
  symtable_test();

  return EXIT_SUCCESS;
}
