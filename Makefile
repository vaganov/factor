CC = gcc
CFLAGS = -ansi -c -O3 -fPIC
LIBS = -lpthread -lgmp
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc
MKDIR = mkdir -p
INSTALL = install
RM = rm -f

prefix = /usr/local
bin_dir = $(prefix)/bin
lib_dir = $(prefix)/lib
include_dir = $(prefix)/include
man_dir = $(prefix)/share/man
man1_dir = $(man_dir)/man1
man3_dir = $(man_dir)/man3

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
	./sieve --bootstrap
	make
install:
	$(MKDIR) $(bin_dir)
	$(INSTALL) -m 0755 factor $(bin_dir)
	$(MKDIR) $(lib_dir)
	$(INSTALL) -m 0755 libfactor.so $(lib_dir)
	$(INSTALL) -m 0644 libfactor.a $(lib_dir)
	$(MKDIR) $(include_dir)
	$(INSTALL) -m 0644 factor.h $(include_dir)
	$(MKDIR) $(man1_dir)
	$(INSTALL) -m 0644 factor.1 $(man1_dir)
	$(MKDIR) $(man3_dir)
	$(INSTALL) -m 0644 factor.3 $(man3_dir)
	$(INSTALL) -m 0644 set_factor_threads.3 $(man3_dir)
clean:
	$(RM) *~
	$(RM) $(objs)
	$(RM) D.h
	$(RM) a.csv
