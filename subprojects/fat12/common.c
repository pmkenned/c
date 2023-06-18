#define _POSIX_C_SOURCE 1
#include "common.h"
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
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

u64
extract_bytes_le(void * data, size_t n)
{
    u64 result = 0;
    u8 * u8p = (u8 *) data;
    for (size_t i = 0; i < n; i++) {
        result += *u8p << 8*i;
        u8p++;
    }
    return result;
}

void
print_bytes(uint8_t * ptr, size_t num_bytes, size_t offset)
{
    for (size_t i=0; i<num_bytes/16; i++) {
        printf("%08lx: ", i*16+offset);
        for (size_t j=0; j<2; j++) {
            for (size_t k=0; k<8; k++) {
                size_t l = i*16+j*8+k;
                printf("%02x ", *(ptr+offset+l));
            }
            printf(" ");
        }
        printf("\n");
    }
}

Buffer
read_entire_file(const char * filename)
{
    Buffer buffer;
    FILE * fp = fopen(filename, "rb");
    if (fp == NULL)
        die(strerror(errno));
    struct stat sb;
    if (fstat(fileno(fp), &sb) == -1)
        die(strerror(errno));
    buffer.len = sb.st_size;
    buffer.data = malloc(buffer.len);
    int nread = fread(buffer.data, 1, buffer.len, fp);
    if (ferror(fp))
        die(strerror(errno));
    assert(nread == (int) buffer.len);
    fclose(fp);
    return buffer;
}
