#include <stdio.h>
#include "print.h"
#include "globals.h"

void print (uint32_t k, const struct degree* d, uint64_t r) {
    uint32_t i;
    uint32_t j;
    for (i = 0; i < k; ++i) {
        if (raw) {
            for (j = 0; j < d[i].b; ++j) {
                printf("\t%u\n", d[i].p);
            }
        }
        else {
            if (d[i].b == 1) {
                printf("\t%u\n", d[i].p);
            }
            else {
                printf("\t%u ^ %u\n", d[i].p, d[i].b);
            }
        }
    }
    if ((r != 1) || (k == 0)) {
        printf("\t%llu\n", r);
    }
}
