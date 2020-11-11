#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dyn_arr.h"
#include "str.h"
#include "map.h"
#include "wrapper.h"
#include "minunit.h"

#if 0
char * test_map_count();
char * test_map_count_words();
#endif

#define BUFFER_LEN 1024
static char buffer[BUFFER_LEN];

char * test_map_count()
{
    map_t * map = map_create(int);
    int * x_p;

    /* TODO:
       return value should be if it was found, pass ptr for value destination
       or return a ptr which can be NULL if not found
    */

    map_set_rvalue(map, "apple", 1);
    map_set_rvalue(map, "banana", 2);
    map_set_rvalue(map, "cantaloupe", 3);
    map_set_rvalue(map, "date", 4);
    map_set_rvalue(map, "fig", 5);
    map_set_rvalue(map, "grape", 6);
    map_set_rvalue(map, "honey", 7);
    map_set_rvalue(map, "lemon", 8);
    map_set_rvalue(map, "orange", 9);
    map_set_rvalue(map, "pear", 10);
    map_set_rvalue(map, "peach", 11);

#if 0
    int x;
    if (map_get(map, "apple", &x)) { printf("apple: %d\n", x); }
    if (map_get(map, "banana", &x)) { printf("banana: %d\n", x); }
    if (map_get(map, "cantaloupe", &x)) { printf("cantaloupe: %d\n", x); }
    if (map_get(map, "date", &x)) { printf("date: %d\n", x); }
    if (map_get(map, "fig", &x)) { printf("fig: %d\n", x); }
    if (map_get(map, "grape", &x)) { printf("grape: %d\n", x); }
    if (map_get(map, "honey", &x)) { printf("honey: %d\n", x); }
    if (map_get(map, "lemon", &x)) { printf("lemon: %d\n", x); }
    if (map_get(map, "orange", &x)) { printf("orange: %d\n", x); }
    if (map_get(map, "pear", &x)) { printf("pear: %d\n", x); }
    if (map_get(map, "peach", &x)) { printf("peach: %d\n", x); }
    if (map_get(map, "kumquat", &x)) { printf("kumquat: %d\n", x); }
#endif

    x_p = map_get_ptr(map, "peach");
    mu_assert(x_p != NULL, "%s", "peach should be in map but isn't");
    x_p = map_get_ptr(map, "kumquat");
    mu_assert(x_p == NULL, "%s", "kumquat  should not be in map but is");

#if 0
    printf("apple: %d\n", map_get(map, "apple", int));
    printf("banana: %d\n", map_get(map, "banana", int));
    printf("cantaloupe: %d\n", map_get(map, "cantaloupe", int));
    printf("date: %d\n", map_get(map, "date", int));
    printf("fig: %d\n", map_get(map, "fig", int));
    printf("grape: %d\n", map_get(map, "grape", int));
    printf("honey: %d\n", map_get(map, "honey", int));
    printf("lemon: %d\n", map_get(map, "lemon", int));
    printf("orange: %d\n", map_get(map, "orange", int));
    printf("pear: %d\n", map_get(map, "pear", int));
    printf("peach: %d\n", map_get(map, "peach", int));

    map_print(map);
#endif

    map_destroy(map);

    return 0;
}

#if 0
char * test_map_struct()
{
    map_t * map = map_create(my_t);

    my_t a = {.x = 1, .y = 2};
    my_t b = {.x = 3, .y = 4};

    map_set(map, "hi", a);
    map_set(map, "you", b);
    map_set(map, "what's", b);
    map_set(map, "up", b);
    my_t c = map_get(map, "hi", my_t);
    my_t d = map_get(map, "you", my_t);
    printf("c: %d %d\n", c.x, c.y);
    printf("d: %d %d\n", d.x, d.y);

    map_print(map);

    map_destroy(map);

    return 0;
}
#endif

/* TODO: use mu_assert */
char * test_map_count_words()
{
    size_t i;
    char ** tokens;
    map_t * map = map_create(int);
    char * file_contents = str_create();
    const char fn[] = "data/hello.txt";
    FILE * fp = Fopen(fn, "r");
    while (fgets(buffer, BUFFER_LEN, fp) != NULL)
        str_concat(file_contents, buffer);
    fclose(fp);
    tokens = str_tokenize(file_contents, " \n?.!");

    for (i=0; i<dyn_arr_length(tokens); i++)
        str_tolower(tokens[i]);

    for (i=0; i<dyn_arr_length(tokens); i++) {
        int * count = map_get_ptr(map, tokens[i]);
        if (count == NULL)
            map_set_rvalue(map, tokens[i], 1);
        else
            (*count)++;
    }

    /*
      printf("\n");
      map_print(map);
      printf("\n");
    */

    for (i=0; i<dyn_arr_length(tokens); i++)
        str_destroy(tokens[i]);
    dyn_arr_destroy(tokens);
    str_destroy(file_contents);
    map_destroy(map);

    return 0;
}

char * test_map_map()
{
    map_t * map_l1 = map_create(map_t *);

    map_t * map_ny = map_create(int);
    map_t * map_me = map_create(int);

    map_set_rvalue(map_ny, "Albany", 97000);
    map_set_rvalue(map_ny, "New York City", 8300000);

    map_set_rvalue(map_me, "Bangor", 33000);
    map_set_rvalue(map_me, "Augusta", 19000);

    map_set(map_l1, "New York", map_ny);
    map_set(map_l1, "Maine",    map_me);

    /* map_print_l2(map_l1); */

    map_destroy(map_l1);

    return 0;
}

char * map_tests()
{
    mu_run_test(test_map_count);
    mu_run_test(test_map_count_words);
    mu_run_test(test_map_map);
    return 0;
}

