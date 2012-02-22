#include <getopt.h>
#include <stdio.h>
#include <factor.h>
#include "globals.h"
#include "print.h"

int main (int argc, char* argv[]) {
    uint32_t i;
    uint64_t n;
    uint32_t k;
    struct degree d[15]; /* enough for n < 2^64 */
    uint64_t r;

    struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"raw", no_argument, 0, 'r'},
        {"benchmark", no_argument, 0, 'B'},
        {0, 0, 0, 0}
    };
    char ch;
    int longind;

    int input = 0;

    char s[21]; /* enough for n < 2^64 */

    const uint64_t benchmark[] = {
        (uint64_t) 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47,
        (uint64_t) - 59,
        ((uint64_t) 1 << 32) + 15,
        (((uint64_t) 1 << 32) - 5) * (((uint64_t) 1 << 32) - 5)
    };

    while ((ch = getopt_long(argc, argv, "hvr", longopts, &longind)) != -1) {
        switch (ch) {
            case 'h':
                printf("%s [ [ --%s ] NUMBER [ NUMBER ... ] | --%s | --%s | --%s ]\n",
                    argv[0],
                    longopts[2].name,
                    longopts[3].name,
                    longopts[1].name,
                    longopts[0].name);
                printf("\n");
                printf("print the prime factors of each specified NUMBER\n");
                printf("if none are specified, read them from standard input\n");
                printf("\n");
                printf("\t--%s, -%c\tprint prime factors in raw format\n", longopts[2].name, longopts[2].val);
                printf("\t--%s\tprint some useful benchmarks and exit\n", longopts[3].name);
                printf("\t--%s, -%c\tprint version and exit\n", longopts[1].name, longopts[1].val);
                printf("\t--%s, -%c\tprint this message and exit\n", longopts[0].name, longopts[0].val);
                return 0;
            case 'v':
                printf("%s v. 1.0\n", argv[0]);
                return 0;
            case 'r':
                raw = 1;
                break;
            case 'B':
                for (i = 0; i < sizeof(benchmark) / sizeof(uint64_t); ++i) {
                    printf("%llu\n", benchmark[i]);
                }
                return 0;
        }
    }

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            ++input;
            if (!sscanf(argv[i], "%llu", &n)) {
                printf("\"%s\": incorrect argument\n", argv[i]);
            }
            else {
                k = factor(n, d, &r);
                printf("%llu\n", n);
                print(k, d, r);
            }
        }
    }

    if (input) {
        return 0;
    }
    else {
        while (1) {
            scanf("%20s", s);
            if (!sscanf(s, "%llu", &n)) {
                printf("\"%s\": incorrect argument\n", s);
            }
            else {
                k = factor(n, d, &r);
                print(k, d, r);
            }
        }
    }
}
