#include <stdio.h>
#include "print.h"

void print (uint32 k, const degree* d, uint64 r) {
    uint32 i;
    for (i = 0; i < k; ++i) {
        if (d[i].b == 1) {
            printf("\t%u\n", d[i].p);
        }
        else {
            printf("\t%u ^ %u\n", d[i].p, d[i].b);
        }
    }
    if ((r != 1) || (k == 0)) {
        printf("\t%llu\n", r);
    }
}
