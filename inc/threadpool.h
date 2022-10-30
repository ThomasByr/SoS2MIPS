#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Increase these constants at your own risk
 * Large values might slow down your system
 */
#define MAX_THREADS 64
#define MAX_QUEUE 65536

typedef struct threadpool_s *threadpool_t;

typedef enum {
  threadpool_invalid = -1,
  threadpool_lock_failure = -2,
  threadpool_queue_full = -3,
  threadpool_shutdown = -4,
  threadpool_thread_failure = -5
} threadpool_error_t;

typedef enum { threadpool_graceful = 1 } threadpool_destroy_flags_t;

/**
 * @brief Creates a threadpool_t object
 *
 * @param thread_count number of worker threads
 * @param queue_size   size of the queue
 * @param flags        unused parameter
 * @return a newly created thread pool or NULL
 */
threadpool_t threadpool_create(int thread_count, int queue_size, int flags);

/**
 * @brief Adds a new task in the queue of a thread pool
 *
 * @param pool     thread pool to which add the task
 * @param function pointer to the function that will perform the task
 * @param argument argument to be passed to the function
 * @param flags    unused parameter
 * @return 0 if all goes well, negative values in case of error (@see
 * threadpool_error_t for codes).
 */
int threadpool_add(threadpool_t pool, void (*routine)(void *), void *arg,
                   int flags);

/**
 * @brief Stops and destroys a thread pool
 *
 * @param pool  thread pool to destroy
 * @param flags flags for shutdown
 *
 * Known values for flags are 0 (default) and threadpool_graceful in
 * which case the thread pool doesn't accept any new tasks but
 * processes all pending tasks before shutdown.
 */
int threadpool_destroy(threadpool_t pool, int flags);

#ifdef __cplusplus
}
#endif
