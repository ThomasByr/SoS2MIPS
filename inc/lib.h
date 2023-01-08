#pragma once

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdlib.h>

#define __VERSION__ "1.6.9"
#define __AUTHORS__ "ThomasByr & ThomasD & EthanH & MathieuM"

#define RST "\x1b[m\x1b[0m"

#define FG_RED "\x1b[0;31m"
#define FG_GRN "\x1b[0;32m"
#define FG_YEL "\x1b[0;33m"
#define FG_BLU "\x1b[0;34m"
#define FG_MAG "\x1b[0;35m"
#define FG_CYN "\x1b[0;36m"
#define FG_WHT "\x1b[0;37m"

#define BG_BLU "\x1b[40m"
#define BG_RED "\x1b[41m"
#define BG_GRN "\x1b[42m"
#define BG_ORA "\x1b[43m"
#define BG_CYN1 "\x1b[44m"
#define BG_YEL "\x1b[45m"
#define BG_CYN2 "\x1b[46m"
#define BG_WHT "\x1b[47m"

extern int errno;

/**
 * @brief Check if the given operation succeeded
 * (i.e. did not return -1)
 *
 * @param op operation to check
 * @return does not return if the operation failed
 */
#define CHK(op)                                                           \
  do {                                                                    \
    if ((op) == -1) {                                                     \
      fprintf(stderr, FG_RED "   CHK: thread %zu on %s:%d\n     -> " RST, \
              pthread_self(), __FILE__, __LINE__);                        \
      fprintf(stderr, #op);                                               \
      fprintf(stderr, "\n");                                              \
      if (errno) {                                                        \
        perror(FG_RED "     -> " RST);                                    \
        fprintf(stderr, "\n");                                            \
      }                                                                   \
      abort();                                                            \
    }                                                                     \
  } while (0)

/**
 * @brief Check if the given pthread operation succeeded
 * (i.e. did not return > 0)
 *
 * @param op operation to check
 * @return does not return if the operation failed
 */
#define TCHK(op)                                                          \
  do {                                                                    \
    if ((errno = (op)) > 0) {                                             \
      fprintf(stderr, FG_RED "  TCHK: thread %zu on %s:%d\n     -> " RST, \
              pthread_self(), __FILE__, __LINE__);                        \
      fprintf(stderr, #op);                                               \
      fprintf(stderr, "\n");                                              \
      perror(FG_RED "     -> " RST);                                      \
      fprintf(stderr, "\n");                                              \
      abort();                                                            \
    }                                                                     \
  } while (0)

/**
 * @brief Causes the calling thread to panic (i.e. abort) with a message.
 *
 * @param fmt format string w/o trailing newline
 * @param ... format arguments
 * @return noreturn
 */
#define panic(fmt, ...)                                                 \
  do {                                                                  \
    fprintf(stderr, FG_RED " panic: thread %zu on %s:%d\n     -> " RST, \
            pthread_self(), __FILE__, __LINE__);                        \
    fprintf(stderr, fmt, ##__VA_ARGS__);                                \
    fprintf(stderr, "\n");                                              \
    if (errno) {                                                        \
      perror(FG_RED "     -> " RST);                                    \
      fprintf(stderr, "\n");                                            \
    }                                                                   \
    abort();                                                            \
  } while (0)

/**
 * @brief Causes the calling thread to panic (i.e. abort) if
 * the code falls through this point.
 * @return noreturn
 */
#define unreachable()                                                   \
  do {                                                                  \
    fprintf(stderr, FG_RED " panic: thread %zu on %s:%d\n     -> " RST, \
            pthread_self(), __FILE__, __LINE__);                        \
    fprintf(stderr, "unreachable code\n");                              \
    abort();                                                            \
  } while (0)

/**
 * @brief Displays an alert message.
 *
 * @param fmt format string w/o trailing newline
 * @param ... format arguments
 */
void alert(const char *restrict fmt, ...);
/**
 * @brief Displays a debug message (only if DEBUG is defined).
 *
 * @param fmt format string w/o trailing newline
 * @param ... format arguments
 */
void debug(const char *restrict fmt, ...);
/**
 * @brief Displays an info message.
 *
 * @param fmt format string w/o trailing newline
 * @param ... format arguments
 */
void info(const char *restrict fmt, ...);

typedef void *(action_callback_t)(void *);
typedef void *(for_each_callback_t)(void *, void *);
typedef void(delete_callback_t)(void *);
typedef void(print_callback_t)(void *);

#ifndef DEBUG
#define ASSERT(x) (void)(x)
#else
#define ASSERT(x) assert(x)
#endif

/**
 * @brief Performs a string to int conversion
 *
 * @param nptr string to convert
 * @return int - the converted value
 */
int strtoi(const char *restrict nptr);

/**
 * @brief Safe call to snprintf
 *
 * @param str  string to write to
 * @param size size of the string
 * @param fmt  formated message to print
 */
void snprintf_s(char *restrict str, size_t size, const char *restrict fmt, ...);

/**
 * @brief Safe call to vsnprintf
 *
 * @param str  string to write to
 * @param size size of the string
 * @param fmt  formated message to print
 * @param ap   va_list of arguments
 */
void vsnprintf_s(char *restrict str, size_t size, const char *restrict fmt,
                 va_list ap);

/**
 * @brief Safe call to memcpy
 *
 * @param dst0   destination to copy to
 * @param src0   source to copy from
 * @param length number of bytes to copy
 */
void *memcpy_s(void *restrict dst0, const void *restrict src0, size_t length);

/**
 * @brief Copies the string pointed to by src, including the terminating null
 * byte ('\0')
 *
 * @param dest destination string
 * @param src  source string
 * @param n    number of characters to copy
 * @return size_t - the number of characters copied, not including the
 * terminating null byte
 */
size_t strlcpy(char *restrict dst, const char *restrict src, size_t siz);

/**
 * @brief Suppress leading and trailing whitespaces
 *
 * @param str string to trim
 */
void trim(char *str);
