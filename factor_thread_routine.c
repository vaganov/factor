#include "factor_thread_routine.h"
#include "factor_thread_routine_arg.h"
#include "seek.h"

void* factor_thread_routine (void* p) {
    struct factor_thread_routine_arg* arg = p;
    uint32_t job;
    uint32_t done;
    while (1) {
        pthread_mutex_lock(arg->mutex);
        if (arg->jobs_left != 0) {
            done = 0;
            job = --(arg->jobs_left);
        }
        else {
            done = 1;
        }
        pthread_mutex_unlock(arg->mutex);
        if (done) {
            return 0;
        }
        seek(arg->n, arg->a[job]);
    }
}
