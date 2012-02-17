#include <stdio.h>
#include "typedef.h"
#include "degree.h"
#include "factor.h"
#include "print.h"

int main (int argc, char* argv[]) {
    uint32 i;
    uint64 n;
    uint32 k;
    degree d[15]; /* enough for n < 2^64 */
    uint64 r;

    const uint64 benchmark[] = {(uint64) 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47,
                                (uint64) - 59,
                                ((uint64) 1 << 32) + 15,
                                (((uint64) 1 << 32) - 5) * (((uint64) 1 << 32) - 5)};

    if (argc != 2) {
        printf("usage: %s NUMBER\n", argv[0]);
        printf("e.g.:\n");
        for (i = 0; i < sizeof(benchmark) / sizeof(uint64); ++i) {
            printf("\t%s %llu\n", argv[0], benchmark[i]);
        }
        return 1;
    }

    if (!sscanf(argv[1], "%llu", &n)) {
        printf("\"%s\": incorrect argument\n", argv[1]);
        return 1;
    }

    printf("%llu\n", n);

    k = factor(n, d, &r);

    print(k, d, r);

    return 0;
}
