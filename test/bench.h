/* bench.h
   Rémi Attab (remi.attab@gmail.com), 23 Jan 2016
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once

#include "test.h"

// -----------------------------------------------------------------------------
// bench
// -----------------------------------------------------------------------------

struct iota_bench;
typedef void (* iota_bench_fn_t) (
        struct iota_bench *, void * ctx, size_t id, size_t n);

void *iota_bench_setup(struct iota_bench *, void *data);
void iota_bench_start(struct iota_bench *);
void iota_bench_stop(struct iota_bench *);

void iota_bench(const char *title, iota_bench_fn_t fn, void *ctx);
