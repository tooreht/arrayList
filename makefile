CC = gcc
CFLAGS = -Wall -g
OBJ = al.o

all: test

test: $(OBJ) test.o
		$(CC) $(CFLAGS) $^ -o $@

interactive: $(OBJ) interactive.o
		$(CC) $(CFLAGS) $^ -o $@

%.o: %.c
		$(CC) $(CFLAGS) -c $<

.PHONY: clean
clean:
		rm -f interactive *.o
