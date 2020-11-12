#include "rand.h"

static unsigned long rand_state = 1;

void my_srand(unsigned long seed)
{
    rand_state = seed;
}

long my_rand()
{
    rand_state = (rand_state * 1103515245 + 12345) % 2147483648;
    return rand_state;
}
