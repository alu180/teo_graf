CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

all: main

main: main.o busca_largura.o busca_profundidade.o
	$(CC) $(CFLAGS) -o main main.o busca_largura.o busca_profundidade.o

main.o: main.c busca_largura.h busca_profundidade.h
	$(CC) $(CFLAGS) -c main.c

busca_largura.o: busca_largura.c busca_largura.h
	$(CC) $(CFLAGS) -c busca_largura.c

busca_profundidade.o: busca_profundidade.c busca_profundidade.h
	$(CC) $(CFLAGS) -c busca_profundidade.c

clean:
	rm -f *.o main