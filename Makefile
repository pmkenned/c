# TODO: make either debug or release default, put them in separate directories? use strip?
# TODO: make install
# TODO: static library build target
# TODO: flag for which libraries to link (e.g. -lm)
# TODO: -L, -I?

CC=gcc
CFLAGS=-Wall -Werror
TARGET=myprog
BUILD_DIR=./build

SRC = main.c \
      parse_options.c \
	  list.c \
	  dyn_arr.c \
	  str.c \
	  map.c \
	  wrapper.c
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)
DEP = $(OBJ:%.o=%.d)

TEST_SRC = test.c \
		   list.c \
		   test_list.c \
		   dyn_arr.c \
		   test_dyn_arr.c \
		   str.c \
		   test_str.c \
		   map.c \
		   test_map.c \
		   map2.c \
		   test_map2.c \
		   wrapper.c
TEST_OBJ = $(TEST_SRC:%.c=$(BUILD_DIR)/%.o)
TEST_DEP = $(TEST_OBJ:%.o=%.d)

all: release

debug: CFLAGS += -DDEBUG -g
debug: $(TARGET)

release: CFLAGS += -DNDEBUG -O2
release: $(TARGET)

$(TARGET): $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(OBJ)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

-include $(DEP)

.PHONY: test

test: CFLAGS += -DDEBUG -g
test: $(BUILD_DIR)/test

$(BUILD_DIR)/test: $(TEST_OBJ)
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@
	-$@

-include $(TEST_DEP)

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -MMD -c $< -o $@

.PHONY: perf

perf: release
	./do_n.sh $(BUILD_DIR)/$(TARGET) 10

static: $(BUILD_DIR)/list.a

# TODO: test this target
$(BUILD_DIR)/%.a: %.o
	mkdir -p $(@D)
	ar rcs $@ $<

#install: all
#	cp -f build/list.a /usr/local/lib/
#	cp -f src/list.h /usr/local/include/

.PHONY: clean

# TODO: find more elegant way to remove *.a files
clean:
	rm -f $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/test $(OBJ) $(DEP) $(TEST_OBJ) $(TEST_DEP) $(BUILD_DIR)/*.a
