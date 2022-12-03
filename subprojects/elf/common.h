#ifndef COMMON_H
#define COMMON_H

#include <stddef.h>
#include <stdint.h>

#define NELEM(X) sizeof(X)/sizeof(X[0])

typedef struct {
    char * p;
    size_t len;
    size_t cap;
} Buffer;

void die(const char * fmt, ...);
void pack_le(void * p, size_t n, uint64_t x);
uint64_t unpack_le(void * p, size_t n);
Buffer read_file(const char * filename);

#endif /* COMMON_H */
