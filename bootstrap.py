#!/usr/bin/python

file = open("small_primes.csv", "r")
small_primes_string = file.readline()
file.close()
small_primes = []
for small_primes_string_substring in small_primes_string.split(", "):
    small_primes.append(int(small_primes_string_substring))
P = 1
for p in small_primes:
    P *= p
file = open("D.h", "w")
file.write("#define D %d" % P)
file.close()
file = open("a.csv", "w")
file.write("%d + %d" % (1, P))
for n in range(2, P):
    for p in small_primes:
        if n % p == 0:
            break
    else:
        file.write(",\n%d" % n)
file.close()
