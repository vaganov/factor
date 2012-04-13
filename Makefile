CC = gcc
CFLAGS = -ansi -c -I. -O3 -fPIC
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

objs = seek.o factor_thread_routine.o factor.o set_factor_threads.o factor_globals.o factor_threads_mutex.o

default: libfactor.so libfactor.a
shared: libfactor.so
static: libfactor.a
libfactor.so: $(objs)
	$(CC) $(LDFLAGS) $(LIBS) $(objs) -o libfactor.so
libfactor.a: $(objs)
	$(AR) $(ARFLAGS) libfactor.a $(objs)
seek.o: seek.c seek.h D.h
	$(CC) $(CFLAGS) seek.c -o seek.o
factor_thread_routine.o: factor_thread_routine.c factor_thread_routine.h factor_thread_routine_arg.h seek.h
	$(CC) $(CFLAGS) factor_thread_routine.c -o factor_thread_routine.o
factor.o: factor.c factor.h small_primes.csv a.csv factor_globals.h factor_thread_routine_arg.h factor_thread_routine.h
	$(CC) $(CFLAGS) factor.c -o factor.o
set_factor_threads.o: set_factor_threads.c factor.h factor_globals.h
	$(CC) $(CFLAGS) set_factor_threads.c -o set_factor_threads.o
factor_globals.o: factor_globals.c factor_globals.h
	$(CC) $(CFLAGS) factor_globals.c -o factor_globals.o
factor_threads_mutex.o: factor_threads_mutex.c factor_globals.h
	$(CC) $(CFLAGS) factor_threads_mutex.c -o factor_threads_mutex.o
D.h:	sieve small_primes.csv
	./sieve
a.csv:	sieve small_primes.csv
	./sieve
bootstrap: libfactor.so
	./sieve
	make
clean:
	rm -f $(objs)
	rm -f D.h
	rm -f a.csv
