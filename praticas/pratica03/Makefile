CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: main

main: main.o dag.o
	$(CC) $(CFLAGS) -o main main.o dag.o

main.o: main.c dag.h
	$(CC) $(CFLAGS) -c main.c

dag.o: dag.c dag.h
	$(CC) $(CFLAGS) -c dag.c

clean:
	rm -f *.o main main.exe