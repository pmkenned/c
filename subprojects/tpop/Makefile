CC=gcc
CFLAGS=-Wall -Wextra -Werror -O2
TARGET=markov
BUILD_DIR=./build

SRC = markov.c
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

.PHONY: all run perf clean

all: $(BUILD_DIR)/$(TARGET)

run: all
	$(BUILD_DIR)/$(TARGET) < Genesis.txt

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@

perf: all
	./do_n.sh 10

clean:
	rm -rf $(BUILD_DIR)
