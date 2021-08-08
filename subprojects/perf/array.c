#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdint.h>
#include <time.h>

enum {
    ROWS = 10,
    COLS = 20
};

static void print_elapsed(struct timespec before, struct timespec after)
{
    uint64_t before_ns = (before.tv_sec * 1000000000) + before.tv_nsec;
    uint64_t after_ns = (after.tv_sec * 1000000000) + after.tv_nsec;
    uint64_t elapsed_ns = after_ns - before_ns;
    printf("%lu ns\n", elapsed_ns);
}

int main()
{
    int i, j;
    struct timespec before, after;
    int array[ROWS][COLS];
    uint64_t before_ns, after_ns, elapsed_ns;

    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++)
            array[i][j] = 0;

    // row major
    clock_gettime(CLOCK_MONOTONIC, &before);
    for (i = 0; i < ROWS; i++)
        for (j = 0; j < COLS; j++)
            array[i][j] += j;
    clock_gettime(CLOCK_MONOTONIC, &after);
    print_elapsed(before, after);

    // column major
    clock_gettime(CLOCK_MONOTONIC, &before);
    for (i = 0; i < COLS; i++)
        for (j = 0; j < ROWS; j++)
            array[j][i] += j;
    clock_gettime(CLOCK_MONOTONIC, &after);
    print_elapsed(before, after);

#if 0
    for (i = 0; i < ROWS; i++) {
        for (j = 0; j < COLS; j++) {
            printf("%d\t", array[i][j]);
        }
        printf("\n");
    }
#endif
    return 0;
}
