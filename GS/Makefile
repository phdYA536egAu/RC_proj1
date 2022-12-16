SHELL = /bin/sh

OBJS =   server.c
CFLAG = -Wall -g
CC = gcc
INCLUDE =
LIBS = -lm



server:
	${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f *.o core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<

	