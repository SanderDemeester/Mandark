src=$(wildcard src/*.c)
obj=$(addprefix obj/,$(notdir $(src:.c=.o)))
CC=gcc
CFLAGS=-I. -Wall -pthread -lm
BIN=mandark

all:	mandark
debug: CC += -g
debug: mandark

mandark: $(obj)
	$(CC) $(CFLAGS) $(obj) -o $@
obj/%.o: src/%.c
	$(CC) $(CFLAGS) -c -o $@ $<
clean:
	if [ -f "Mandark" ]; then \
	rm Mandark; \
	fi; done
	@for file in $(obj); do \
	if [ -f $$file ]; then \
	rm "$$file"; \
	fi; done
