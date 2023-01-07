#include <pthread.h>
#include <stdlib.h>
#include <string.h>

#include "lib.h"
#include "m.h"

#include "vec.h"

void *access_vec(void *arg) {
  vec_t v = (vec_t)arg;
  for (size_t i = 0; i < 1000; i++) {
    vec_push(v, (void *)i);
  }

  for (size_t i = 0; i < 1000; i++) {
    vec_pop(v);
  }
  return NULL;
}

void vec_test_0(void) {
  vec_t v = vec_new();
  vec_push(v, (void *)1);
  vec_push(v, (void *)2);
  vec_push(v, (void *)3);
  vec_push(v, (void *)4);

  assert_eq(vec_size(v), 4);
  assert_eq(vec_get(v, 0), (void *)1);
  assert_eq(vec_get(v, 1), (void *)2);
  assert_eq(vec_get(v, 2), (void *)3);
  assert_eq(vec_get(v, 3), (void *)4);

  vec_free(v);
}

void vec_test_1(void) {
  size_t n = 1000;
  vec_t v = vec_new(n);
  assert_eq(vec_size(v), 0);
  assert_eq(vec_capacity(v), n);

  for (size_t i = 0; i < n; i++) {
    vec_push(v, (void *)i);
  }

  assert_eq(vec_size(v), n);
  assert_geq(vec_capacity(v), n);

  for (size_t i = 0; i < n; i++) {
    assert_eq(vec_get(v, i), (void *)i);
  }

  vec_free(v);
}

void vec_test_2(void) {
  size_t n = 1000;
  vec_t v = vec_new(n);
  assert_eq(vec_size(v), 0);
  assert_eq(vec_capacity(v), n);

  for (size_t i = 0; i < n; i++) {
    // here we add +1 to i to avoid 0 (NULL) values
    vec_push(v, (void *)(i + 1));
  }

  assert_eq(vec_size(v), n);
  assert_geq(vec_capacity(v), n);

  void *data;
  vec_foreach(v, i, data) {
    // as we check `data = vec_get(v, i)` against NULL
    // we need to not get 0 as the first value
    assert_eq(data, (void *)(i + 1));
  }

  vec_free(v);
}

void vec_test_3(void) {
  vec_t v = vec_from((void *)1, (void *)2, (void *)3, (void *)4, NULL);

  assert_eq(vec_size(v), 4);
  assert_eq(vec_capacity(v), 4);

  assert_eq(vec_get(v, 0), (void *)1);
  assert_eq(vec_get(v, 1), (void *)2);
  assert_eq(vec_get(v, 2), (void *)3);
  assert_eq(vec_get(v, 3), (void *)4);

  vec_free(v);
}

void vec_test_4(void) {
  size_t n = 1000;
  vec_t v = vec_new(n);

  for (size_t i = 0; i < n; i++) {
    vec_push(v, (void *)i);
  }

  // check for thread safety
  pthread_t threads[10];
  for (size_t i = 0; i < 10; i++) {
    TCHK(pthread_create(&threads[i], NULL, (void *(*)(void *))access_vec, v));
  }

  for (size_t i = 0; i < 10; i++) {
    TCHK(pthread_join(threads[i], NULL));
    assert_eq(vec_size(v), n);
  }
  assert_eq(vec_size(v), n);

  vec_free(v);
}

void vec_test_5(void) {
  vec_t v = vec_from("long string a", "long string b", "long string c", NULL);

  assert_eq(vec_size(v), 3);
  assert_eq(vec_capacity(v), 3);

  assert_eq(strcmp(vec_get(v, 0), "long string a"), 0);
  assert_eq(strcmp(vec_get(v, 1), "long string b"), 0);
  assert_eq(strcmp(vec_get(v, 2), "long string c"), 0);

  vec_free(v);
}

void vec_test(void) {
  test_case(vec_test_0);
  test_case(vec_test_1);
  test_case(vec_test_2);
  test_case(vec_test_3);
  test_case(vec_test_4);
  test_case(vec_test_5);
}
