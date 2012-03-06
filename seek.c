#include "seek.h"
#include "D.h"

uint32_t seek (uint64_t n, uint32_t a) {
    uint64_t _a;
    if (a == 0) {
        return 0;
    }
    while (1) {
        if (n % a == 0) {
            return a;
        }
        a += D;
        if (a < D) {
            return 0;
        }
        _a = a;
        if (_a * _a > n) {
            return 0;
        }
    }
}
