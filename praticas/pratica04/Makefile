CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

OBJS = main.o conectividade.o planaridade.o
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

main.o: main.c conectividade.h planaridade.h
	$(CC) $(CFLAGS) -c main.c

conectividade.o: conectividade.c conectividade.h
	$(CC) $(CFLAGS) -c conectividade.c

planaridade.o: planaridade.c planaridade.h
	$(CC) $(CFLAGS) -c planaridade.c

clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe