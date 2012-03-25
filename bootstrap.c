#include <stdio.h>
#include <factor.h>

int main (void) {
    const uint32_t small_primes[] = {2, 3, 5, 7, 11};
    const uint32_t k = sizeof(small_primes) / sizeof(uint32_t);
    uint32_t P = 1;
    uint32_t i;
    uint32_t n;
    uint32_t coprime;
    uint32_t A;
    struct degree d[9];
    const char a_name[] = "a.h";
    const char D_name[] = "D.h";
    FILE* a = fopen(a_name, "w");
    FILE* D = fopen(D_name, "w");
    if (a == 0) {
        printf("error: can not open \"%s\" for writing, exiting\n", a_name);
        return 1;
    }
    if (D == 0) {
        printf("error: can not open \"%s\" for writing, exiting\n", D_name);
        return 1;
    }
    for (i = 0; i < k; ++i) {
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
        for (i = 0; i < k; ++i) {
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
