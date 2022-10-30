#pragma once

#include <stdarg.h>

typedef struct _io *cmd_args;

/**
 * @brief Initialize the command line arguments structure.
 * 
 * @return cmd_args - command line arguments structure
 */
cmd_args cmd_args_init(void);

/**
 * @brief Free the command line arguments underlying structure.
 * 
 * @param args - command line arguments structure
*/
void cmd_args_free(cmd_args args);

/**
 * @brief Read from the command line arguments and store them in a struct
 *
 * @param argc number of arguments
 * @param argv array of arguments
 * @param args struct to store the arguments
 */
void parse_args(int argc, char *argv[], cmd_args args);

/**
 * @brief Check if the arguments are valid
 *
 * @param args struct containing the arguments
 */
void check_args(const cmd_args args);
