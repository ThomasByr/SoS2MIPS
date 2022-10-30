#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>

#include "lib.h"

void _display(const char *restrict fmt, va_list *ap) {
  vfprintf(stderr, fmt, *ap);
  fprintf(stderr, "\n");
}

void alert(const char *restrict fmt, ...) {
  fprintf(stderr, FG_YEL " alert: " RST);
  va_list args;
  va_start(args, fmt);
  _display(fmt, &args);
  va_end(args);
}

void debug(const char *restrict fmt, ...) {
#ifndef DEBUG
  (void)fmt;
#else
  fprintf(stderr, FG_GRN " debug: " RST);
  va_list args;
  va_start(args, fmt);
  _display(fmt, &args);
  va_end(args);
#endif
}

void info(const char *restrict fmt, ...) {
  fprintf(stderr, FG_BLU "  info: " RST);
  va_list args;
  va_start(args, fmt);
  _display(fmt, &args);
  va_end(args);
}
