#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "app.h"
#include "io.h"

#include "lib.h"

/* global variables for lex/yacc */

int opt_lvl;
bool verbose;
char *output;

int main(int argc, char *argv[]) {
  struct cmd_args args;
  cmd_args_init(&args);

  parse_args(argc, argv, &args);
  check_args(&args);
  print_args(&args);

  /* set global variables for lex/yacc */

  opt_lvl = args.opt_lvl;
  verbose = args.verbose;
  output = args.output;

  /* run the application */

  return run_app(&args);
}
