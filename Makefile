CC = gcc
CFLAGS = -ansi -c -I. -O3 -fPIC
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

lib_objs = seek.o factor_thread_routine.o factor.o set_factor_threads.o factor_globals.o

default: libfactor.so libfactor.a
shared: libfactor.so
static: libfactor.a
libfactor.so: $(lib_objs) Makefile
	$(CC) $(LDFLAGS) $(LIBS) $(lib_objs) -o libfactor.so
libfactor.a: $(lib_objs) Makefile
	$(AR) $(ARFLAGS) libfactor.a $(lib_objs)
seek.o: seek.c seek.h D.h Makefile
	$(CC) $(CFLAGS) seek.c -o seek.o
factor_thread_routine.o: factor_thread_routine.c factor_thread_routine.h factor_thread_routine_arg.h seek.h Makefile
	$(CC) $(CFLAGS) factor_thread_routine.c -o factor_thread_routine.o
factor.o: factor.c factor.h D.h small_primes.csv a.csv factor_globals.h factor_thread_routine_arg.h factor_thread_routine.h Makefile
	$(CC) $(CFLAGS) factor.c -o factor.o
set_factor_threads.o: set_factor_threads.c factor.h factor_globals.h
	$(CC) $(CFLAGS) set_factor_threads.c -o set_factor_threads.o
factor_globals.o: factor_globals.c factor_globals.h Makefile
	$(CC) $(CFLAGS) factor_globals.c -o factor_globals.o
D.h:	bootstrap small_primes.csv
	./bootstrap
a.csv:	bootstrap small_primes.csv
	./bootstrap
clean:
	rm -f $(lib_objs)
