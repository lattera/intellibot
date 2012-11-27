CC=clang
CFLAGS=-g -I../include -I../queue/include -I../libsock/include -I/usr/local/include -fPIC
LDFLAGS=-shared

log.so: log.o
	${CC} -o $@ ${LDFLAGS} log.o

log.o: log.c
