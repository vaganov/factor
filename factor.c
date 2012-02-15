#include <pthread.h>
#include "factor.h"
#include "launch_arg.h"
#include "launch.h"
#include "seek.h"

uint32 factor (uint64 n, degree* d, uint64* r) {
    uint32 k = 0;
    uint32 i;
    uint32 p;
    uint32 b;
    pthread_t pid[7];
    launch_arg arg[7];
    const uint32 a[] = {31, 7, 11, 13, 17, 19, 23, 29};
    uint32 a7;

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
            if ((uint32) (arg[i].a - 1) < (uint32) (p - 1)) { /* (a != 0) && ((p == 0) || (a < p)) */
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
