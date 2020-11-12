#include <stdio.h>
#include <string.h>
#include <time.h>
#if 0
#include "../src/dyn_arr.h"
#include "../src/str.h"
#include "../src/map.h"
#endif
#include "minunit.h"

char * list_tests();
char * dyn_arr_tests();
char * str_tests();
char * map_tests();
char * map2_tests();

int tests_run = 0;
char err_msg[ERR_MSG_LEN];

char * all_tests() {

    mu_run_test_suite(list_tests);
    mu_run_test_suite(dyn_arr_tests);
    mu_run_test_suite(str_tests);
    mu_run_test_suite(map_tests);
    mu_run_test_suite(map2_tests);

    return 0;
}

int main() {

    char * test_results = all_tests();

    if (test_results != 0)
        printf("Test failed: %s\n", test_results);
    else
        printf("All tests passed! (%i total)\n", tests_run);

    return 0;
}
