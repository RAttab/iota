/* bench.c
   Rémi Attab (remi.attab@gmail.com), 23 Jan 2016
   FreeBSD-style copyright and disclaimer apply
*/

#include "bench.h"
#include "utils/time.h"

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// -----------------------------------------------------------------------------
// utils
// -----------------------------------------------------------------------------


static double scale_elapsed(double t, char *s)
{
    static const char scale[] = "smunp";

    size_t i = 0;
    for (i = 0; i < sizeof(scale); ++i) {
        if (t >= 1.0) break;
        t *= 1000.0;
    }

    *s = scale[i];
    return t;
}



// -----------------------------------------------------------------------------
// iota_bench
// -----------------------------------------------------------------------------

struct iota_bench
{
    void **setup_data;

    bool started;
    struct timespec start;

    bool stopped;
    struct timespec stop;
};

void *iota_bench_setup(struct iota_bench *bench, void *data)
{
    if (data) *bench->setup_data = data;
    return *bench->setup_data;
}

void iota_bench_start(struct iota_bench *bench)
{
    assert(!bench->started);
    bench->started = true;

    clock_monotonic(&bench->start);
}

void iota_bench_stop(struct iota_bench *bench)
{
    iota_no_opt_clobber(); // make sure everything is done before we stop.

    clock_monotonic(&bench->stop);
    bench->stopped = true;
}


// -----------------------------------------------------------------------------
// utils
// -----------------------------------------------------------------------------


static double  sec() { return 1; }
static double msec() { return  sec() / 1000; }
static double usec() { return msec() / 1000; }
static double nsec() { return usec() / 1000; }

static double bench_elapsed(struct iota_bench *bench)
{
    return
        (bench->stop.tv_sec - bench->start.tv_sec) +
        ((bench->stop.tv_nsec - bench->start.tv_nsec) * nsec());
}

static double run_bench(
        struct iota_bench *bench,
        iota_bench_fn_t fn,
        void *ctx,
        size_t id,
        size_t n)
{
    fn(bench, ctx, id, n);

    if (!bench->stopped) iota_bench_stop(bench);
    assert(bench->started);

    return bench_elapsed(bench);
}

int cmp_elapsed(const void *_lhs, const void *_rhs)
{
    double lhs = *((const double *) _lhs);
    double rhs = *((const double *) _rhs);

    if (lhs < rhs) return -1;
    if (lhs > rhs) return 1;
    return 0;
}


static void bench_report(const char *title, size_t n, double *dist, size_t dist_len)
{
    for (size_t i = 0; i < dist_len; ++i) dist[i] /= n;
    qsort(dist, dist_len, sizeof(double), cmp_elapsed);

    char p0_mul = ' ';
    double p0_val = scale_elapsed(dist[0], &p0_mul);

    char p50_mul = ' ';
    double p50_val = scale_elapsed(dist[dist_len / 2], &p50_mul);

    char p90_mul = ' ';
    double p90_val = scale_elapsed(dist[(dist_len * 90) / 100], &p90_mul);

    printf("bench: %-30s  %8lu    p0:%6.2f%c    p50:%6.2f%c    p90:%6.2f%c\n",
            title, n, p0_val, p0_mul, p50_val, p50_mul, p90_val, p90_mul);
}

void iota_bench(const char *title, iota_bench_fn_t fn, void *ctx)
{
    const double duration = 1 * msec();
    const size_t iterations = 1000;

    size_t dist_len = iterations;
    double dist[dist_len];
    memset(dist, 0, dist_len * sizeof(double));

    size_t n = 1;
    double elapsed = -1;
    for (; elapsed < duration; n *= 2) {
        assert(n < 100UL * 1000 * 1000 * 1000);

        void *setup_data;
        struct iota_bench bench = { .setup_data = &setup_data };
        elapsed = run_bench(&bench, fn, ctx, 0, n);
    }

    for (size_t i = 0; i < iterations; ++i) {
        void *setup_data;
        struct iota_bench bench = { .setup_data = &setup_data };
        dist[i] = run_bench(&bench, fn, ctx, 0, n);
    }

    bench_report(title, n, dist, dist_len);
}
