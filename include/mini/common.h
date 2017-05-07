#ifndef INCLUDE_MINI_COMMON_H_
#define INCLUDE_MINI_COMMON_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define CHECK(VALUE, MESSAGE)                                                \
    do {                                                                     \
      if ((VALUE)) break;                                                    \
      fprintf(stderr, "%s[%d] Assertion failure: " #MESSAGE "\n",         \
              __FILE__, __LINE__);                                           \
      abort();                                                               \
    } while (0)

#define CHECK_EQ(A, B, MESSAGE) CHECK((A) == (B), MESSAGE)
#define CHECK_NE(A, B, MESSAGE) CHECK((A) != (B), MESSAGE)

void mini_prepare_runner(const char* main);
int mini_run_single(const char* test);

#endif  /* INCLUDE_MINI_COMMON_H_ */
