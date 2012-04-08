#ifndef _FACTOR_GLOBALS_H
#define _FACTOR_GLOBALS_H

#include <stdint.h>

#define USE_THREADS_MUTEX 1

#if USE_THREADS_MUTEX
#include <pthread.h>
#endif

extern uint32_t factor_threads;

#if USE_THREADS_MUTEX
extern pthread_mutex_t factor_threads_mutex;
#endif

#endif
