/* time.c
   Rémi Attab (remi.attab@gmail.com), 03 Mar 2016
   FreeBSD-style copyright and disclaimer apply
*/


// -----------------------------------------------------------------------------
// time
// -----------------------------------------------------------------------------

iota_ts_t clock_wall()
{
    struct timespec ts;

    int ret = clock_gettime(CLOCK_REALTIME_COARSE, &ts);
    if (!ret) return ts.tv_sec;

    abort();
}

uint64_t clock_rdtsc()
{
    uint64_t msb, lsb;

#ifdef __amd64
    __asm__ __volatile__ ("rdtsc" : "=a" (lsb), "=d" (msb));
#else
#error "Read your platform's perf counter here"
#endif

    return msb << 32 | lsb;
}


// -----------------------------------------------------------------------------
// sleep
// -----------------------------------------------------------------------------

bool nsleep(uint64_t nanos)
{
    struct timespec ts;
    clock_monotonic(&ts);

    ts.tv_nsec += nanos;
    if (ts.tv_nsec >= 1000000000) {
        ts.tv_sec += ts.tv_nsec / 1000000000;
        ts.tv_nsec = ts.tv_nsec % 1000000000;
    }

    while (true) {
        int ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
        if (!ret) return true;
        if (errno == EINTR) continue;
        abort();
    }
}
