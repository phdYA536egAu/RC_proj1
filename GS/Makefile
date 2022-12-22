SHELL = /bin/sh

#UDP
OBJS = server.c
CFLAGS = -Wall -g
CC = gcc
INCLUDE =
LIBS = 


server:
	${OBJ}
	${CC} ${CFLAG} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f server

a:
	make clean
	make server
	


#TCP TEST
t:
	gcc -o t tcpTestServer.c
	./t   

