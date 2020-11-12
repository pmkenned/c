#ifndef RAND_H
#define RAND_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#define MY_RAND_MAX = 2147483647

void my_srand(unsigned long seed);
long my_rand();

#ifdef __cplusplus
}
#endif

#endif /* RAND_H */
