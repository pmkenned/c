#ifndef WRAPPER_H
#define WRAPPER_H

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

/* TODO: find a way to use __FILE__ and __LINE__ */

/* stdio */
FILE * Fopen(const char *pathname, const char *mode);

/* malloc */
void *Malloc(size_t size);
void Free(void *ptr);
void *Calloc(size_t nmemb, size_t size);
void *Realloc(void *ptr, size_t size);
/* void *Reallocarray(void *ptr, size_t nmemb, size_t size); */

#endif /* WRAPPER_H */
