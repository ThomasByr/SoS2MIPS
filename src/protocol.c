#include <stdbool.h>

#include "protocol.h"

extern int opt_lvl;
extern bool verbose;
extern char *output;

static const char sys_call_names[][16] = {
    "print_int", "print_float", "print_double",    "print_string",
    "read_int",  "read_float",  "read_double",     "read_string",
    "sbrk",      "exit",        "print_character", "read_character",
    "open",      "read",        "write",           "close",
    "exit2",
};

static const char reg_names[][4] = {
    "$0",  "$at", "$v0", "$v1", "$a0", "$a1", "$a2", "$a3", "$t0", "$t1", "$t2",
    "$t3", "$t4", "$t5", "$t6", "$t7", "$s0", "$s1", "$s2", "$s3", "$s4", "$s5",
    "$s6", "$s7", "$t8", "$t9", "$k0", "$k1", "$gp", "$sp", "$fp", "$ra",
};

const char *reg_name(enum reg reg) { return reg_names[reg]; }

const char *sys_call_name(enum sys_call sys_call) {
  return sys_call_names[sys_call];
}
