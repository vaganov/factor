#!/usr/bin/python

import sys
from ctypes import *
from optparse import OptionParser

class degree (Structure): # factor.h
    _fields_ = [("p", c_uint32),
                ("b", c_uint32)]

def main ():
    parser = OptionParser()
    parser.add_option("--bootstrap",
                      action = "store_true",
                      dest = "bootstrap",
                      default = False,
                      help = "use libfactor.so to build better sieve")
    (options, args) = parser.parse_args()

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

    if options.bootstrap:
        libname = "./libfactor.so"
        try:
            libfactor = cdll.LoadLibrary(libname)
        except OSError:
            sys.stderr.write("cannot link \"%s\"\n" % libname)
            return 1
        factor = libfactor.factor # factor.h
        factor.argtypes = [c_uint64, POINTER(degree), POINTER(c_uint64)]
        factor.restype = c_uint32

    file = open("a.csv", "w")
    if options.bootstrap:
        A = 1
        while True:
            d = (degree * 9)() # enough for n < 2^32
            k = factor(c_uint64(1 + A * P), d, None)
            if k == 1:
                if d[0].b == 1:
                    if A > 1:
                        file.write("%d + %d * %d" % (1, A, P))
                    else:
                        file.write("%d + %d" % (1, P))
                    break
            A += 1
    else:
        file.write("%d + %d" % (1, P))
    for n in range(2, P):
        for p in small_primes:
            if n % p == 0:
                break
        else:
            if options.bootstrap:
                A = 0
                while True:
                    d = (degree * 9)() # enough for n < 2^32
                    k = factor(c_uint64(n + A * P), d, None)
                    if k == 1:
                        if d[0].b == 1:
                            if A > 0:
                                if A > 1:
                                    file.write(",\n%d + %d * %d" % (n, A, P))
                                else:
                                    file.write(",\n%d + %d" % (n, P))
                            else:
                                file.write(",\n%d" % n)
                            break
                    A += 1
            else:
                file.write(",\n%d" % n)
    file.close()

    return 0

if __name__ == "__main__":
    main()
