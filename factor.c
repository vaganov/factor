#include <pthread.h>
#include <factor.h>
#include "D.h"
#include "factor_thread_routine_arg.h"
#include "factor_thread_routine.h"

uint32_t factor (uint64_t n, struct degree* d, uint64_t* r) {
    uint32_t k = 0;
    uint32_t i;
    uint32_t p;
    uint32_t b;
    uint32_t a[] = {
#include "a.h"
    };
    const uint32_t phi = sizeof(a) / sizeof(uint32_t);
    pthread_t pid[phi - 1];
    pthread_mutex_t mutex;
    struct factor_thread_routine_arg arg;
    uint32_t threads = factor_threads;

    if (n <= 1) {
        if (r != 0) {
            *r = n;
        }
        return k;
    }
#if D % 2 == 0
    b = 0;
    while (n % 2 == 0) {
        n /= 2;
        ++b;
    }
    if (b != 0) {
        d[k].p = 2;
        d[k].b = b;
        ++k;
    }
#endif
#if D % 3 == 0
    b = 0;
    while (n % 3 == 0) {
        n /= 3;
        ++b;
    }
    if (b != 0) {
        d[k].p = 3;
        d[k].b = b;
        ++k;
    }
#endif
#if D % 5 == 0
    b = 0;
    while (n % 5 == 0) {
        n /= 5;
        ++b;
    }
    if (b != 0) {
        d[k].p = 5;
        d[k].b = b;
        ++k;
    }
#endif
#if D % 7 == 0
    b = 0;
    while (n % 7 == 0) {
        n /= 7;
        ++b;
    }
    if (b != 0) {
        d[k].p = 7;
        d[k].b = b;
        ++k;
    }
#endif
#if D % 11 == 0
    b = 0;
    while (n % 11 == 0) {
        n /= 11;
        ++b;
    }
    if (b != 0) {
        d[k].p = 11;
        d[k].b = b;
        ++k;
    }
#endif
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
