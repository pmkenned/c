#include <stdio.h>
#include "map2.h"
#include "minunit.h"

char * test_map2()
{
    /*map2_t * map = map2_create(char *); */
    map2b_t * map = map2b_create(TYPE_C_STR);

    map2b_set(map, "New York", "Albany");
    map2b_set(map, "Maine", "Augusta");

    map2b_print(map);

    map2b_destroy(map);

    return 0;
}

char * map2_tests()
{
    mu_run_test(test_map2);
    return 0;
}
