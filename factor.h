#ifndef _FACTOR_H
#define _FACTOR_H

#include <stdint.h>

struct degree {
    uint32_t p;
    uint32_t b;
};

#ifdef __cplusplus
extern "C" {
#endif

uint32_t factor (uint64_t n, struct degree* d, uint64_t* r);

void set_factor_threads (uint32_t threads);

#ifdef __cplusplus
}
#endif

#endif
