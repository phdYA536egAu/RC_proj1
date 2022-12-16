SHELL = /bin/sh

OBJS =   player.c
CFLAG = -Wall -g
CC = gcc
INCLUDE =
LIBS = -lm


player:
	${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f *.o core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<

