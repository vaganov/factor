#include "seek.h"
#include "D.h"

void seek (const mpz_t n, mpz_t a) {
    mpz_t sqrt_n;
    if (mpz_sgn(a) == 0) {
        return;
    }
    mpz_init(sqrt_n);
    mpz_sqrt(sqrt_n, n);
    while (1) {
        if (mpz_divisible_p(n, a)) {
            return;
        }
        mpz_add_ui(a, a, D);
        if (mpz_cmp(a, sqrt_n) > 0) {
            mpz_set_ui(a, 0);
            return;
        }
    }
}
