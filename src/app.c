#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "app.h"
#include "protocol.h"
#include "quad.h"
#include "symtable.h"
#include "threadpool.h"
#include "vec.h"

#include "lib.h"

// Used to look up unmangled IDs with scoping
struct symtable *scoped_id_table;
// Used to store IDs with mangled names
struct symtable *flat_id_table;
// Used throughout to save memory with string constants
struct symtable *stringconst_table;
// Used by the parser to save memory with ID names
struct symtable *id_name_table;
extern vec_t quad_array;

int error_count = 0;

extern struct symnode *main_func_symnode;

void discard_file(void *filename) {
  char *file = (char *)filename;
  info("removing %s", file);
  CHK(unlink(file));
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
  // one thread because we should be able to kill qtspim without exiting our app
  // but not the other way around (qtspim should not be able to kill our app)
  // two queued jobs max because we don't want to launch qtspim after the file
  // is removed
  // might work with only one queued job max, but this is safer
#define THREAD 1
#define QUEUE 2

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

  id_name_table = symtable_new();
  quad_array = vec_new();

  // print the token
  extern int yylex(void);
  int token;
  while ((token = yylex()) != 0) {
    printf("TOKEN : %d\n", token);
  }

  // launch yyparse
  if (yyparse() != 0) {
    printf("error\n");
    status = EXIT_FAILURE;
  }

  if (args->stdisplay) symtable_display(id_name_table);

  quad_vec_display();

  // launch qtspim
  switch (threadpool_add(pool, launch_qtspim, args->output, 0)) {
  case 0:
    break;
  case threadpool_invalid:
    panic("invalid threadpool (threadpool seems to be NULL)");
  case threadpool_lock_failure:
    panic("lock failure on threadpool");
  case threadpool_queue_full:
    panic("queue is full");
  case threadpool_shutdown:
    panic("threadpool is shutting down");
  default:
    panic("unknown error on threadpool");
  }

  if (args->dispose_on_exit) {
    switch (threadpool_add(pool, discard_file, args->output, 0)) {
    case 0:
      break;
    case threadpool_invalid:
      panic("invalid threadpool (threadpool seems to be NULL)");
    case threadpool_lock_failure:
      panic("lock failure on threadpool");
    case threadpool_queue_full:
      panic("queue is full");
    case threadpool_shutdown:
      panic("threadpool is shutting down");
    default:
      panic("unknown error on threadpool");
    }
  }

  // wait for tasks to finish (blocking)
  switch (threadpool_destroy(pool, 1)) {
  case 0:
    break;
  case threadpool_invalid:
    panic("invalid threadpool (threadpool seems to be NULL)");
  case threadpool_lock_failure:
    panic("lock failure on threadpool");
  case threadpool_shutdown:
    panic("threadpool is shutting down");
  default:
    panic("unknown error on threadpool");
  }

  CHK(fclose(yyin));
  CHK(fclose(yyout));

  generate_asm();

  return status;
}
