TARGET=intellibot
CFLAGS=-g -I../include -I../queue/include -I../libsock/include
LDFLAGS=
CC=clang

SRCS=main.c bot.c ../libsock/src/libsock.c ../libsock/src/util.c ../queue/src/queue.c
OBJS=main.o bot.o queue.o util.o libsock.o

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${LDFLAGS} ${OBJS}

queue.o: ../queue/src/queue.c
util.o libsock.o: ../libsock/src/util.c ../libsock/src/libsock.c

clean:
	rm -f *.o ${TARGET}
