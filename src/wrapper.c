#include "wrapper.h"

/* stdio */

FILE * Fopen(const char *pathname, const char *mode)
{
    FILE * fp = fopen(pathname, mode);

    if (fp == NULL) {
        perror(pathname);
        exit(EXIT_FAILURE);
    }

    return fp;
}

/* malloc */

void *Malloc(size_t size)
{
    void * temp = malloc(size);
    if (temp == NULL && size > 0) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    return temp;
}

void Free(void *ptr)
{
    free(ptr);
}

void *Calloc(size_t nmemb, size_t size)
{
    void * temp = calloc(nmemb, size);
    if (temp == NULL && nmemb > 0 && size > 0) {
        perror("calloc");
        exit(EXIT_FAILURE);
    }
    return temp;
}

void *Realloc(void *ptr, size_t size)
{
    void * temp = realloc(ptr, size);
    if (temp == NULL) {
        free(ptr);
        perror("realloc");
        exit(EXIT_FAILURE);
    }
    return temp;
}
