#!/usr/bin/python

import sys
from ctypes import *
from optparse import *

parser = OptionParser(version = "%prog 1.5")
(options, args) = parser.parse_args()

libfactor = cdll.LoadLibrary("./libfactor.so")
factor = libfactor.factor
factor.restype = c_uint

class degree(Structure):
    _fields_ = [("p", c_uint),
                ("b", c_uint)]

#n = c_ulonglong(18446744030759878681)
#n = c_ulonglong(18446744073709551557)
n = c_ulonglong(12)
d = (degree * 15)()
r = c_ulonglong()
k = factor(n, byref(d), byref(r))
for i in range(k):
    print "\t%d ^ %d" % (d[i].p, d[i].b)
print r

#for arg in sys.argv[1:]:
#    print int(arg)
