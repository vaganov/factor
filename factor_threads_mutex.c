#include "factor_globals.h"

#if USE_THREADS_MUTEX

static void __attribute__((constructor)) factor_threads_mutex_init (void) {
    pthread_mutex_init(&factor_threads_mutex, 0);
}

static void __attribute__((destructor)) factor_threads_mutex_destroy (void) {
    pthread_mutex_destroy(&factor_threads_mutex);
}

#endif
