CC = gcc
CFLAGS = -Wall -Wextra -g

all: main

main: main.o grafo_matriz.o grafo_lista.o
	$(CC) $(CFLAGS) -o main main.o grafo_matriz.o grafo_lista.o

main.o: main.c grafo_matriz.h grafo_lista.h
	$(CC) $(CFLAGS) -c main.c

grafo_matriz.o: grafo_matriz.c grafo_matriz.h
	$(CC) $(CFLAGS) -c grafo_matriz.c

grafo_lista.o: grafo_lista.c grafo_lista.h
	$(CC) $(CFLAGS) -c grafo_lista.c

clean:
	rm -f *.o main