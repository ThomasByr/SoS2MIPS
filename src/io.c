#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

#include "io.h"

#include "lib.h"

static noreturn void display_help(const char *restrict fmt, ...) {
  const int status = fmt == NULL ? EXIT_SUCCESS : EXIT_FAILURE;
  const char *restrict help =
      "SoS2MIPS - A Subscript of Shell to MIPS32 Assembly Compiler\n"
      "version " __VERSION__ " by " __AUTHORS__ "\n"
      "usage: sos2mips [options] -i <filename>\n"
      "options:\n"
      "  -h, --help\t\tDisplay this help message and exit\n"
      "  -v, --version\t\tDisplay the version of this program and exit\n"
      "  -l, --license\t\tDisplay the license of this program and exit\n"
      "  -i, --in\t\tSpecify the input file\n"
      "  -o, --out\t\tSpecify the output file\n"
      "  .., --tos\t\tDisplay the Symbol Table on runtime\n"
      "  .., --verbose\t\tDisplay verbose information on runtime\n"
      "  -O, --optlvl\t\tSet the optimization level\n"
      "the input and output files can be non-option arguments\n"
      "but are taken in order of appearance\n";

  if (fmt != NULL) {
    fprintf(stderr, FG_YEL " alert: " RST);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    va_end(ap);
    fprintf(stderr, "\n\n");
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

void cmd_args_init(struct cmd_args *args) {
  // struct _io *args = (struct _io *)malloc(sizeof(struct _io));
  args->filename = NULL;
  args->output = "a.s";

  args->dispose_on_exit = true;

  args->stdisplay = false;
  args->verbose = false;
  args->no_exe = false;

  args->opt_lvl = 0;

  // return args;
}

void parse_args(int argc, char *argv[], struct cmd_args *args) {
#define TOS_OPT 1000
#define VERB_OPT 2000
#define NOEXE_OPT 3000

  static const struct option long_options[] = {
      {"help", no_argument, NULL, 'h'},
      {"version", no_argument, NULL, 'v'},
      {"license", no_argument, NULL, 'l'},
      {"in", required_argument, NULL, 'i'},
      {"out", required_argument, NULL, 'o'},
      {"tos", no_argument, NULL, TOS_OPT},
      {"verbose", no_argument, NULL, VERB_OPT},
      {"no-exe", no_argument, NULL, NOEXE_OPT},
      {"optlvl", required_argument, NULL, 'O'},
      {NULL, 0, NULL, 0},
  };
  static const char short_options[] = ":i:o:O:hvl";

  extern int opterr;
  opterr = 0;

  int opt;
  while ((opt = getopt_long(argc, argv, short_options, long_options, NULL)) !=
         -1) {

    switch (opt) {

    case 'h':
      display_help(NULL);
      unreachable();
      break;

    case 'v':
      display_version();
      unreachable();
      break;

    case 'l':
      display_license();
      unreachable();
      break;

    case 'i':
      args->filename = optarg;
      break;

    case 'o':
      args->output = optarg;
      args->dispose_on_exit = false;
      break;

    case TOS_OPT:
      args->stdisplay = true;
      break;

    case VERB_OPT:
      args->verbose = true;
      break;

    case NOEXE_OPT:
      args->no_exe = true;
      break;

    case 'O':
      args->opt_lvl = strtoi(optarg);
      break;

    case '?':
      display_help("unrecognized option '%s'", argv[optind - 1]);
      unreachable();
      break;

    case ':':
      display_help("option '%s' requires an argument", argv[optind - 1]);
      unreachable();
      break;

    default:
      unreachable();
    }
  }
  if (optind < argc) /* non-option arguments remain */ {
    if (args->filename == NULL) {
      args->filename = argv[optind];
      optind++;
    }
    if (optind < argc) {
      args->output = argv[optind];
      args->dispose_on_exit = false;
      optind++;
    }

    // check if there are more arguments
    for (int i = optind; i < argc; i++) {
      alert("ignoring extra argument: %s", argv[i]);
    }
  }
  optind = 0;
}

void check_args(const struct cmd_args *args) {
  if (args->filename == NULL) {
    display_help("no input file specified");
  }
  if (args->opt_lvl < 0 || args->opt_lvl > 1) {
    display_help("invalid optimization level");
  }
  if (args->no_exe && args->dispose_on_exit) {
    display_help("can't run in no-exe mode without output file");
  }
}

void print_args(const struct cmd_args *args) {
  debug("filename: %s", args->filename);
  debug("output: %s", args->output);
  debug("dispose_on_exit: %s", args->dispose_on_exit ? "true" : "false");
  debug("stdisplay: %s", args->stdisplay ? "true" : "false");
  debug("verbose: %s", args->verbose ? "true" : "false");
  debug("no_exe: %s", args->no_exe ? "true" : "false");
  debug("opt_lvl: %d", args->opt_lvl);
}
