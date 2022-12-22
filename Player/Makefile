SHELL = /bin/sh

#UDP
OBJS =   player.c
CFLAGS = -Wall -g
CC = gcc
INCLUDE =
LIBS = 


player:
	${OBJ}
	${CC} ${CFLAG} ${INCLUDES} -o $@ ${OBJS} ${LIBS}

clean:
	-rm -f player

a:
	make clean
	make player
	


#TCP TEST
t:
	gcc -o t tcpTestPlayer.c
	./t   

