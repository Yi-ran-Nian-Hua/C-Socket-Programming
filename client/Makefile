SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))

CFLAGS = -Wall -c

CC = gcc

TARGET = client

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $(PROJECT_ROOT_PATH)/$@.out $^ 

*.o: *.c
	$(CC) -o $@ $(CFLAGS) $^ 

.PHONY: clean
clean:
	rm -rf *.o $(PROJECT_ROOT_PATH)/$(TARGET).out
