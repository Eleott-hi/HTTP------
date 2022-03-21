CC=gcc
LN=ld
CFLAGS=-Wall -std=c99

HEADERS=hashtab.h tree.h
SOURCES=hashtab.c tree.c
OBJECTS=hashtab.o tree.o

.PHONY: defualt
defualt: build link

build: $(HEADERS) $(SOURCES)
	$(CC) $(CFLAGS) -c $(SOURCES)
link: $(OBJECTS)
	$(LN) -r $(OBJECTS) -o extclib.o
clean:
	rm -f *.o *.i *.s
