CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

FILES=main.c extclib/extclib.o

.PHONY: defualt build run
defualt: build run

build: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o main
run:
	./main
