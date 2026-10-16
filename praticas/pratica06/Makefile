CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -g

TARGET = main
OBJS = main.o dijkstra.o

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c dijkstra.h
	$(CC) $(CFLAGS) -c main.c

dijkstra.o: dijkstra.c dijkstra.h
	$(CC) $(CFLAGS) -c dijkstra.c

clean:
	rm -f $(TARGET) $(TARGET).exe $(OBJS)

.PHONY: all clean