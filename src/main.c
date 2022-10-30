#include <stdio.h>
#include <stdlib.h>

#include "io.h"

#include "lib.h"

extern int yydebug;
extern int yyparse(void);

int main(int argc, char *argv[]) {
  cmd_args args = cmd_args_init();

  parse_args(argc, argv, args);
  check_args(args);

  cmd_args_free(args);

  // yydebug = 0;
  // int r = yyparse();
  return EXIT_SUCCESS;
}
