#include "factor_globals.h"

uint32_t factor_threads = 1;

#if USE_THREADS_MUTEX
pthread_mutex_t factor_threads_mutex;
#endif
