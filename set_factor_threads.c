#include "factor.h"
#include "factor_globals.h"

void set_factor_threads (uint32_t threads) {
#if USE_THREADS_MUTEX
    pthread_mutex_lock(&factor_threads_mutex);
#endif
    factor_threads = threads;
#if USE_THREADS_MUTEX
    pthread_mutex_unlock(&factor_threads_mutex);
#endif
}
