#include <stdio.h>
#include "factor.h"

int main (void) {
    mpz_t n;
    uint32_t k;
    uint32_t i;
    struct degree d[15];
    mpz_init(n);
    set_factor_threads(8);
    while (1) {
        printf("n = ");
        mpz_inp_str(n, 0, 10);
        k = factor(n, d);
        for (i = 0; i < k; ++i) {
            printf("\t");
            mpz_out_str(0, 10, d[i].p);
            if (d[i].b > 1) {
                printf(" ^ %lu", d[i].b);
            }
            printf("\n");
        }
    }
}
