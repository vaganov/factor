#include <getopt.h>
#include <iostream>
#include <stdio.h>
#include "factor.h"
#include "globals.h"
#include "print.h"

int main (int argc, char* argv[]) {
    const struct option longopts[] = {
        {"help", no_argument, 0, 'h'},
        {"version", no_argument, 0, 'v'},
        {"raw", no_argument, 0, 'r'},
        {"benchmark", no_argument, 0, 'B'},
        {"linebreak", no_argument, 0, 'n'},
        {"threads", required_argument, 0, 'j'},
        {0, 0, 0, 0}
    };
    char ch;
    int longind;
    while ((ch = getopt_long(argc, argv, "hvrn", longopts, &longind)) != -1) {
        switch (ch) {
            case 'h':
                std::cout << argv[0] << " [ [ --" << longopts[2].name << " ] [ --" << longopts[4].name << " ] [ --" << longopts[5].name << "=NUMBER ] NUMBER [ NUMBER ... ] | --" << longopts[3].name << " | --" << longopts[1].name << " | --" << longopts[0].name << " ]\n";
                std::cout << "\n";
                std::cout << "print the prime factors of each specified NUMBER\n";
                std::cout << "if none are specified, read them from standard input\n";
                std::cout << "\n";
                std::cout << "\t--" << longopts[2].name << ", -" << (char) longopts[2].val << "\t\tprint prime factors in raw format\n";
                std::cout << "\t--" << longopts[4].name << ", -" << (char) longopts[4].val << "\t\tprint prime factors on separate lines\n";
                std::cout << "\t--" << longopts[5].name << "=NUMBER\tsplit into NUMBER threads (unlimited by default)\n";
                std::cout << "\t--" << longopts[3].name << "\t\tprint some useful benchmarks and exit\n";
                std::cout << "\t--" << longopts[1].name << ", -" << (char) longopts[1].val << "\t\tprint version and exit\n";
                std::cout << "\t--" << longopts[0].name << ", -" << (char) longopts[0].val << "\t\tprint this message and exit\n";
                return 0;
            case 'v':
                std::cout << argv[0] << " v. 1.4\n";
                return 0;
            case 'r':
                raw = true;
                break;
            case 'B':
                std::cout << "these should require the greatest amount of time:\n";
                std::cout << (uint64_t) -59 << " = 2^64 - 59 (the largest prime number < 2^64)\n";
                std::cout << (((uint64_t) 1 << 32) - 5) * (((uint64_t) 1 << 32) - 5) << " = (2^32 - 5)^2 (the largest prime square < 2^64)\n";
                std::cout << "these are rather technological:\n";
                std::cout << (uint64_t) 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47 << " = 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47\n";
                std::cout << ((uint64_t) 1 << 32) + 15 << " = 2^32 + 15 (the first prime number > 2^32)\n";
                return 0;
            case 'n':
                linebreak = true;
                break;
            case 'j':
                uint32_t threads;
                if (sscanf(optarg, "%u", &threads) != 0) {
                    set_factor_threads(threads);
                }
                else {
                    std::cout << "\"" << optarg << "\": incorrect argument\n";
                }
                break;
        }
    }

    bool input = false;
    for (int i = 1; i < argc; ++i) {
        if (argv[i][0] != '-') {
            input = true;
            uint64_t n;
            if (sscanf(argv[i], "%llu", &n) != 0) {
                struct degree d[15];
                uint64_t r;
                uint32_t k = factor(n, d, &r);
                if (linebreak) {
                    std::cout << n << "\n";
                    print(k, d, r);
                }
                else {
                    std::cout << n << ":";
                    print(k, d, r);
                    std::cout << "\n";
                }
            }
            else {
                std::cout << "\"" << argv[i] << "\": incorrect argument\n";
            }
        }
    }

    if (input) {
        return 0;
    }
    else {
        while (true) {
            std::string s;
            std::cin >> s;
            uint64_t n;
            if (sscanf(s.c_str(), "%llu", &n)) {
                struct degree d[15];
                uint64_t r;
                uint32_t k = factor(n, d, &r);
                if (linebreak) {
                    print(k, d, r);
                }
                else {
                    std::cout << n << ":";
                    print(k, d, r);
                    std::cout << "\n";
                }
            }
            else {
                std::cout << "\"" << s << "\": incorrect argument\n";
            }
        }
    }
}
