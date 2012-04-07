CC = gcc
CFLAGS = -ansi -c -I. -O3 -fPIC
CXX = g++
CXXFLAGS = -c -I. -O3
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

lib_objs = seek.o factor_thread_routine.o factor.o factor_globals.o
bin_objs = main.o globals.o print.o

default: factor libfactor.so
shared: libfactor.so
static: libfactor.a
factor: $(bin_objs) libfactor.a Makefile
	$(CXX) $(LIBS) $(bin_objs) libfactor.a -o factor
libfactor.so: $(lib_objs) Makefile
	$(CC) $(LDFLAGS) $(LIBS) $(lib_objs) -o libfactor.so
libfactor.a: $(lib_objs) Makefile
	$(AR) $(ARFLAGS) libfactor.a $(lib_objs)
main.o: main.cxx factor.h print.h globals.h Makefile
	$(CXX) $(CXXFLAGS) main.cxx -o main.o
seek.o: seek.c seek.h D.h Makefile
	$(CC) $(CFLAGS) seek.c -o seek.o
factor_thread_routine.o: factor_thread_routine.c factor_thread_routine.h factor_thread_routine_arg.h seek.h Makefile
	$(CC) $(CFLAGS) factor_thread_routine.c -o factor_thread_routine.o
factor.o: factor.c factor.h D.h small_primes.csv a.csv factor_thread_routine_arg.h factor_thread_routine.h Makefile
	$(CC) $(CFLAGS) factor.c -o factor.o
factor_globals.o: factor_globals.c factor.h Makefile
	$(CC) $(CFLAGS) factor_globals.c -o factor_globals.o
print.o: print.cxx print.h factor.h globals.h Makefile
	$(CXX) $(CXXFLAGS) print.cxx -o print.o
globals.o: globals.cxx globals.h Makefile
	$(CXX) $(CXXFLAGS) globals.cxx -o globals.o
D.h:	bootstrap.py small_primes.csv
	./bootstrap.py
a.csv:	bootstrap.py small_primes.csv
	./bootstrap.py
clean:
	rm -f $(lib_objs)
	rm -f $(bin_objs)
