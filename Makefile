CC = gcc
CFLAGS = -Wall -Wextra -std=gnu11
LIBS = -lpcap

SRC = $(shell find . -name "*.c")
OBJ = $(SRC:.c=.o)

TARGET = ps

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LIBS)

clean:
	rm -f $(OBJ) $(TARGET)