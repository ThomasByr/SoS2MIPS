#pragma once

enum sys_call {
  print_int = 1,    // $a0 = integer
  print_float = 2,  // $f12 = float ($a0...$a3)
  print_double = 3, // $f12 = double ($a0...$a3)
  print_string = 4, // $a0 = string address

  read_int = 5,    // -> $v0 = integer
  read_float = 6,  // -> $f0 = float ($v0)
  read_double = 7, // -> $f0 = double ($v0)
  read_string = 8, // $a0 = string address, $a1 = max length

  sbrk = 9, // $a0 = address -> $v0 = address

  exit = 10,

  print_character = 11, // $a0 = character
  read_character = 12,  // -> $v0 = character

  open = 13,  // $a0 = filename, $a1 = flags, $a2 = mode -> $v0 = fd
  read = 14,  // $a0 = fd, $a1 = buf, $a2 = count -> $v0 = bytes read
  write = 15, // $a0 = fd, $a1 = buf, $a2 = count -> $v0 = bytes written
  close = 16, // $a0 = fd -> $v0 = 0 on success, -1 on error

  exit2 = 17, // $a0 = exit code
};

enum reg {
  zero = 0, // the hardwritten value 0
  at = 1,   // reserved for assembler

  v0 = 2, // expression evaluation and results of a function
  v1 = 3, // expression evaluation and results of a function

  a0 = 4, // function arguments 1
  a1 = 5, // function arguments 2
  a2 = 6, // function arguments 3
  a3 = 7, // function arguments 4

  t0 = 8,  // temporary registers (not preserved across function calls)
  t1 = 9,  // temporary registers (not preserved across function calls)
  t2 = 10, // temporary registers (not preserved across function calls)
  t3 = 11, // temporary registers (not preserved across function calls)
  t4 = 12, // temporary registers (not preserved across function calls)
  t5 = 13, // temporary registers (not preserved across function calls)
  t6 = 14, // temporary registers (not preserved across function calls)
  t7 = 15, // temporary registers (not preserved across function calls)

  s0 = 16, // saved registers (preserved across function calls)
  s1 = 17, // saved registers (preserved across function calls)
  s2 = 18, // saved registers (preserved across function calls)
  s3 = 19, // saved registers (preserved across function calls)
  s4 = 20, // saved registers (preserved across function calls)
  s5 = 21, // saved registers (preserved across function calls)
  s6 = 22, // saved registers (preserved across function calls)
  s7 = 23, // saved registers (preserved across function calls)

  t8 = 24, // temporary registers (not preserved across function calls)
  t9 = 25, // temporary registers (not preserved across function calls)

  k0 = 26, // reserved for OS kernel
  k1 = 27, // reserved for OS kernel

  gp = 28, // global pointer
  sp = 29, // stack pointer
  fp = 30, // frame pointer
  ra = 31, // return address
};
