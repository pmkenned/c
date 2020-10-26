#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dyn_arr.h"
#include "str.h"
#include "map.h"
#include "wrapper.h"
#include "minunit.h"

//char * test_map_count();
//char * test_map_count_words();

char * test_map_count()
{
    map_t * map = map_create(int);

    // TODO:
    // return value should be if it was found, pass ptr for value destination
    // or return a ptr which can be NULL if not found

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

    //int x;
    //if (map_get(map, "apple", &x)) { printf("apple: %d\n", x); }
    //if (map_get(map, "banana", &x)) { printf("banana: %d\n", x); }
    //if (map_get(map, "cantaloupe", &x)) { printf("cantaloupe: %d\n", x); }
    //if (map_get(map, "date", &x)) { printf("date: %d\n", x); }
    //if (map_get(map, "fig", &x)) { printf("fig: %d\n", x); }
    //if (map_get(map, "grape", &x)) { printf("grape: %d\n", x); }
    //if (map_get(map, "honey", &x)) { printf("honey: %d\n", x); }
    //if (map_get(map, "lemon", &x)) { printf("lemon: %d\n", x); }
    //if (map_get(map, "orange", &x)) { printf("orange: %d\n", x); }
    //if (map_get(map, "pear", &x)) { printf("pear: %d\n", x); }
    //if (map_get(map, "peach", &x)) { printf("peach: %d\n", x); }
    //if (map_get(map, "kumquat", &x)) { printf("kumquat: %d\n", x); }

    int * x_p;
    printf("\n");
    if ((x_p = map_get_ptr(map, "peach"))) { printf("peach: %d\n", *x_p); }
    if ((x_p = map_get_ptr(map, "kumquat"))) { printf("kumquat: %d\n", *x_p); }
    printf("\n");

    //printf("apple: %d\n", map_get(map, "apple", int));
    //printf("banana: %d\n", map_get(map, "banana", int));
    //printf("cantaloupe: %d\n", map_get(map, "cantaloupe", int));
    //printf("date: %d\n", map_get(map, "date", int));
    //printf("fig: %d\n", map_get(map, "fig", int));
    //printf("grape: %d\n", map_get(map, "grape", int));
    //printf("honey: %d\n", map_get(map, "honey", int));
    //printf("lemon: %d\n", map_get(map, "lemon", int));
    //printf("orange: %d\n", map_get(map, "orange", int));
    //printf("pear: %d\n", map_get(map, "pear", int));
    //printf("peach: %d\n", map_get(map, "peach", int));

    map_print(map);

    map_destroy(map);

    return 0;
}

//char * test_map_struct()
//{
//    map_t * map = map_create(my_t);
//
//    my_t a = {.x = 1, .y = 2};
//    my_t b = {.x = 3, .y = 4};
//
//    map_set(map, "hi", a);
//    map_set(map, "you", b);
//    map_set(map, "what's", b);
//    map_set(map, "up", b);
//    my_t c = map_get(map, "hi", my_t);
//    my_t d = map_get(map, "you", my_t);
//    printf("c: %d %d\n", c.x, c.y);
//    printf("d: %d %d\n", d.x, d.y);
//
//    map_print(map);
//
//    map_destroy(map);
//
//    return 0;
//}

char * test_map_count_words()
{
    size_t i;
    char * file_contents = str_create();
    const char fn[] = "data/hello.txt";
    FILE * fp = Fopen(fn, "r");
    while (fgets(buffer, BUFFER_LEN, fp) != NULL)
        str_concat(file_contents, buffer);
    fclose(fp);
    char ** tokens = str_tokenize(file_contents, " \n?.!");

    for (i=0; i<dyn_arr_length(tokens); i++)
        str_tolower(tokens[i]);

    map_t * map = map_create(int);

    for (i=0; i<dyn_arr_length(tokens); i++) {
        int * count = map_get_ptr(map, tokens[i]);
        if (count == NULL)
            map_set_rvalue(map, tokens[i], 1);
        else
            (*count)++;
    }

    printf("\n");
    map_print(map);
    printf("\n");

    for (i=0; i<dyn_arr_length(tokens); i++)
        str_destroy(tokens[i]);
    dyn_arr_destroy(tokens);
    str_destroy(file_contents);
    map_destroy(map);

    return 0;
}

char * map_tests()
{
    mu_run_test(test_map_count);
    mu_run_test(test_map_count_words);
    return 0;
}

