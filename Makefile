CC=gcc
CFLAGS=-Wall -pthread
BIN=mandark
all:	mandark
debug: CC += -g
debug: mandark
mandark: tcp_wrap.o mandark.c
	$(CC) $(CFLAGS) tcp_wrap.o  mandark.c -o $(BIN)
tcp_wrap.o:
	$(CC) $(CLAGS) -c tcp_wrap.c -o tcp_wrap.o
clean:	
	rm $(BIN)
	rm *.o
