#include "common.h"
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void
die(const char * fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
	va_end(ap);
    exit(EXIT_FAILURE);
}

void
pack_le(void * p, size_t n, uint64_t x)
{
    assert(n <= 8);
    for (size_t i = 0; i < n; i++) {
        ((char *)p)[i] = x & 0xff;
        x >>= 8;
    }
}

uint64_t
unpack_le(void * p, size_t n)
{
    assert(n <= 8);
    uint64_t x = 0;
    for (size_t i = 0; i < n; i++) {
        x |= ((uint8_t *)p)[i] << 8*i;
    }
    return x;
}

Buffer
read_file(const char * filename)
{
    FILE * fp = fopen(filename, "r");
    if (fp == NULL) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    struct stat sb;
    if (fstat(fileno(fp), &sb) == -1) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    Buffer buffer;
    buffer.cap = (size_t) (sb.st_size);
    buffer.len = buffer.cap;
    buffer.p = malloc(sizeof(*buffer.p)*buffer.cap);
    fread(buffer.p, 1, buffer.len, fp);
    if (ferror(fp)) {
        perror(filename);
        exit(EXIT_FAILURE);
    }
    fclose(fp);
    return buffer;
}
