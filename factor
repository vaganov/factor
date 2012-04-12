#!/usr/bin/python

import sys
import os
from ctypes import *
from optparse import OptionParser

class degree (Structure): # factor.h
    _fields_ = [("p", c_uint32),
                ("b", c_uint32)]

def main ():
    usage = "%prog [ [ --raw ] [ --linebreak ] [ --threads=NUMBER ] NUMBER [ NUMBER ... ] | --benchmark | --version | --help ]"
    version = "%prog 1.7"
    parser = OptionParser(usage = usage, version = version)
    parser.add_option("--raw", "-r",
                      action = "store_true",
                      dest = "raw",
                      default = False,
                      help = "print prime factors in raw format")
    parser.add_option("--linebreak", "-n",
                      action = "store_true",
                      dest = "linebreak",
                      default = False,
                      help = "print prime factors on separate lines")
    parser.add_option("--threads", "-j",
                      action = "store",
                      type = "int",
                      dest = "threads",
                      default = os.sysconf('SC_NPROCESSORS_ONLN'),
                      metavar = "NUMBER",
                      help = "split into NUMBER threads (number of processors by default)")
    parser.add_option("--benchmark",
                      action = "store_true",
                      dest = "benchmark",
                      default = False,
                      help = "print some useful benchmarks and exit")
    (options, args) = parser.parse_args()

    if options.benchmark:
        print_benchmark()
        return 0

    libname = "libfactor.so"
    try:
        libfactor = cdll.LoadLibrary(libname)
    except OSError:
        sys.stderr.write("cannot link \"%s\" (check LD_LIBRARY_PATH)\n" % libname)
        return 1

    if options.threads < 0:
        sys.stderr.write("number of threads cannot be negative\n")
        return 1
    if options.threads < 2 ** 32:
        threads = c_uint32(options.threads)
    else:
        threads = c_uint32(0) # unlimited number of threads
    set_factor_threads = libfactor.set_factor_threads # factor.h
    set_factor_threads.argtypes = [c_uint32]
    set_factor_threads.restype = None
    set_factor_threads(threads)

    factor = libfactor.factor # factor.h
    factor.argtypes = [c_uint64, POINTER(degree), POINTER(c_uint64)]
    factor.restype = c_uint32

    if len(args) == 0:
        while True:
            try:
                arg = raw_input()
            except KeyboardInterrupt:
                sys.stdout.write("\n")
                return 0
            (valid, n) = str_to_uint64(arg)
            if not valid:
                continue
            d = (degree * 15)() # enough for n < 2^64
            r = c_uint64()
            k = factor(n, d, byref(r))
            print_factorization(n, k, d, r, options.linebreak, options.raw, False)
    else:
        for arg in args:
            (valid, n) = str_to_uint64(arg)
            if not valid:
                continue
            d = (degree * 15)() # enough for n < 2^64
            r = c_uint64()
            k = factor(n, d, byref(r))
            print_factorization(n, k, d, r, options.linebreak, options.raw, True)
        return 0

def str_to_uint64 (string):
    try:
        number = long(string)
    except ValueError:
        sys.stderr.write("\"%s\": incorrect input\n" % string)
        return (False, None)
    if number < 0:
        sys.stderr.write("%d is negative\n" % number)
        return (False, None)
    if number >= 2 ** 64:
        sys.stderr.write("%d is too large (must be less than 2 ^ 64)\n" % number)
        return (False, None)
    return (True, c_uint64(number))

def print_factorization (n, k, d, r, linebreak, raw, print_input):
    if print_input:
        sys.stdout.write("%d" % n.value)
        if linebreak:
            sys.stdout.write("\n")
        else:
            sys.stdout.write(":")
    else:
        if not linebreak:
            sys.stdout.write("%d:" % n.value)
    for i in range(k):
        if d[i].b == 1:
            if linebreak:
                sys.stdout.write("\t")
            else:
                sys.stdout.write(" ")
            sys.stdout.write("%d" % d[i].p)
            if linebreak:
                sys.stdout.write("\n")
        else:
            if raw:
                for j in range(d[i].b):
                    if linebreak:
                        sys.stdout.write("\t")
                    else:
                        sys.stdout.write(" ")
                    sys.stdout.write("%d" % d[i].p)
                    if linebreak:
                        sys.stdout.write("\n")
            else:
                if linebreak:
                    sys.stdout.write("\t")
                else:
                    sys.stdout.write(" ")
                    if k > 1:
                        sys.stdout.write("(")
                sys.stdout.write("%d ^ %d" % (d[i].p, d[i].b))
                if linebreak:
                    sys.stdout.write("\n")
                else:
                    if k > 1:
                        sys.stdout.write(")")
    if k == 0 or r.value != 1:
        if linebreak:
            sys.stdout.write("\t")
        else:
            sys.stdout.write(" ")
        sys.stdout.write("%d" % r.value)
        if linebreak:
            sys.stdout.write("\n")
    if not linebreak:
        sys.stdout.write("\n")

def print_benchmark ():
    benchmark = [2 ** 64 - 59,
                 (2 ** 32 - 5) ** 2,
                 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47,
                 2 ** 32 + 15,
                 10 ** 14 - 41,
                 (10 ** 7 - 9) ** 2]
    sys.stdout.write("these should require the greatest amount of time:\n")
    sys.stdout.write("%d = 2^64 - 59 (the largest prime number < 2^64)\n" % benchmark[0])
    sys.stdout.write("%d = (2^32 - 5)^2 (the largest prime square < 2^64)\n" % benchmark[1])
    sys.stdout.write("these are rather technological:\n")
    sys.stdout.write("%d = 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47\n" % benchmark[2])
    sys.stdout.write("%d = 2^32 + 15 (the first prime number > 2^32)\n" % benchmark[3])
    sys.stdout.write("these are Solaris-specific\n")
    sys.stdout.write("%d = 10^14 - 41 (the largest prime number < 10^14)\n" % benchmark[4])
    sys.stdout.write("%d = (10^7 - 9)^2 (the largest prime square < 10^14)\n" % benchmark[5])

if __name__ == "__main__":
    main()
