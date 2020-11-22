ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif
CC=gcc
CFLAGS=-Wall -Werror -Wextra -Wpedantic -std=c89
CPPFLAGS=-I$(DESTDIR)$(PREFIX)/include
LDFLAGS=-L$(DESTDIR)$(PREFIX)/lib/
LDLIBS=-lmylib
TARGET=foo

SRC := $(wildcard *.c)
OBJ = $(SRC:%.c=%.o)
DEP = $(OBJ:%.o=%.d)

.PHONY: all clean

all: $(TARGET)

# possibly unnecessary as this is the implicit rule (?)
%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

-include $(DEP)

$(TARGET): $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

clean:
	rm -f $(TARGET) *.d *.o
