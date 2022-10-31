#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>

#include "threadpool.h"

#include "lib.h"

typedef enum {
  immediate_shutdown = 1,
  graceful_shutdown = 2
} threadpool_shutdown_t;

/**
 * @struct threadpool_task
 * @brief the work struct
 *
 * @var function Pointer to the function that will perform the task.
 * @var argument Argument to be passed to the function.
 */
typedef struct {
  void (*function)(void *);
  void *argument;
} threadpool_task_t;

/**
 * @brief The threadpool struct
 *
 * @var notify       Condition variable to notify worker threads.
 * @var threads      Array containing worker threads ID.
 * @var thread_count Number of threads
 * @var queue        Array containing the task queue.
 * @var queue_size   Size of the task queue.
 * @var head         Index of the first element.
 * @var tail         Index of the next element.
 * @var count        Number of pending tasks
 * @var shutdown     Flag indicating if the pool is shutting down
 * @var started      Number of started threads
 */
struct threadpool_s {
  pthread_mutex_t lock;
  pthread_cond_t notify;
  pthread_t *threads;
  threadpool_task_t *queue;
  int thread_count;
  int queue_size;
  int head;
  int tail;
  int count;
  int shutdown;
  int started;
};

/**
 * @function void *threadpool_thread(void *threadpool)
 * @brief the worker thread
 * @param threadpool the pool which own the thread
 */
static void *threadpool_thread(void *threadpool);

int threadpool_free(threadpool_t pool);

threadpool_t threadpool_create(int thread_count, int queue_size, int flags) {
  threadpool_t pool;
  int i;
  (void)flags;

  if (thread_count <= 0 || thread_count > MAX_THREADS || queue_size <= 0 ||
      queue_size > MAX_QUEUE) {
    return NULL;
  }

  if ((pool = (struct threadpool_s *)malloc(sizeof(struct threadpool_s))) ==
      NULL) {
    goto err;
  }

  // initialize
  pool->thread_count = 0;
  pool->queue_size = queue_size;
  pool->head = pool->tail = pool->count = 0;
  pool->shutdown = pool->started = 0;

  // allocate thread and task queue
  pool->threads = (pthread_t *)malloc(sizeof(pthread_t) * thread_count);
  pool->queue =
      (threadpool_task_t *)malloc(sizeof(threadpool_task_t) * queue_size);

  // initialize mutex and conditional variable first
  if ((pthread_mutex_init(&(pool->lock), NULL) != 0) ||
      (pthread_cond_init(&(pool->notify), NULL) != 0) ||
      (pool->threads == NULL) || (pool->queue == NULL)) {
    goto err;
  }

  // start worker threads
  for (i = 0; i < thread_count; i++) {
    if (pthread_create(&(pool->threads[i]), NULL, threadpool_thread,
                       (void *)pool) != 0) {
      threadpool_destroy(pool, 0);
      return NULL;
    }
    pool->thread_count++;
    pool->started++;
  }

  return pool;

err:
  if (pool) {
    threadpool_free(pool);
  }
  return NULL;
}

int threadpool_add(threadpool_t pool, void (*function)(void *), void *argument,
                   int flags) {
  int err = 0;
  int next;
  (void)flags;

  if (pool == NULL || function == NULL) {
    return threadpool_invalid;
  }

  if (pthread_mutex_lock(&(pool->lock)) != 0) {
    return threadpool_lock_failure;
  }

  next = (pool->tail + 1) % pool->queue_size;

  do {
    // are we full ?
    if (pool->count == pool->queue_size) {
      err = threadpool_queue_full;
      break;
    }

    // are we shutting down ?
    if (pool->shutdown) {
      err = threadpool_shutdown;
      break;
    }

    // add task to queue
    pool->queue[pool->tail].function = function;
    pool->queue[pool->tail].argument = argument;
    pool->tail = next;
    pool->count += 1;

    // pthread_cond_broadcast
    if (pthread_cond_signal(&(pool->notify)) != 0) {
      err = threadpool_lock_failure;
      break;
    }
  } while (0);

  if (pthread_mutex_unlock(&pool->lock) != 0) {
    err = threadpool_lock_failure;
  }

  return err;
}

int threadpool_destroy(threadpool_t pool, int flags) {
  int i, err = 0;

  if (pool == NULL) {
    return threadpool_invalid;
  }

  if (pthread_mutex_lock(&(pool->lock)) != 0) {
    return threadpool_lock_failure;
  }

  do {
    // already shutting down
    if (pool->shutdown) {
      err = threadpool_shutdown;
      break;
    }

    pool->shutdown =
        (flags & threadpool_graceful) ? graceful_shutdown : immediate_shutdown;

    // wake up all worker threads
    if ((pthread_cond_broadcast(&(pool->notify)) != 0) ||
        (pthread_mutex_unlock(&(pool->lock)) != 0)) {
      err = threadpool_lock_failure;
      break;
    }

    // join all worker thread
    for (i = 0; i < pool->thread_count; i++) {
      if (pthread_join(pool->threads[i], NULL) != 0) {
        err = threadpool_thread_failure;
      }
    }
  } while (0);

  // only if everything went well do we deallocate the pool
  if (!err) {
    threadpool_free(pool);
  }
  return err;
}

int threadpool_free(threadpool_t pool) {
  if (pool == NULL || pool->started > 0) {
    return -1;
  }

  // did we manage to allocate ?
  if (pool->threads) {
    free(pool->threads);
    free(pool->queue);

    // because we allocate pool->threads after initializing the
    // mutex and condition variable, we're sure they're initialized
    // let's lock the mutex just in case
    pthread_mutex_lock(&(pool->lock));
    pthread_mutex_destroy(&(pool->lock));
    pthread_cond_destroy(&(pool->notify));
  }
  free(pool);
  return 0;
}

static void *threadpool_thread(void *threadpool) {
  threadpool_t pool = (threadpool_t)threadpool;
  threadpool_task_t task;

  for (;;) {
    // lock must be taken to wait on conditional variable
    pthread_mutex_lock(&(pool->lock));

    // wait on condition variable, check for spurious wakeups
    // when returning from pthread_cond_wait(), we own the lock
    while ((pool->count == 0) && (!pool->shutdown)) {
      pthread_cond_wait(&(pool->notify), &(pool->lock));
    }

    if ((pool->shutdown == immediate_shutdown) ||
        ((pool->shutdown == graceful_shutdown) && (pool->count == 0))) {
      break;
    }

    // grab our task
    task.function = pool->queue[pool->head].function;
    task.argument = pool->queue[pool->head].argument;
    pool->head = (pool->head + 1) % pool->queue_size;
    pool->count -= 1;

    // unlock
    pthread_mutex_unlock(&(pool->lock));

    // get to work
    (*(task.function))(task.argument);
  }

  pool->started--;

  pthread_mutex_unlock(&(pool->lock));
  pthread_exit(NULL);
  return (NULL);
}
