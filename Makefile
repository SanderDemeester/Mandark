CC=gcc
CFLAGS=-Wall -pthread
BIN=mandrak
all:	mandrak
mandrak: tcp_wrap.o mandrak.c
	$(CC) $(CFLAGS) tcp_wrap.o  mandrak.c -o $(BIN)
tcp_wrap.o:
	$(CC) $(CLAGS) -c tcp_wrap.c -o tcp_wrap.o
clean:	
	rm $(BIN)
	rm *.o
