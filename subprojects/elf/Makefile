CC ?= gcc
CPPFLAGS += -D_XOPEN_SOURCE=600
#CPPFLAGS += -D_POSIX_C_SOURCE=200112L
CFLAGS += -Wall -Wextra -std=c99
LDFLAGS +=
LDLIBS +=

TARGET=elfread
BUILD_DIR=./build
SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

CFLAGS += -ggdb
#CFLAGS += -O2 -DNDEBUG

.PHONY: all run clean

all: $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET) build/elfread

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c -o $@ $<

-include $(DEP)
