CC=gcc
CPPFLAGS=
CFLAGS=-Wall -Wextra -pedantic -std=c99
LDFLAGS=
LDLIBS=

TARGET=a.out
BUILD_DIR=./build
SRC=main.c
OBJ=$(SRC:%.c=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

.PHONY: all run test clean

all: $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

# TODO
#test:

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c -o $@ $<

-include $(DEP)
