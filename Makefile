CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2

TARGET = base64
SRC = main.c encode.c decode.c

all: $(TARGET)

$(TARGET): $(SRC) base64.h
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean
