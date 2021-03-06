/* test.h
   Rémi Attab (remi.attab@gmail.com), 03 Mar 2016
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once

#include "utils/rng.h"
#include "utils/htable.h"
#include "utils/type_pun.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <setjmp.h>
#include <string.h>

#include <cmocka.h>


// -----------------------------------------------------------------------------
// asserts
// -----------------------------------------------------------------------------

bool assert_float_equal_impl(double a, double b, double epsilon);

#define assert_float_equal(a, b, epsilon)                       \
    assert_true(assert_float_equal_impl(a, b, epsilon))


bool assert_htable_equal_impl(
        struct htable *,
        const struct htable_bucket *buckets,
        size_t buckets_len,
        double epsilon);

#define make_kv(k, v) {k, pun_dtoi(v)}

#define assert_htable_equal(set, eps, ...)                          \
    do {                                                                \
        struct htable_bucket exp[] = { __VA_ARGS__ };                   \
        size_t len = sizeof(exp) / sizeof(struct htable_bucket);        \
        assert_true(assert_htable_equal_impl(set, exp, len, eps));  \
    } while (false)

