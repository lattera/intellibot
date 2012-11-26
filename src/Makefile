TARGET=intellibot
CFLAGS=-g -I../include
LDFLAGS=
CC=clang

SRCS=main.c
OBJS=main.o

${TARGET}: ${OBJS}
	${CC} -o ${TARGET} ${LDFLAGS} ${OBJS}

clean:
	rm -f *.o ${TARGET}
