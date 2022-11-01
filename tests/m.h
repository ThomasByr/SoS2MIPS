/* m.h
A simple assertion macro.
*   `test_case` macro is used to define a test case.
It should at least hold one of the `assert` macros.
Note that `assert` macros may not produce the desired output if not used in a
test case.
*   `assert` macro is used to check the truth evaluation of an expression.
If the expression evaluates to false, the program will be terminated with an
error
*   `assert_eq` macro is used to check the equality of two values.
This macro, unlike `assert`, tries to cast the values to the same type before
evaluation. The type of the first argument is arbitrarily chosen.
*   `assert_neq`, `assert_lt`, `assert_gt`, `assert_leq`, `assert_geq` macros
are used to check the inequality of two values.
*   only use `assert_info` when the evaluated expression does not expand to it's
original form (for example, `assert_info(_a == _b, a == b)` if `a` and `b` are
casted to the same type in `_a` and `_b`).
*   `cast_to_type` macro is used to cast two values to the same type.
As specified before, the type of the first argument is arbitrarily chosen.
This macro thus creates two temporary variables, `_a` and `_b` that will be
used to perform checks.
*/

#pragma once

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "lib.h"

extern unsigned long _no_asserts;

#define assert_info(expr, ...)                                                 \
  do {                                                                         \
    _no_asserts++;                                                             \
    if (!(expr)) {                                                             \
      fprintf(stderr, FG_RED "\nassertion failed: " RST "%s\n", #__VA_ARGS__); \
      abort();                                                                 \
    }                                                                          \
  } while (0);

#ifndef assert
#define assert(expr) assert_info(expr, expr);
#endif

#define cast_to_type(a, b) \
  __typeof__(a) _a = (a);  \
  __typeof__(a) _b = (b);

#define assert_eq(a, b)            \
  do {                             \
    cast_to_type(a, b);            \
    assert_info(_a == _b, a == b); \
  } while (0);
#define assert_neq(a, b)           \
  do {                             \
    cast_to_type(a, b);            \
    assert_info(_a != _b, a != b); \
  } while (0);
#define assert_lt(a, b)          \
  do {                           \
    cast_to_type(a, b);          \
    assert_info(_a < _b, a < b); \
  } while (0);
#define assert_gt(a, b)          \
  do {                           \
    cast_to_type(a, b);          \
    assert_info(_a > _b, a > b); \
  } while (0);
#define assert_leq(a, b)           \
  do {                             \
    cast_to_type(a, b);            \
    assert_info(_a <= _b, a <= b); \
  } while (0);
#define assert_geq(a, b)           \
  do {                             \
    cast_to_type(a, b);            \
    assert_info(_a >= _b, a >= b); \
  } while (0);

#ifndef QUIET
#define exe(f) \
  do {         \
    f();       \
  } while (0);
#else
#define exe(f)                                \
  do {                                        \
    int _stdout, _stderr, _null;              \
    CHK(_stdout = dup(1));                    \
    CHK(_stderr = dup(2));                    \
    CHK(_null = open("/dev/null", O_WRONLY)); \
    CHK(dup2(_null, 1));                      \
    CHK(dup2(_null, 2));                      \
    f();                                      \
    fflush(stdout);                           \
    fflush(stderr);                           \
    CHK(dup2(_stdout, 1));                    \
    CHK(dup2(_stderr, 2));                    \
    CHK(close(_null));                        \
  } while (0);
#endif

/**
 * @brief Macro used to define a test case on runtime
 *
 * @param fun function to run
 */
#define test_case(fun)                                                       \
  do {                                                                       \
    _no_asserts = 0;                                                         \
    fprintf(stderr, FG_BLU " running " RST "%-20s:: %-20s", __FILE__, #fun); \
    fflush(stderr);                                                          \
    exe(fun);                                                                \
    if (_no_asserts > 0) {                                                   \
      fprintf(stderr, FG_GRN "ok (%lu)\n" RST, _no_asserts);                 \
    } else {                                                                 \
      fprintf(stderr, FG_YEL "fake (0)\n" RST);                              \
    }                                                                        \
  } while (0);
