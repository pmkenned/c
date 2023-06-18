#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stddef.h>

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;
typedef int64_t  s64;

typedef struct {
    void * data;
    size_t len;
} Buffer;

void die(const char * fmt, ...);
u64 extract_bytes_le(void * data, size_t n);
void print_bytes(uint8_t * ptr, size_t num_bytes, size_t offset);
Buffer read_entire_file(const char * filename);

#endif
