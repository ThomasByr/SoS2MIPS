#pragma once

#include <stdarg.h>
#include <stdbool.h>

struct cmd_args {
  char *filename;
  char *output;

  bool dispose_on_exit;

  bool stdisplay;
  bool verbose;
  bool no_exe;

  int opt_lvl;
};

/**
 * @brief Initialize the command line arguments structure.
 *
 * @param args command line arguments structure
 */
void cmd_args_init(struct cmd_args *args);

/**
 * @brief Read from the command line arguments and store them in a struct
 *
 * @param argc number of arguments
 * @param argv array of arguments
 * @param args struct to store the arguments
 */
void parse_args(int argc, char *argv[], struct cmd_args *args);

/**
 * @brief Check if the arguments are valid
 *
 * @param args struct containing the arguments
 */
void check_args(const struct cmd_args * args);

/**
 * @brief Print command line arguments content
 *
 * @param args - command line arguments structure
 */
void print_args(const struct cmd_args *args);
