#!/usr/bin/python

file = open("small_primes.csv", "r")
string = file.readline()
file.close()
small_primes = []
for substring in string.split(", "):
    small_primes.append(int(substring))
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