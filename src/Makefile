TARGET=intellibot
CFLAGS=-g -I../include -I../queue/include -I../libsock/include -I/usr/local/include
LDFLAGS=-lsqlite3 -L/usr/local/lib
CC=clang

SRCS=main.c bot.c sql.c irc.c ../libsock/src/libsock.c ../libsock/src/util.c ../queue/src/queue.c
OBJS=main.o bot.o sql.o irc.o queue.o util.o libsock.o

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${LDFLAGS} ${OBJS}

queue.o: ../queue/src/queue.c
util.o libsock.o: ../libsock/src/util.c ../libsock/src/libsock.c

clean:
	rm -f *.o ${TARGET}
