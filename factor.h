#ifndef _FACTOR_H
#define _FACTOR_H

#include <stdint.h>

struct degree {
    uint32_t p;
    uint32_t b;
};

extern uint32_t factor_threads;

#ifdef __cplusplus
extern "C" {
#endif

uint32_t factor (uint64_t, struct degree*, uint64_t*);

#ifdef __cplusplus
}
#endif

#endif
