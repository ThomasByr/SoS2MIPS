#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "app.h"
#include "threadpool.h"

#include "lib.h"

void discard_file(void *filename) {
  char *f = (char *)filename;
  CHK(remove(f));
}

void launch_qtspim(void *filename) {
  int status;
  switch (fork()) {
  case -1:
    panic("fork on qtspim failed");
  case 0:
    execlp("qtspim", "qtspim", "-file", (char *)filename, NULL);
    panic("execlp on qtspim failed");
  }
  CHK(wait(&status));
  if (WIFEXITED(status)) {
    if (WEXITSTATUS(status) != EXIT_SUCCESS) {
      panic("qtspim exited with status %d", WEXITSTATUS(status));
    }
  } else if (WIFSIGNALED(status)) {
    panic("qtspim terminated by signal %d", WTERMSIG(status));
  } else {
    panic("qtspim terminated abnormally");
  }
}

int run_app(const struct cmd_args *args) {
#define THREAD 32
#define QUEUE 256

  int status = EXIT_SUCCESS;

  threadpool_t pool = threadpool_create(THREAD, QUEUE, 0);
  if (pool == NULL) panic("failed to create threadpool");

  extern FILE *yyin, *yyout;
  extern int yyparse(void);

  // open input and output files
  yyin = fopen(args->filename, "r");
  if (yyin == NULL) panic("failed to open input file");
  yyout = fopen(args->output, "w");
  if (yyout == NULL) panic("failed to open output file");

  // launch yyparse
  if (yyparse() != 0) {
    status = EXIT_FAILURE;
  }

  threadpool_add(pool, launch_qtspim, args->output, 0); // launch qtspim
  threadpool_destroy(pool, 1);                          // wait

  if (args->dispose_on_exit) {
    printf("removing %s\n", args->output);
    discard_file(args->output);
  }

  CHK(fclose(yyin));
  CHK(fclose(yyout));

  return status;
}
