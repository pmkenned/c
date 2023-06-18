CC ?= gcc
CPPFLAGS += -MMD
CFLAGS += -Wall -Wextra -std=c99
LDFLAGS +=
LDLIBS +=

TARGET=fat12
BUILD_DIR=./build
SRC=$(wildcard *.c)
OBJ=$(SRC:%.c=$(BUILD_DIR)/%.o)
DEP=$(OBJ:%.o=%.d)

CFLAGS += -ggdb
#CFLAGS += -O2 -DNDEBUG

INPUT ?= ./data/borland_turbo_c_v2_disk1.img

.PHONY: all run clean

all: $(BUILD_DIR)/$(TARGET)

run: $(BUILD_DIR)/$(TARGET)
	$(BUILD_DIR)/$(TARGET) $(INPUT)

clean:
	rm -rf $(BUILD_DIR)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -o $@ $<

-include $(DEP)
