CC = gcc
CFLAGS = -Wall -g -pedantic
CLIBS = -lm

ALL = calc_unbounded_int test_unbounded

all:$(ALL)

calc_unbounded_int : calc_unbounded_int.o unbounded_int.o
test_unbounded_int : test_unbounded_int.o unbounded_int.o

unbounded_int.o : unbounded_int.c unbounded_int.h
calc_unbounded_int.o : calc_unbounded_int.c unbounded_int.h
test_unbounded_int.o : test_unbounded_int.c unbounded_int.h

clean :
	rm -rf *~ *.o
cleanall :
	rm -rf *~ *.o $(ALL)
