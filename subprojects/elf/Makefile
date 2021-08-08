CC ?= gcc
CPPFLAGS +=
CFLAGS += -Wall -Wextra -std=c99
LDFLAGS +=
LDLIBS +=

TARGET=elfread
BUILD_DIR=./build
SRC=main.c\
	elf.c
OBJ=$(SRC:%.c=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

CFLAGS += -ggdb
#CFLAGS += -O2 -DNDEBUG

.PHONY: all run clean

all: $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c -o $@ $<

-include $(DEP)
