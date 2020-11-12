#include <stdio.h>
#include <string.h>
#include <time.h>
#include "../src/dyn_arr.h"
#include "../src/str.h"
#include "../src/map.h"
#include "minunit.h"

#if 0
char * test_empty_dyn_arr();
char * test_dyn_arr_append();
char * test_dyn_arr_append_rval();
char * test_dyn_arr_struct();
#endif

typedef struct {
    int x;
    int y;
} my_t;

char * test_empty_dyn_arr() {
    int * arr = dyn_arr_create(int);
    size_t len = dyn_arr_length(arr);
    mu_assert(len == 0, "empty dynamic array should have length of 0, not %lu", (unsigned long)  len);
    dyn_arr_destroy(arr);
    return 0;
}

char * test_dyn_arr_append() {
    size_t N = 20;
    size_t i, len;
    size_t * arr = dyn_arr_create(size_t);

    for (i=0; i<N; i++) {
        dyn_arr_append(arr, i);
    }

    len = dyn_arr_length(arr);
    mu_assert(len == N, "empty dynamic array should have length of %lu, not %lu", (unsigned long) N, (unsigned long) len);

    for (i=0; i<N; i++) {
        size_t x_read = arr[i];
        mu_assert(x_read == i, "value should be %lu, not %lu", (unsigned long) i, (unsigned long) x_read);
    }

    dyn_arr_destroy(arr);
    return 0;
}

char * test_dyn_arr_append_rval() {
    int i;
    int * arr = dyn_arr_create(int);
    for (i=0; i<20; i++) {
        dyn_arr_append_rval(arr, 20-i);
    }
    for (i=0; i<20; i++) {
        int x_read = arr[i];
        mu_assert(x_read == 20-i, "value should be %d, not %d", 20-i, x_read);
    }
    dyn_arr_destroy(arr);
    return 0;
}

char * test_dyn_arr_struct() {
    my_t * arr = dyn_arr_create(my_t);

    /* my_t a = {.x = 5, .y = 2}; */
    my_t a = {5, 2};
    my_t b;

    dyn_arr_append(arr, a);

    b = arr[0];
    mu_assert(b.x == 5, "b.x != %d", 5);
    mu_assert(b.y == 2, "b.y != %d", 2);

    b.x = 1;
    b.y = 3;
    arr[0] = b;

    b = arr[0];
    mu_assert(b.x == 1, "b.x != %d", 1);
    mu_assert(b.y == 3, "b.y != %d", 3);

    dyn_arr_destroy(arr);

    return 0;
}

char * dyn_arr_tests()
{
    mu_run_test(test_empty_dyn_arr);
    mu_run_test(test_dyn_arr_append);
    mu_run_test(test_dyn_arr_append_rval);
    mu_run_test(test_dyn_arr_struct);
    return 0;
}
