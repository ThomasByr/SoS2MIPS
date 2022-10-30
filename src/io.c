#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include "io.h"

#include "lib.h"

typedef struct _io {
  char filename[BUFSIZ];
  char output[BUFSIZ];

  bool stdisplay;
  bool verbose;

  unsigned opt_lvl_set;
  bool opt_lvl;
} _io;

static noreturn void display_help(const char *restrict fmt, ...) {
  const int status = fmt == NULL ? EXIT_SUCCESS : EXIT_FAILURE;
  const char *restrict help =
      "SoS2MIPS - A Subscript of Shell to MIPS32 Assembly Compiler\n"
      "version " __VERSION__ " by " __AUTHORS__ "\n"
      "usage: sos2mips [options] -i file\n";

  if (fmt != NULL) {
    va_list args;
    va_start(args, fmt);
    alert(fmt, &args);
    va_end(args);
    printf("\n");
  }
  printf("%s", help);

  exit(status);
}

static noreturn void display_version(void) {
  printf("SoS2MIPS - A Subscript of Shell to MIPS32 Assembly Compiler\n"
         "version " __VERSION__ " by " __AUTHORS__ "\n");
  exit(EXIT_SUCCESS);
}

static noreturn void display_license(void) {
  const char *restrict l =
      "This project is licensed under the [GPL-3.0](LICENSE) license. "
      "Please see the [license](LICENSE) file for more "
      "details.\n\nRedistribution and use in source and binary forms, with "
      "or without\nmodification, are permitted provided that the following "
      "conditions are met:\n\n- Redistributions of source code must retain "
      "the above copyright notice,\n  this list of conditions and the "
      "following disclaimer.\n\n- Redistributions in binary form must "
      "reproduce the above copyright notice,\n  this list of conditions "
      "and the following disclaimer in the documentation\n  and/or other "
      "materials provided with the distribution.\n\n- Neither the name of "
      "the SoS2MIPS authors nor the names of its\n  contributors may be "
      "used to endorse or promote products derived from\n  this software "
      "without specific prior written permission.\n\nTHIS SOFTWARE IS "
      "PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS \"AS IS\""
      "\nAND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED "
      "TO, THE\nIMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A "
      "PARTICULAR PURPOSE\nARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT "
      "HOLDER OR CONTRIBUTORS BE\nLIABLE FOR ANY DIRECT, INDIRECT, "
      "INCIDENTAL, SPECIAL, EXEMPLARY, OR\nCONSEQUENTIAL DAMAGES "
      "(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF\nSUBSTITUTE GOODS OR "
      "SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS\nINTERRUPTION) "
      "HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER "
      "IN\nCONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR "
      "OTHERWISE)\nARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, "
      "EVEN IF ADVISED OF THE\nPOSSIBILITY OF SUCH DAMAGE.\n";
  printf("%s", l);
  exit(EXIT_SUCCESS);
}

cmd_args cmd_args_init(void) {
  struct _io *args = (struct _io *)malloc(sizeof(struct _io));
  memset(args->filename, '\0', BUFSIZ);
  memset(args->output, '\0', BUFSIZ);

  args->stdisplay = false;
  args->verbose = false;

  args->opt_lvl_set = 0;
  args->opt_lvl = false;

  return args;
}

void cmd_args_free(cmd_args args) { free(args); }

void parse_args(int argc, char *argv[], cmd_args args) {
#define TOS_OPT 1000
#define VERB_OPT 2000
#define O0_OPT 3000
#define O1_OPT 3001

  static const struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"version", no_argument, NULL, 'v'},
      {"license", no_argument, NULL, 'l'},
      {"in", required_argument, NULL, 'i'},
      {"out", required_argument, NULL, 'o'},
      {"tos", no_argument, NULL, TOS_OPT},
      {"verbose", no_argument, NULL, VERB_OPT},
      {"O0", no_argument, NULL, O0_OPT},
      {"O1", no_argument, NULL, O1_OPT},
      {NULL, 0, NULL, 0},
  };
  static const char short_options[] = "hvli:o:";

  int opt;
  char *bad_opt;
  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {
    switch (opt) {

    case 'h':
      display_help(NULL);
      panic("unreachable");
      break;

    case 'v':
      display_version();
      panic("unreachable");
      break;

    case 'l':
      display_license();
      panic("unreachable");
      break;

    case 'i':
      strncpy(args->filename, optarg, BUFSIZ);
      break;

    case 'o':
      strncpy(args->output, optarg, BUFSIZ);
      break;

    case TOS_OPT:
      args->stdisplay = true;
      break;

    case VERB_OPT:
      args->verbose = true;
      break;

    case O0_OPT | O1_OPT:
      args->opt_lvl_set ++;
      args->opt_lvl = (opt == O1_OPT);
      break;

    default:
      bad_opt = argv[optind - 1];
      display_help("unrecognized option '%s'", bad_opt);
      panic("unreachable");
      break;
    }

    if (optind < argc) {
      display_help("unrecognized argument '%s'", argv[optind]);
      panic("unreachable");
    }
  }
  optind = 0;
}

void check_args(const cmd_args args) {
  if (args->filename[0] == '\0') {
    display_help("no input file specified");
  }
  if (args->opt_lvl_set > 1) {
    display_help("optimization level can only be specified once");
  }
}
