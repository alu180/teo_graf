CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: main

main: main.o agm.o
	$(CC) $(CFLAGS) -o main main.o agm.o

main.o: main.c agm.h
	$(CC) $(CFLAGS) -c main.c

agm.o: agm.c agm.h
	$(CC) $(CFLAGS) -c agm.c

clean:
	rm -f *.o main main.exe

.PHONY: all clean