SQUIRREL= squirrel
SRC= src
CFLAGS = -O2 -Wall

OUT= bin/sq_imp
INCZ= -I$(SQUIRREL)/include -I. -I$(SQUIRREL)/sqlibs -Iinclude
LIBZ= -L$(SQUIRREL)/lib
LIB= -lsquirrel -lsqstdlib

SRCS= $(SRC)/prisioners_dilemma.c $(SRC)/squirrel_imp.c

# TODO: Clean up file (call Squirrel's makefile)

64:
	g++ -O2 -D_SQ64 -Wall -o $(OUT) $(SRCS) $(INCZ) $(LIBZ) $(LIB)
