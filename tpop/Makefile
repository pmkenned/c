CC=gcc
CFLAGS=-Wall -Wextra -Werror -O2
TARGET=markov

SRC = markov.c
OBJ = $(SRC:%.c=%.o)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $^ -o $@

all: $(TARGET)

.PHONY: perf

perf: all
	./do_n.sh 10

.PHONY: clean

clean:
	rm -f *.o $(TARGET)
