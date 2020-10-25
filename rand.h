#ifndef RAND_H
#define RAND_H

#include <stdint.h>

#define MY_RAND_MAX = 2147483647

void my_srand(unsigned long seed);
long my_rand();

#endif /* RAND_H */
