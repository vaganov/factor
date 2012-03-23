#ifndef _FACTOR_THREAD_ROUTINE_ARG_H
#define _FACTOR_THREAD_ROUTINE_ARG_H

#include <pthread.h>
#include <stdint.h>

struct factor_thread_routine_arg {
    pthread_mutex_t* mutex;
    uint32_t jobs_left;
    uint64_t n;
    uint32_t* a;
};

#endif
