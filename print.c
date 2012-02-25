#include <stdio.h>
#include "print.h"
#include "globals.h"

void print (uint32_t k, const struct degree* d, uint64_t r) {
    const char sep_char = linebreak ? '\t' : ' ';
    uint32_t i;
    uint32_t j;
    for (i = 0; i < k; ++i) {
        if (raw) {
            for (j = 0; j < d[i].b; ++j) {
                printf("%c%u", sep_char, d[i].p);
                if (linebreak) {
                    printf("\n");
                }
            }
        }
        else {
            if (d[i].b == 1) {
                printf("%c%u", sep_char, d[i].p);
                if (linebreak) {
                    printf("\n");
                }
            }
            else {
                printf("%c", sep_char);
                if (!linebreak) {
                    printf("(");
                }
                printf("%u ^ %u", d[i].p, d[i].b);
                if (linebreak) {
                    printf("\n");
                }
                else {
                    printf(")");
                }
            }
        }
    }
    if ((r != 1) || (k == 0)) {
        printf("%c%llu", sep_char, r);
        if (linebreak) {
            printf("\n");
        }
    }
}
