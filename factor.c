#include <pthread.h>
#include "factor.h"
#include "factor_globals.h"
#include "factor_thread_routine_arg.h"
#include "factor_thread_routine.h"

uint32_t factor (uint64_t n, struct degree* d, uint64_t* r) {
    uint32_t k = 0;
    uint32_t i;
    uint32_t p;
    uint32_t b;
    uint32_t a[] = {
#include "a.csv"
    };
    const uint32_t phi = sizeof(a) / sizeof(uint32_t);
    const uint32_t small_primes[] = {
#include "small_primes.csv"
    };
    const uint32_t K = sizeof(small_primes) / sizeof(uint32_t);
    pthread_t pid[phi - 1];
    pthread_mutex_t mutex;
    struct factor_thread_routine_arg arg;
    uint32_t threads;

#if USE_THREADS_MUTEX
    pthread_mutex_lock(&factor_threads_mutex);
#endif
    threads = factor_threads;
#if USE_THREADS_MUTEX
    pthread_mutex_unlock(&factor_threads_mutex);
#endif

    if (n <= 1) {
        if (r != 0) {
            *r = n;
        }
        return k;
    }
    for (i = 0; i < K; ++i) {
        b = 0;
        while (n % small_primes[i] == 0) {
            n /= small_primes[i];
            ++b;
        }
        if (b != 0) {
            d[k].p = small_primes[i];
            d[k].b = b;
            ++k;
        }
    }
    pthread_mutex_init(&mutex, 0);
    arg.mutex = &mutex;
    arg.a = a;
    if (threads == 0) {
        threads = phi;
    }
    if (threads > phi) {
        threads = phi;
    }
    while (1) {
        arg.n = n;
        arg.jobs_left = phi;
        for (i = 0; i < threads - 1; ++i) {
            pthread_create(&pid[i], 0, factor_thread_routine, &arg);
        }
        factor_thread_routine(&arg);
        for (i = 0; i < threads - 1; ++i) {
            pthread_join(pid[i], 0);
        }
        p = a[0];
        for (i = 1; i < phi; ++i) {
            if ((uint32_t) (a[i] - 1) < (uint32_t) (p - 1)) { /* (a != 0) && ((p == 0) || (a < p)) */
                p = a[i];
            }
        }
        if (p != 0) {
            b = 0;
            do {
                n /= p;
                ++b;
            } while (n % p == 0);
            d[k].p = p;
            d[k].b = b;
            ++k;
        }
        else {
            break;
        }
    }
    pthread_mutex_destroy(&mutex);
    if ((n > 1) && (n < ((uint64_t) 1 << 32))) {
        d[k].p = n;
        d[k].b = 1;
        ++k;
        n = 1;
    }
    if (r != 0) {
        *r = n;
    }
    return k;
}
