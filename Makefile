CC = gcc
CFLAGS = -c -O3 -ansi -fPIC
LIBS = -lpthread
LDFLAGS = -shared -mimpure-text
AR = ar
ARFLAGS = -rc

ess_objs = seek.o launch.o factor.o
objs = main.o print.o $(ess_objs)

default: factor
shared: libfactor.so
static: libfactor.a
factor: $(objs) Makefile
	$(CC) $(LIBS) $(objs) -o factor
libfactor.so: $(ess_objs) Makefile
	$(CC) $(LDFLAGS) $(LIBS) $(ess_objs) -o libfactor.so
libfactor.a: $(ess_objs) Makefile
	$(AR) $(ARFLAGS) libfactor.a $(ess_objs)
main.o: main.c typedef.h degree.h factor.h print.h Makefile
	$(CC) $(CFLAGS) main.c -o main.o
seek.o: seek.c seek.h typedef.h Makefile
	$(CC) $(CFLAGS) seek.c -o seek.o
launch.o: launch.c launch.h launch_arg.h typedef.h seek.h Makefile
	$(CC) $(CFLAGS) launch.c -o launch.o
factor.o: factor.c factor.h typedef.h degree.h launch_arg.h launch.h seek.h Makefile
	$(CC) $(CFLAGS) factor.c -o factor.o
print.o: print.c print.h typedef.h degree.h
	$(CC) $(CFLAGS) print.c -o print.o
clean:
	rm -f $(objs)
