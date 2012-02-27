CC = gcc
CFLAGS = -c -I. -O3 -ansi -fPIC
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

lib_objs = seek.o launch.o factor.o
bin_objs = main.o globals.o print.o

default: factor libfactor.so
shared: libfactor.so
static: libfactor.a
factor: $(bin_objs) libfactor.a Makefile
	$(CC) $(LIBS) $(bin_objs) libfactor.a -o factor
libfactor.so: $(lib_objs) Makefile
	$(CC) $(LDFLAGS) $(LIBS) $(lib_objs) -o libfactor.so
libfactor.a: $(lib_objs) Makefile
	$(AR) $(ARFLAGS) libfactor.a $(lib_objs)
main.o: main.c factor.h print.h globals.h Makefile
	$(CC) $(CFLAGS) main.c -o main.o
seek.o: seek.c seek.h Makefile
	$(CC) $(CFLAGS) seek.c -o seek.o
launch.o: launch.c launch.h launch_arg.h seek.h Makefile
	$(CC) $(CFLAGS) launch.c -o launch.o
factor.o: factor.c factor.h launch_arg.h launch.h seek.h Makefile
	$(CC) $(CFLAGS) factor.c -o factor.o
print.o: print.c print.h factor.h globals.h Makefile
	$(CC) $(CFLAGS) print.c -o print.o
globals.o: globals.c globals.h Makefile
	$(CC) $(CFLAGS) globals.c -o globals.o
clean:
	rm -f $(lib_objs)
	rm -f $(bin_objs)
