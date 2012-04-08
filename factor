#!/usr/bin/python

import sys
from ctypes import *
from optparse import *

usage = "%prog [ [ --raw ] [ --linebreak ] [ --threads=NUMBER ] NUMBER [ NUMBER ... ] | --benchmark | --version | --help ]"
version = "%prog 1.5"
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
                  metavar = "NUMBER",
                  help = "split into NUMBER threads (unlimited by default)")
parser.add_option("--benchmark",
                  action = "store_true",
                  dest = "benchmark",
                  default = False,
                  help = "print some useful benchmarks and exit")
(options, args) = parser.parse_args()

if options.benchmark:
    benchmark = [2 ** 64 - 59,
                 (2 ** 32 - 5) ** 2,
                 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47,
                 2 ** 32 + 15]
    sys.stdout.write("these should require the greatest amount of time:\n")
    sys.stdout.write("%d = 2^64 - 59 (the largest prime number < 2^64)\n" % benchmark[0])
    sys.stdout.write("%d = (2^32 - 5)^2 (the largest prime square < 2^64)\n" % benchmark[1])
    sys.stdout.write("these are rather technological:\n")
    sys.stdout.write("%d = 2 * 3 * 5 * 7 * 11 * 13 * 17 * 19 * 23 * 29 * 31 * 37 * 41 * 43 * 47\n" % benchmark[2])
    sys.stdout.write("%d = 2^32 + 15 (the first prime number > 2^32)\n" % benchmark[3])
    sys.exit(0)

libfactor = cdll.LoadLibrary("libfactor.so")

if options.threads != None:
    threads = c_uint32(options.threads)
    set_factor_threads = libfactor.set_factor_threads # factor.h
    set_factor_threads.argtypes = [c_uint32]
    set_factor_threads.restype = None
    set_factor_threads(threads)

class degree(Structure):
    _fields_ = [("p", c_uint32),
                ("b", c_uint32)]

factor = libfactor.factor # factor.h
factor.argtypes = [c_uint64, POINTER(degree), POINTER(c_uint64)]
factor.restype = c_uint32

if len(args) == 0:
    while True:
        n = c_uint64(long(raw_input()))
        d = (degree * 15)() # enough for n < 2^64
        r = c_uint64()
        k = factor(n, d, byref(r))
        if not options.linebreak:
            sys.stdout.write("%d:" % n.value)
        for i in range(k):
            if d[i].b == 1:
                if options.linebreak:
                    sys.stdout.write("\t")
                else:
                    sys.stdout.write(" ")
                sys.stdout.write("%d" % d[i].p)
                if options.linebreak:
                    sys.stdout.write("\n")
            else:
                if options.raw:
                    for j in range(d[i].b):
                        if options.linebreak:
                            sys.stdout.write("\t")
                        else:
                            sys.stdout.write(" ")
                        sys.stdout.write("%d" % d[i].p)
                        if options.linebreak:
                            sys.stdout.write("\n")
                else:
                    if options.linebreak:
                        sys.stdout.write("\t")
                    else:
                        sys.stdout.write(" ")
                        if k > 1:
                            sys.stdout.write("(")
                    sys.stdout.write("%d ^ %d" % (d[i].p, d[i].b))
                    if options.linebreak:
                        sys.stdout.write("\n")
                    else:
                        if k > 1:
                            sys.stdout.write(")")
        if k == 0 or r.value != 1:
            if options.linebreak:
                sys.stdout.write("\t")
            else:
                sys.stdout.write(" ")
            sys.stdout.write("%d" % r.value)
            if options.linebreak:
                sys.stdout.write("\n")
        if not options.linebreak:
            sys.stdout.write("\n")
else:
    for arg in args:
        n = c_uint64(long(arg))
        d = (degree * 15)() # enough for n < 2^64
        r = c_uint64()
        k = factor(n, d, byref(r))
        sys.stdout.write("%d" % n.value)
        if options.linebreak:
            sys.stdout.write("\n")
        else:
            sys.stdout.write(":")
        for i in range(k):
            if d[i].b == 1:
                if options.linebreak:
                    sys.stdout.write("\t")
                else:
                    sys.stdout.write(" ")
                sys.stdout.write("%d" % d[i].p)
                if options.linebreak:
                    sys.stdout.write("\n")
            else:
                if options.raw:
                    for j in range(d[i].b):
                        if options.linebreak:
                            sys.stdout.write("\t")
                        else:
                            sys.stdout.write(" ")
                        sys.stdout.write("%d" % d[i].p)
                        if options.linebreak:
                            sys.stdout.write("\n")
                else:
                    if options.linebreak:
                        sys.stdout.write("\t")
                    else:
                        sys.stdout.write(" ")
                        if k > 1:
                            sys.stdout.write("(")
                    sys.stdout.write("%d ^ %d" % (d[i].p, d[i].b))
                    if options.linebreak:
                        sys.stdout.write("\n")
                    else:
                        if k > 1:
                            sys.stdout.write(")")
        if k == 0 or r.value != 1:
            if options.linebreak:
                sys.stdout.write("\t")
            else:
                sys.stdout.write(" ")
            sys.stdout.write("%d" % r.value)
            if options.linebreak:
                sys.stdout.write("\n")
        if not options.linebreak:
            sys.stdout.write("\n")
