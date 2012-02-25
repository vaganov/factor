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
        {"linebreak", no_argument, 0, 'n'},
        {0, 0, 0, 0}
    };
    char ch;
    int longind;

    int input = 0;

    char s[21]; /* enough for n < 2^64 */

    while ((ch = getopt_long(argc, argv, "hvrn", longopts, &longind)) != -1) {
        switch (ch) {
            case 'h':
                printf("%s [ [ --%s ] [ --%s ] NUMBER [ NUMBER ... ] | --%s | --%s | --%s ]\n",
                    argv[0],
                    longopts[2].name,
                    longopts[4].name,
                    longopts[3].name,
                    longopts[1].name,
                    longopts[0].name);
                printf("\n");
                printf("print the prime factors of each specified NUMBER\n");
                printf("if none are specified, read them from standard input\n");
                printf("\n");
                printf("\t--%s, -%c\t\tprint prime factors in raw format\n", longopts[2].name, longopts[2].val);
                printf("\t--%s, -%c\t\tprint prime factors on separate lines\n", longopts[4].name, longopts[4].val);
                printf("\t--%s\t\tprint some useful benchmarks and exit\n", longopts[3].name);
                printf("\t--%s, -%c\t\tprint version and exit\n", longopts[1].name, longopts[1].val);
                printf("\t--%s, -%c\t\tprint this message and exit\n", longopts[0].name, longopts[0].val);
                return 0;
            case 'v':
                printf("%s v. 1.1\n", argv[0]);
                return 0;
            case 'r':
                raw = 1;
                break;
            case 'B':
                printf("these should require the greatest amount of time:\n");
                printf("%llu = 2^64 - 59 (the largest prime number < 2^64)\n", (uint64_t) -59);
                printf("%llu = (2^32 - 5)^2 (the largest prime square < 2^64)\n", (((uint64_t) 1 << 32) - 5) * (((uint64_t) 1 << 32) - 5));
                printf("these are rather technological:\n");
                printf("%llu = 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47\n",
                    (uint64_t) 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47);
                printf("%llu = 2^32 + 15 (the first prime number > 2^32)\n", ((uint64_t) 1 << 32) + 15);
                return 0;
            case 'n':
                linebreak = 1;
                break;
        }
    }

    for (i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            ++input;
            if (sscanf(argv[i], "%llu", &n)) {
                k = factor(n, d, &r);
                if (linebreak) {
                    printf("%llu\n", n);
                    print(k, d, r);
                }
                else {
                    printf("%llu:", n);
                    print(k, d, r);
                    printf("\n");
                }
            }
            else {
                printf("\"%s\": incorrect argument\n", argv[i]);
            }
        }
    }

    if (input) {
        return 0;
    }
    else {
        while (1) {
            scanf("%20s", s);
            if (sscanf(s, "%llu", &n)) {
                k = factor(n, d, &r);
                if (linebreak) {
                    print(k, d, r);
                }
                else {
                    printf("%llu:", n);
                    print(k, d, r);
                    printf("\n");
                }
            }
            else {
                printf("\"%s\": incorrect argument\n", s);
            }
        }
    }
}
