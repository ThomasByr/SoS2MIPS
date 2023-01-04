#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lib.h"
#include "m.h"

#include "threadpool.h"

#define THREAD 32
#define QUEUE 256

int tasks = 0, done = 0;
pthread_mutex_t lock;

void dummy_task(void *arg) {
  (void)arg;
  usleep(1000);
  TCHK(pthread_mutex_lock(&lock));
  done++;
  TCHK(pthread_mutex_unlock(&lock));
}

void thpool_test_0(void) {
  threadpool_t pool;

  tasks = 0;
  done = 0;
  TCHK(pthread_mutex_init(&lock, NULL));

  assert_neq((pool = threadpool_create(THREAD, QUEUE, 0)), NULL);

  while (threadpool_add(pool, &dummy_task, NULL, 0) == 0) {
    tasks++;
  }

  while ((tasks / 2) > done) {
    usleep(1000);
  }
  assert_eq(threadpool_destroy(pool, 0), 0);
  assert_lt(done, tasks);
  TCHK(pthread_mutex_destroy(&lock));
}

void thpool_test_1(void) {
  threadpool_t pool;

  tasks = 0;
  done = 0;
  TCHK(pthread_mutex_init(&lock, NULL));

  assert_neq((pool = threadpool_create(THREAD, QUEUE, 0)), NULL);

  for (int i = 0; i < 100; i++) {
    assert_eq(threadpool_add(pool, &dummy_task, NULL, 0), 0);
    tasks++;
  }

  assert_eq(threadpool_destroy(pool, 1), 0);
  TCHK(pthread_mutex_destroy(&lock));
  assert_eq(tasks, done);
}

void thpool_test_2(void) {
  threadpool_t pool1, pool2;

  tasks = 0;
  done = 0;
  TCHK(pthread_mutex_init(&lock, NULL));

  assert_neq((pool1 = threadpool_create(THREAD, QUEUE, 0)), NULL);
  assert_neq((pool2 = threadpool_create(THREAD, QUEUE, 0)), NULL);

  for (int i = 0; i < 100; i++) {
    assert_eq(threadpool_add(pool1, &dummy_task, NULL, 0), 0);
    assert_eq(threadpool_add(pool2, &dummy_task, NULL, 0), 0);
    tasks++;
  }

  assert_eq(threadpool_destroy(pool1, 1), 0);
  assert_eq(threadpool_destroy(pool2, 1), 0);
  TCHK(pthread_mutex_destroy(&lock));
  assert_eq(2 * tasks, done);
}

void thpool_test(void) {
  test_case(thpool_test_0);
  test_case(thpool_test_1);
  test_case(thpool_test_2);
}
