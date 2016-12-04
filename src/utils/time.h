/* time.h
   Rémi Attab (remi.attab@gmail.com), 03 Mar 2016
   FreeBSD-style copyright and disclaimer apply
*/

#pragma once

#include "compiler.h"

#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


// -----------------------------------------------------------------------------
// time
// -----------------------------------------------------------------------------

typedef uint64_t iota_ts_t;

iota_ts_t clock_wall();
iota_ts_t clock_rdtsc();

inline void clock_monotonic(struct timespec *ts)
{
    if (iota_unlikely(clock_gettime(CLOCK_MONOTONIC, ts) == -1)) abort();
}


// -----------------------------------------------------------------------------
// sleep
// -----------------------------------------------------------------------------

bool nsleep(uint64_t nanos);
