SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CC = gcc
CFLAGS = -Wall -std=c99 -c

TARGET = server

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $^ -o $(PROJECT_ROOT_PATH)/$@.out  # gcc $(OBJ) -o pwd/server.out

*.o: *.c
	$(CC) $(CFLAGS) $^ -o $@


.PHONY: clean
clean:
	rm -rf *.o $(PROJECT_ROOT_PATH)/$(TARGET).out

