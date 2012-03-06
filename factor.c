#include <pthread.h>
#include <factor.h>
#include "launch_arg.h"
#include "launch.h"
#include "seek.h"
#include "D.h"

#if D == 30
#define NTHREADS 8
#elif D == 60
#define NTHREADS 16
#else
#error D.h: only D = 30 or D = 60 supported
#endif

uint32_t factor (uint64_t n, struct degree* d, uint64_t* r) {
    uint32_t k = 0;
    uint32_t i;
    uint32_t p;
    uint32_t b;
    pthread_t pid[NTHREADS]; /* pid[0] not used */
    launch_arg arg[NTHREADS]; /* arg[0] not used */
    const uint32_t a[] = {
        1 + D, 7, 11, 13, 17, 19, 23, 29
#if D >= 60
        ,31, 37, 41, 43, 47, 49 + D, 53, 59
#endif
    };
    uint32_t a0;

    if (n <= 1) {
        *r = n;
        return k;
    }

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

    for (i = NTHREADS - 1; i != 0; --i) {
        arg[i].a = a[i];
    }
    a0 = a[0];

    while (1) {
        for (i = NTHREADS - 1; i != 0; --i) {
            arg[i].n = n;
        }
        for (i = NTHREADS - 1; i != 0; --i) {
            pthread_create(&pid[i], 0, launch, &arg[i]);
        }
        a0 = seek(n, a0);
        for (i = NTHREADS - 1; i != 0; --i) {
            pthread_join(pid[i], 0);
        }
        p = a0;
        for (i = NTHREADS - 1; i != 0; --i) {
            if ((uint32_t) (arg[i].a - 1) < (uint32_t) (p - 1)) { /* (a != 0) && ((p == 0) || (a < p)) */
                p = arg[i].a;
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

    *r = n;
    return k;
}
