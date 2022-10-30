#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "m.h"

#include "threadpool.h"

#define THREAD 32
#define QUEUE 256

int tasks = 0, done = 0;
pthread_mutex_t lock;

void dummy_task(void *arg) {
  (void)arg;
  usleep(10000);
  pthread_mutex_lock(&lock);
  done++;
  pthread_mutex_unlock(&lock);
}

void thpool_test_0(void) {
  threadpool_t pool;

  pthread_mutex_init(&lock, NULL);

  assert_neq((pool = threadpool_create(THREAD, QUEUE, 0)), NULL);
  fprintf(stderr,
          "Pool started with %d threads and "
          "queue size of %d\n",
          THREAD, QUEUE);

  while (threadpool_add(pool, &dummy_task, NULL, 0) == 0) {
    pthread_mutex_lock(&lock);
    tasks++;
    pthread_mutex_unlock(&lock);
  }

  fprintf(stderr, "Added %d tasks\n", tasks);

  while ((tasks / 2) > done) {
    usleep(10000);
  }
  assert_eq(threadpool_destroy(pool, 0), 0);
  fprintf(stderr, "Did %d tasks\n", done);
}

void thpool_test(void) {
  test_case(thpool_test_0);
}
