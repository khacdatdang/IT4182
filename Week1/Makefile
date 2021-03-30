CFLAGS = -c -Wall
CC = gcc
LIBS = -lm
all: main
main : indexer.o
	$(CC) indexer.o -o main
indexer.o: indexer.c
	$(CC) $(CFLAGS) indexer.c
clean:
	rm  -f *.o *-