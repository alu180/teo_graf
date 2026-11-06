CC      = gcc
CFLAGS  = -Wall -Wextra -std=c11 -O2
OBJS    = main.o cobertura.o
TARGET  = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c cobertura.h
	$(CC) $(CFLAGS) -c main.c

cobertura.o: cobertura.c cobertura.h
	$(CC) $(CFLAGS) -c cobertura.c

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

.PHONY: all clean