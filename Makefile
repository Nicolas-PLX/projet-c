CC = gcc
CFLAGS = -Wall -g -pedantic
CLIBS = -lm

projet : calc_unbounded_int.o unbounded_int.o

unbounded_int.o : unbounded_int.c unbounded_int.h
calc_unbounded_int.o : calc_unbounded_int.c unbounded_int.h

clean : 
	rm -rf *~
cleanall :
	rm -rf *~ *.o projet