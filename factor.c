#include <pthread.h>
#include <factor.h>
#include "launch_arg.h"
#include "launch.h"
#include "seek.h"

uint32_t factor (uint64_t n, struct degree* d, uint64_t* r) {
    uint32_t k = 0;
    uint32_t i;
    uint32_t p;
    uint32_t b;
    pthread_t pid[7];
    launch_arg arg[7];
    const uint32_t a[] = {31, 7, 11, 13, 17, 19, 23, 29};
    uint32_t a7;

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

    for (i = 0; i < 7; ++i) {
        arg[i].a = a[i];
    }
    a7 = a[7];

    while (1) {
        for (i = 0; i < 7; ++i) {
            arg[i].n = n;
            pthread_create(&pid[i], 0, launch, &arg[i]);
        }
        p = a7 = seek(n, a7);
        for (i = 0; i < 7; ++i) {
            pthread_join(pid[i], 0);
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
