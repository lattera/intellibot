TARGET=intellibot
CFLAGS=-g -I../include -I../queue/include
LDFLAGS=
CC=clang

SRCS=main.c bot.c ../queue/src/queue.c
OBJS=main.o bot.o queue.o

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${LDFLAGS} ${OBJS}

queue.o: ../queue/src/queue.c

clean:
	rm -f *.o ${TARGET}
