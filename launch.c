#include "launch.h"
#include "launch_arg.h"
#include "seek.h"

void* launch (void* p) {
    launch_arg* arg = p;
    arg->a = seek(arg->n, arg->a);
    return 0;
}
