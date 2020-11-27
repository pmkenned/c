# TODO: shared object: CFLAGS += -fPIC -shared (windows: *.dll ; linux: *.so)
# TODO: cygwin?
# TODO: consider writing a configure script that outputs a makefile

# NOTE: for recursively building a sub-project:
#.PHONY: libfoo
#libfoo:
#	$(MAKE) -C libs/foo

# NOTES:
# use SRC := $(wildcard *.c) to specify all *.c files

CC=gcc
#CPPFLAGS=-I/usr/include
CFLAGS=-Wall -Werror -Wextra -pedantic -std=c89 -Wno-variadic-macros 
#CFLAGS+=-Wno-format
#LDFLAGS=-nodefaultlibs -L/usr/lib
LDLIBS=-lpthread
CXXFLAGS=-Weffc++
ARFLAGS=rcs
TARGET=myprog
SRC_DIR=./src
TEST_DIR=./test
BUILD_DIR=./build
DEBUG_DIR=$(BUILD_DIR)/debug
RELEASE_DIR=$(BUILD_DIR)/release
TEST_RELEASE_DIR=$(BUILD_DIR)/test_debug
TEST_DEBUG_DIR=$(BUILD_DIR)/test_release

CPP_TARGET=myprog_cpp
CPP_RELEASE_DIR=$(BUILD_DIR)/release_cpp

# DETECT OPERATING SYSTEM
ifeq ($(OS),Windows_NT)
	DETECTED_OS := Windows
else
	DETECTED_OS := $(shell uname)
endif

# SET DEFINES BASED ON OPERATING SYSTEM
ifeq ($(DETECTED_OS),Windows)
	CPPFLAGS += -DWIN32
	LIB_TARGET=libmylib.lib
endif
ifeq ($(DETECTED_OS),Linux)
	CPPFLAGS += -DLINUX
	LIB_TARGET=libmylib.a
endif

# INSTALLATION DESTINATION
ifeq ($(PREFIX),)
	PREFIX := /usr/local
endif

SRC_FILES = main.c \
	        parse_options.c \
	        list.c \
	        dyn_arr.c \
	        str.c \
	        map.c \
	        wrapper.c

SRC = $(SRC_FILES:%.c=$(SRC_DIR)/%.c)

OBJ_D = $(SRC:$(SRC_DIR)/%.c=$(DEBUG_DIR)/%.o)
DEP_D = $(OBJ_D:%.o=%.d)

OBJ_R = $(SRC:$(SRC_DIR)/%.c=$(RELEASE_DIR)/%.o)
DEP_R = $(OBJ_R:%.o=%.d)

CPP_SRC = main.cpp str.c dyn_arr.c
CPP_OBJ_R_TMP = $(CPP_SRC:%.cpp=$(CPP_RELEASE_DIR)/%.o)
CPP_OBJ_R = $(CPP_OBJ_R_TMP:%.c=$(RELEASE_DIR)/%.o)
# FILES_OUT = $(patsubst %.xls,%.xlsx,$(patsubst %.doc,%.docx,$(FILES_IN)))
CPP_DEP_R = $(CPP_OBJ_R:%.o=%.d)

TEST_SRC_FILES = test.c \
		   test_list.c \
		   test_dyn_arr.c \
		   test_str.c \
		   test_map.c \
		   test_map2.c

TEST_SRC = $(TEST_SRC_FILES:%.c=$(TEST_DIR)/%.c)

TEST_OBJ_D = $(TEST_SRC:$(TEST_DIR)/%.c=$(TEST_DEBUG)/%.o)
TEST_DEP_D = $(TEST_OBJ_D:%.o=%.d)

TEST_OBJ_R = $(TEST_SRC:$(TEST_DIR)/%.c=$(TEST_RELEASE_DIR)/%.o)
TEST_DEP_R = $(TEST_OBJ_R:%.o=%.d)

LIB_SRC = list.c \
		  map.c \
		  dyn_arr.c \
		  str.c \
		  map2.c \
		  wrapper.c

LIB_INC = $(LIB_SRC:%.c=%.h)

LIB_OBJ_D = $(LIB_SRC:%.c=$(DEBUG_DIR)/%.o)
LIB_DEP_D = $(LIB_OBJ_D:%.o=%.d)

LIB_OBJ_R = $(LIB_SRC:%.c=$(RELEASE_DIR)/%.o)
LIB_DEP_R = $(LIB_OBJ_R:%.o=%.d)

.PHONY: all debug release

all: release

debug: CFLAGS += -DDEBUG -g
debug: $(DEBUG_DIR)/$(TARGET)

release: CFLAGS += -DNDEBUG -O2
release: $(RELEASE_DIR)/$(TARGET)

$(DEBUG_DIR)/$(TARGET): $(OBJ_D)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

$(RELEASE_DIR)/$(TARGET): $(OBJ_R)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)

-include $(DEP_D)
-include $(DEP_R)

.PHONY: cpp

cpp: $(CPP_RELEASE_DIR)/$(CPP_TARGET)

$(CPP_RELEASE_DIR)/$(CPP_TARGET): $(CPP_OBJ_R)
	mkdir -p $(@D)
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)

.PHONY: test test_debug test_release

test: test_release

test_debug: CFLAGS += -DDEBUG -g
test_debug: $(DEBUG_DIR)/test

test_release: CFLAGS += -DNDEBUG -O2
test_release: $(RELEASE_DIR)/test

$(DEBUG_DIR)/test: $(TEST_OBJ_D) $(LIB_OBJ_D)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	-$@

$(RELEASE_DIR)/test: $(TEST_OBJ_R) $(LIB_OBJ_R)
	mkdir -p $(@D)
	$(CC) $(LDFLAGS) $^ -o $@ $(LDLIBS)
	-$@

-include $(LIB_DEP_D)
-include $(LIB_DEP_R)

-include $(TEST_DEP_D)
-include $(TEST_DEP_R)

# OBJECT FILE RULES

$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

$(TEST_DEBUG_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

$(TEST_RELEASE_DIR)/%.o: $(TEST_DIR)/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -c $< -o $@

$(CPP_RELEASE_DIR)/%.o: $(SRC_DIR)/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -MMD -c $< -o $@

-include $(CPP_DEP_R)

.PHONY: perf

perf: release
	./do_n.sh "$(RELEASE_DIR)/$(TARGET) --seed 0 -n 1000000" 10

.PHONY: install uninstall

install: release
	mkdir -p $(DESTDIR)$(PREFIX)/bin
	cp -f $(RELEASE_DIR)/$(TARGET) $(DESTDIR)$(PREFIX)/bin/
	mkdir -p $(DESTDIR)$(PREFIX)/man/man1
	#install -g 0 -o 0 -m 0644 $(TARGET).1 $(DESTDIR)$(PREFIX)/man/man1/
	cp doc/$(TARGET).1 $(DESTDIR)$(PREFIX)/man/man1/
	gzip $(DESTDIR)$(PREFIX)/man/man1/$(TARGET).1

uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/$(TARGET)
	rm -f $(DESTDIR)$(PREFIX)/man/man1/$(TARGET).1.gz

.PHONY: library install_lib uninstall_lib

library: $(RELEASE_DIR)/$(LIB_TARGET)

$(RELEASE_DIR)/$(LIB_TARGET): $(LIB_OBJ_R)
	mkdir -p $(@D)
	$(AR) $(ARFLAGS) $@ $^

install_lib: library
	mkdir -p $(DESTDIR)$(PREFIX)/lib
	mkdir -p $(DESTDIR)$(PREFIX)/include
	cp -f $(RELEASE_DIR)/$(LIB_TARGET) $(DESTDIR)$(PREFIX)/lib/
	cp -f $(LIB_INC) $(DESTDIR)$(PREFIX)/include/
	# TODO: man page in section 3

uninstall_lib:
	rm -f $(DESTDIR)$(PREFIX)/lib/$(LIB_TARGET)
	rm -f $(DESTDIR)$(PREFIX)/include/{$(LIB_INC)}

.PHONY: clean

clean:
	rm -rf $(BUILD_DIR)
