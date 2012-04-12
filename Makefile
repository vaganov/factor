CC = gcc
CFLAGS = -ansi -c -I. -O3 -fPIC
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

lib_objs = seek.o factor_thread_routine.o factor.o set_factor_threads.o factor_globals.o factor_threads_mutex.o

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
factor.o: factor.c factor.h small_primes.csv a.csv factor_globals.h factor_thread_routine_arg.h factor_thread_routine.h Makefile
	$(CC) $(CFLAGS) factor.c -o factor.o
set_factor_threads.o: set_factor_threads.c factor.h factor_globals.h Makefile
	$(CC) $(CFLAGS) set_factor_threads.c -o set_factor_threads.o
factor_globals.o: factor_globals.c factor_globals.h Makefile
	$(CC) $(CFLAGS) factor_globals.c -o factor_globals.o
factor_threads_mutex.o: factor_threads_mutex.c factor_globals.h Makefile
	$(CC) $(CFLAGS) factor_threads_mutex.c -o factor_threads_mutex.o
D.h:	bootstrap.py small_primes.csv Makefile
	./bootstrap.py
a.csv:	bootstrap.py small_primes.csv Makefile
	./bootstrap.py
bootstrap: libfactor.so
	./bootstrap.py
	make
clean:
	rm -f $(lib_objs)
	rm -f D.h
	rm -f a.csv
