CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g

SRCS = Coloracao.c Main.c
OBJS = $(SRCS:.c=.o)
TARGET = Main

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean