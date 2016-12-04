/* bench_bench.c
   Rémi Attab (remi.attab@gmail.com), 24 Jan 2016
   FreeBSD-style copyright and disclaimer apply

   control tests for the bench framework
*/

#include "bench.h"
#include "utils/time.h"

// -----------------------------------------------------------------------------
// harness bench
// -----------------------------------------------------------------------------

void run_harness_bench(struct iota_bench *b, void *data, size_t id, size_t n)
{
    (void) id, (void) data;
    iota_bench_start(b);

    for (size_t i = 0; i < n; ++i) iota_no_opt();
}

void harness_bench(iota_unused void **state)
{
    iota_bench("harness_bench", run_harness_bench, NULL);
}


// -----------------------------------------------------------------------------
// sleep bench
// -----------------------------------------------------------------------------

void run_sleep_bench(struct iota_bench *b, void *data, size_t id, size_t n)
{
    (void) id, (void) data;
    iota_bench_start(b);

    nsleep(n);
}

void sleep_bench(iota_unused void **state)
{
    iota_bench("sleep_bench", run_sleep_bench, NULL);
}


// -----------------------------------------------------------------------------
// setup
// -----------------------------------------------------------------------------

int main(void)
{
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(harness_bench),
        cmocka_unit_test(sleep_bench),
    };

    return cmocka_run_group_tests(tests, NULL, NULL);
}
