CC=gcc
CFLAGS=-Wall -pthread
BIN=mandrak
all:	mandrak
mandrak: mandrak.c
	$(CC) $(CFLAGS) mandrak.c -o $(BIN)
clean:	
	rm $(BIN)
