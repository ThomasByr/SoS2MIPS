#include <stdlib.h>

#include "dict_test.h"
#include "threadpool_test.h"
#include "vec_test.h"

unsigned long _no_asserts = 0;

int main(void) {
  thpool_test();
  dict_test();
  vec_test();

  return EXIT_SUCCESS;
}
