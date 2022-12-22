SHELL = /bin/sh

#UDP
OBJS =   server.c
CFLAG = -Wall -g
CC = gcc
INCLUDE =
LIBS = 



server:
	${OBJ}
	${CC} ${CFLAGS} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f server core *.core

.cpp.o:
	${CC} ${CFLAGS} ${INCLUDES} -c $<

#TCP

t:
	gcc -o t tcpTestServer.c
	./t