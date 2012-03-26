#include <getopt.h>
#include <stdio.h>
#include <factor.h>

int main (int argc, char* argv[]) {
    const struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"threads", required_argument, 0, 'j'},
        {0, 0, 0, 0}
    };
    char ch;
    int longind;

    const uint32_t small_primes[] = {
#include "small_primes.csv"
    };
    const uint32_t K = sizeof(small_primes) / sizeof(uint32_t);
    uint32_t P = 1;
    uint32_t i;
    uint32_t n;
    uint32_t coprime;
    uint32_t A;
    struct degree d[9];
    const char a_name[] = "a.csv";
    const char D_name[] = "D.h";
    FILE* a;
    FILE* D;

    while ((ch = getopt_long(argc, argv, "hv", longopts, &longind)) != -1) {
        switch (ch) {
            case 'h':
                printf("%s [ --%s=NUMBER | --%s | --%s ]\n",
                    argv[0],
                    longopts[2].name,
                    longopts[1].name,
                    longopts[0].name);
                printf("\n");
                printf("\t--%s=NUMBER\tsplit into NUMBER threads (unlimited by default)\n", longopts[2].name);
                printf("\t--%s, -%c\t\tprint version and exit\n", longopts[1].name, longopts[1].val);
                printf("\t--%s, -%c\t\tprint this message and exit\n", longopts[0].name, longopts[0].val);
                return 0;
            case 'v':
                printf("%s v. 1.3\n", argv[0]);
                return 0;
            case 'j':
                if (!sscanf(optarg, "%u", &factor_threads)) {
                    printf("\"%s\": incorrect argument\n", optarg);
                }
                break;
        }
    }

    if ((a = fopen(a_name, "w")) == 0) {
        printf("error: can not open \"%s\" for writing, exiting\n", a_name);
        return 1;
    }
    if ((D = fopen(D_name, "w")) == 0) {
        printf("error: can not open \"%s\" for writing, exiting\n", D_name);
        return 1;
    }
    for (i = 0; i < K; ++i) {
        P *= small_primes[i];
    }
    fprintf(D, "#define D %u", P);
    fclose(D);
    for (A = 1; 1; ++A) {
        if (factor(1 + A * P, d, 0) == 1) {
            if (d[0].b == 1) {
                break;
            }
        }
    }
    if (A == 1) {
        fprintf(a, "1 + %u", P);
    }
    else {
        fprintf(a, "1 + %u * %u", A, P);
    }
    for (n = 2; n < P; ++n) {
        coprime = 1;
        for (i = 0; i < K; ++i) {
            if (n % small_primes[i] == 0) {
                coprime = 0;
                break;
            }
        }
        if (!coprime) {
            continue;
        }
        for (A = 0; 1; ++A) {
            if (factor(n + A * P, d, 0) == 1) {
                if (d[0].b == 1) {
                    break;
                }
            }
        }
        fprintf(a, ",\n%u", n);
        if (A != 0) {
            if (A == 1) {
                fprintf(a, " + %u", P);
            }
            else {
                fprintf(a, " + %u * %u", A, P);
            }
        }
    }
    fclose(a);
    return 0;
}
