CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

FILES=main.c extclib.o

.PHONY: defualt build run
defualt: build run

build: $(FILES)
	$(CC) $(CFLAGS) $(FILES) -o main -lws2_32
run:
	./main

