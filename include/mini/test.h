#ifndef INCLUDE_MINI_TEST_H_
#define INCLUDE_MINI_TEST_H_

#include "mini/common.h"

#ifndef TEST_ENUM
# error Please define TEST_ENUM before including `mini-test.h`
# define TEST_ENUM(V)
#endif  /* !TEST_ENUM */

#define TEST_DECL(N) void test__##N();

TEST_ENUM(TEST_DECL)

#undef TEST_DECL

#define TEST_FN(N) test__##N
#define TEST_IMPL(N) void test__##N()

#endif  /* INCLUDE_MINI_TEST_H_ */
