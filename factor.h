#ifndef _FACTOR_H
#define _FACTOR_H

#include <stdint.h>
#include <gmp.h>

struct degree {
    mpz_t p;
    uint32_t b;
};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t factor (const mpz_t n, struct degree* d);

void set_factor_threads (uint32_t threads);

#ifdef __cplusplus
}
#endif

#endif
