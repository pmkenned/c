.PHONY: all run clean
CC=gcc
CPPFLAGS=
CFLAGS=
LDFLAGS=

TARGET=a.out
BUILD_DIR=./build

SRC=main.c \
	assert.c \
	except.c
OBJ=$(SRC:%.c=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

all: $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	$(CC) -c -MMD $(CPPFLAGS) $(CFLAGS) -o $@ $<

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

-include $(DEP)

clean:
	rm -f $(BUILD_DIR)
