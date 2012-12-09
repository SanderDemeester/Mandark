B1;3201;0csrc=$(wildcard src/*.c)
obj=$(addprefix obj/,$(notdir $(src:.c=.o)))
CC=gcc
CFLAGS=-I. -Wall -pthread -std=c99 -lm
BIN=mandark

all:	mandark
debug: CC += -g
debug: mandark

ckill: $(obj)
	$(CC) $(CFLAGS) $(obj) -o $@
obj/%.o: src/*.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	if [ -f "Mandark" ]; then \
	rm Mandark; \
	fi; done
	@for file in $(obj); do \
	if [ -f $$file ]; then \
	rm "$$file"; \
	fi; done
