CC = gcc
CFLAGS = -Wall -Wextra -std=c99

main: main.c bellman_ford.c bellman_ford.h
	$(CC) $(CFLAGS) -o main main.c bellman_ford.c

clean:
	rm -f main main.exe