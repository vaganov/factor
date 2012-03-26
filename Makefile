CC = gcc
CFLAGS = -c -I. -O3 -ansi -fPIC
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

lib_objs = seek.o factor_thread_routine.o factor.o factor_globals.o
bin_objs = main.o globals.o print.o
bootstrap_obj = bootstrap.o

default: factor libfactor.so
shared: libfactor.so
static: libfactor.a
factor: $(bin_objs) libfactor.a Makefile
	$(CC) $(LIBS) $(bin_objs) libfactor.a -o factor
libfactor.so: $(lib_objs) Makefile
	$(CC) $(LDFLAGS) $(LIBS) $(lib_objs) -o libfactor.so
libfactor.a: $(lib_objs) Makefile
	$(AR) $(ARFLAGS) libfactor.a $(lib_objs)
bootstrap: $(bootstrap_obj) libfactor.a Makefile
	$(CC) $(LIBS) $(bootstrap_obj) libfactor.a -o bootstrap
main.o: main.c factor.h print.h globals.h Makefile
	$(CC) $(CFLAGS) main.c -o main.o
seek.o: seek.c seek.h D.h Makefile
	$(CC) $(CFLAGS) seek.c -o seek.o
factor_thread_routine.o: factor_thread_routine.c factor_thread_routine.h factor_thread_routine_arg.h seek.h Makefile
	$(CC) $(CFLAGS) factor_thread_routine.c -o factor_thread_routine.o
factor.o: factor.c factor.h D.h a.csv factor_thread_routine_arg.h factor_thread_routine.h Makefile
	$(CC) $(CFLAGS) factor.c -o factor.o
factor_globals.o: factor_globals.c factor.h Makefile
	$(CC) $(CFLAGS) factor_globals.c -o factor_globals.o
print.o: print.c print.h factor.h globals.h Makefile
	$(CC) $(CFLAGS) print.c -o print.o
globals.o: globals.c globals.h Makefile
	$(CC) $(CFLAGS) globals.c -o globals.o
bootstrap.o: bootstrap.c factor.h small_primes.csv Makefile
	$(CC) $(CFLAGS) bootstrap.c -o bootstrap.o
clean:
	rm -f $(lib_objs)
	rm -f $(bin_objs)
	rm -f $(bootstrap_obj)
