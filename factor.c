#include <pthread.h>
#include "factor.h"
#include "factor_globals.h"
#include "factor_thread_routine_arg.h"
#include "factor_thread_routine.h"

uint32_t factor (const mpz_t _n, struct degree* d) {
    mpz_t n;
    uint32_t k = 0;
    uint32_t i;
    mpz_t p;
    uint32_t b;
    const uint32_t _a[] = {
#include "a.csv"
    };
    const uint32_t phi = sizeof(_a) / sizeof(uint32_t);
    mpz_t a[phi];
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

    mpz_init_set(n, _n);
    mpz_init(p);
    for (i = 0; i < phi; ++i) {
        mpz_init_set_ui(a[i], _a[i]);
    }

    for (i = 0; i < K; ++i) {
        mpz_set_ui(p, small_primes[i]);
        b = mpz_remove(n, n, p);
        if (b != 0) {
            mpz_init_set(d[k].p, p);
            d[k].b = b;
            ++k;
        }
    }
    pthread_mutex_init(&mutex, 0);
    arg.mutex = &mutex;
    mpz_init(arg.n);
    arg.a = a;
    if (threads == 0) {
        threads = phi;
    }
    if (threads > phi) {
        threads = phi;
    }
    while (1) {
        mpz_set(arg.n, n);
        arg.jobs_left = phi;
        for (i = 0; i < threads - 1; ++i) {
            pthread_create(&pid[i], 0, factor_thread_routine, &arg);
        }
        factor_thread_routine(&arg);
        for (i = 0; i < threads - 1; ++i) {
            pthread_join(pid[i], 0);
        }
        mpz_set(p, a[0]);
        for (i = 1; i < phi; ++i) {
            if ((mpz_sgn(a[i]) != 0) && ((mpz_sgn(p) == 0) || (mpz_cmp(a[i], p) < 0))) {
                mpz_set(p, a[i]);
            }
        }
        if (mpz_sgn(p) != 0) {
            b = mpz_remove(n, n, p);
            mpz_init_set(d[k].p, p);
            d[k].b = b;
            ++k;
        }
        else {
            break;
        }
    }
    pthread_mutex_destroy(&mutex);
    if (mpz_cmp_ui(n, 1) > 0) {
        mpz_init_set(d[k].p, n);
        d[k].b = 1;
        ++k;
    }
    return k;
}
