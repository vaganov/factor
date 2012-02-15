#include "seek.h"

uint32 seek (uint64 n, uint32 a) {
    uint64 _a;
    if (a == 0) {
        return 0;
    }
    while (1) {
        if (n % a == 0) {
            return a;
        }
        a += 30;
        if (a < 30) {
            return 0;
        }
        _a = a;
        if (_a * _a > n) {
            return 0;
        }
    }
}
