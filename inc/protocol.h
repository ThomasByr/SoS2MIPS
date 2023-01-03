#pragma once

#include <stdio.h>

enum sys_call {
  sc_print_int = 1,    // $a0 = integer
  sc_print_float = 2,  // $f12 = float ($a0...$a3)
  sc_print_double = 3, // $f12 = double ($a0...$a3)
  sc_print_string = 4, // $a0 = string address

  sc_read_int = 5,    // -> $v0 = integer
  sc_read_float = 6,  // -> $f0 = float ($v0)
  sc_read_double = 7, // -> $f0 = double ($v0)
  sc_read_string = 8, // $a0 = string address, $a1 = max length

  sc_sbrk = 9, // $a0 = address -> $v0 = address

  sc_exit = 10,

  sc_print_character = 11, // $a0 = character
  sc_read_character = 12,  // -> $v0 = character

  sc_open = 13,  // $a0 = filename, $a1 = flags, $a2 = mode -> $v0 = fd
  sc_read = 14,  // $a0 = fd, $a1 = buf, $a2 = count -> $v0 = bytes read
  sc_write = 15, // $a0 = fd, $a1 = buf, $a2 = count -> $v0 = bytes written
  sc_close = 16, // $a0 = fd -> $v0 = 0 on success, -1 on error

  sc_exit2 = 17, // $a0 = exit code
};

enum reg {
  reg_zero = 0, // the hardwritten value 0
  reg_at = 1,   // reserved for assembler

  reg_v0 = 2, // expression evaluation and results of a function
  reg_v1 = 3, // expression evaluation and results of a function

  reg_a0 = 4, // function arguments 1
  reg_a1 = 5, // function arguments 2
  reg_a2 = 6, // function arguments 3
  reg_a3 = 7, // function arguments 4

  reg_t0 = 8,  // temporary registers (not preserved across function calls)
  reg_t1 = 9,  // temporary registers (not preserved across function calls)
  reg_t2 = 10, // temporary registers (not preserved across function calls)
  reg_t3 = 11, // temporary registers (not preserved across function calls)
  reg_t4 = 12, // temporary registers (not preserved across function calls)
  reg_t5 = 13, // temporary registers (not preserved across function calls)
  reg_t6 = 14, // temporary registers (not preserved across function calls)
  reg_t7 = 15, // temporary registers (not preserved across function calls)

  reg_s0 = 16, // saved registers (preserved across function calls)
  reg_s1 = 17, // saved registers (preserved across function calls)
  reg_s2 = 18, // saved registers (preserved across function calls)
  reg_s3 = 19, // saved registers (preserved across function calls)
  reg_s4 = 20, // saved registers (preserved across function calls)
  reg_s5 = 21, // saved registers (preserved across function calls)
  reg_s6 = 22, // saved registers (preserved across function calls)
  reg_s7 = 23, // saved registers (preserved across function calls)

  reg_t8 = 24, // temporary registers (not preserved across function calls)
  reg_t9 = 25, // temporary registers (not preserved across function calls)

  reg_k0 = 26, // reserved for OS kernel
  reg_k1 = 27, // reserved for OS kernel

  reg_gp = 28, // global pointer
  reg_sp = 29, // stack pointer
  reg_fp = 30, // frame pointer
  reg_ra = 31, // return address
};

// Iterate through the quad_array and generates assembly for each quad,
// filling out the quad_assembly_lines array accordingly
void generate_asm(FILE *out);
