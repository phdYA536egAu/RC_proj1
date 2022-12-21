SHELL = /bin/sh

OBJS =   player.c
CFLAG = -Wall -g
CC = gcc
INCLUDE =
LIBS = 


player:
	${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f player core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<

