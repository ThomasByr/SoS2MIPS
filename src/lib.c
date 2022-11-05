#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int strtoi(const char *restrict nptr) {
  char *endptr;
  int x = strtol(nptr, &endptr, 10);

  if (endptr == nptr || *endptr != '\0') {
    panic("must be a number");
  } // if not a number
  if (errno == ERANGE) {
    panic("out of range [%ld, %ld]", LONG_MIN, LONG_MAX);
  } // if out of range of long (might be a problem with implicit cast)
  return x;
}

void snprintf_s(char *restrict str, size_t size, const char *restrict fmt,
                ...) {
  int n;
  va_list ap;

  va_start(ap, fmt);
  n = vsnprintf(str, size, fmt, ap);
  va_end(ap);

  if (n < 0) panic("vsnprintf failure");
  if ((size_t)n >= size) panic("format string too long");
}

typedef long word; // "word" used for optimal copy speed

#define wsize sizeof(word)
#define wmask (wsize - 1)

void *memcpy_s(void *restrict dst0, const void *restrict src0, size_t length) {
  char *dst = dst0;
  const char *src = src0;

  if (length == 0 || dst == src) return dst;
  size_t t;

  /** macros : loop-t-times ; and loop-t-times, t>0 */
  // clang-format off
#define	TLOOP(s) if (t) TLOOP1(s) 
#define	TLOOP1(s) do { s; } while (--t)
  // clang-format on

  if ((unsigned long)dst < (unsigned long)src) {
    // copy forward
    t = (uintptr_t)src; // only need low bits
    if ((t | (uintptr_t)dst) & wmask) {
      // try to align operands
      // this cannot be done unless the low bits match
      if ((t ^ (uintptr_t)dst) & wmask || length < wsize)
        t = length;
      else
        t = wsize - (t & wmask);
      length -= t;
      TLOOP1(*dst++ = *src++);
    }
    // copy whole words, then mop up any trailing bytes
    t = length / wsize;
    TLOOP(*(word *)dst = *(word *)src; src += wsize; dst += wsize);
    t = length & wmask;
    TLOOP(*dst++ = *src++);
  } else {
    // copy backwards
    // otherwise essentially the same
    // alignment works as before, except that it takes
    // (t&wmask) bytes to align, not wsize-(t&wmask)
    src += length;
    dst += length;
    t = (uintptr_t)src;
    if ((t | (uintptr_t)dst) & wmask) {
      if ((t ^ (uintptr_t)dst) & wmask || length <= wsize)
        t = length;
      else
        t &= wmask;
      length -= t;
      TLOOP1(*--dst = *--src);
    }
    t = length / wsize;
    TLOOP(src -= wsize; dst -= wsize; *(word *)dst = *(word *)src);
    t = length & wmask;
    TLOOP(*--dst = *--src);
  }
  return dst0;
}

size_t strlcpy(char *restrict dst, const char *restrict src, size_t siz) {
  char *d = dst;
  const char *s = src;
  size_t n = siz;

  if (n != 0) {
    while (--n != 0) {
      if ((*d++ = *s++) == '\0') break;
    }
  }

  if (n == 0) {
    if (siz != 0) *d = '\0';
    while (*s++) {
    }
  }
  return s - src - 1;
}

void trim(char *str) {
  char *end;

  while (isspace(*str)) {
    str++;
  }

  if (*str == 0) return;

  end = str + strlen(str) - 1;
  while (end > str && isspace(*end)) {
    end--;
  }

  *(end + 1) = '\0';
}
