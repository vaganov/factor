#include <iostream>
#include "print.h"
#include "globals.h"

void print (uint32_t k, const struct degree* d, uint64_t r) {
    const char sep_char = linebreak ? '\t' : ' ';
    uint32_t i;
    uint32_t j;
    for (i = 0; i < k; ++i) {
        if (raw) {
            for (j = 0; j < d[i].b; ++j) {
                std::cout << sep_char << d[i].p;
                if (linebreak) {
                    std::cout << "\n";
                }
            }
        }
        else {
            if (d[i].b == 1) {
                std::cout << sep_char << d[i].p;
                if (linebreak) {
                    std::cout << "\n";
                }
            }
            else {
                std::cout << sep_char;
                if (!linebreak) {
                    std::cout << "(";
                }
                std::cout << d[i].p << " ^ " << d[i].b;
                if (linebreak) {
                    std::cout << "\n";
                }
                else {
                    std::cout << ")";
                }
            }
        }
    }
    if ((r != 1) || (k == 0)) {
        std::cout << sep_char << r;
        if (linebreak) {
            std::cout << "\n";
        }
    }
}
