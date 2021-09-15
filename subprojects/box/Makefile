CC = gcc
CPPFLAGS =
CFLAGS = -Wall -Wextra -pedantic -std=c99
LDFLAGS = 
LDLIBS = 

TARGET = a.out
BUILD_DIR = build

SRC = $(wildcard *.c)
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)

.PHONY: all run test clean

all: $(BUILD_DIR)/$(TARGET)

run: all
	$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

.PHONY: F5 F6

F5: all
F6: run

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -MMD -o $@ $<

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

-include $(DEP)
