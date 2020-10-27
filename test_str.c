#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dyn_arr.h"
#include "str.h"
#include "map.h"
#include "wrapper.h"
#include "minunit.h"

//char * test_str();
//char * test_dyn_arr_str();
//char * test_str_tokenize();
//char * test_str_tokenize_file();

#define BUFFER_LEN 1024
static char buffer[BUFFER_LEN];

char * test_str()
{
    const char target_str[] = "hello, world!";
    char * s = str_create_from("hello, ");
    str_concat(s, "world!");
    mu_assert(strcmp(s, target_str) == 0, "string should be %s", target_str);
    size_t len = str_length(s);
    mu_assert(len == sizeof(target_str)-1, "length of s should be %zu, not %zu", sizeof(target_str)-1, len);
    str_destroy(s);
    return 0;
}

char * test_dyn_arr_str()
{
    char * s1 = str_create_from("hello");
    char * s2 = str_create_from("good morning");

    char ** arr = dyn_arr_create(char *);
    dyn_arr_append(arr, s1);
    dyn_arr_append(arr, s2);

    mu_assert(strcmp(arr[0], "hello") == 0, "arr[0] should be %s", "hello");
    mu_assert(strcmp(arr[1], "good morning") == 0, "arr[1] should be %s", "good morning");

    str_destroy(s1);
    str_destroy(s2);
    dyn_arr_destroy(arr);
    return 0;
}

char * test_str_tokenize()
{
    size_t i;
    char ** arr = str_tokenize("hi there, what are you doing?", " ");

    mu_assert(strcmp(arr[0], "hi") == 0,        "arr[0] should be %s", "hi");
    mu_assert(strcmp(arr[1], "there,") == 0,    "arr[1] should be %s", "there,");
    mu_assert(strcmp(arr[2], "what") == 0,      "arr[2] should be %s", "what");
    mu_assert(strcmp(arr[3], "are") == 0,       "arr[3] should be %s", "are");
    mu_assert(strcmp(arr[4], "you") == 0,       "arr[4] should be %s", "you");
    mu_assert(strcmp(arr[5], "doing?") == 0,    "arr[5] should be %s", "doing");
    mu_assert(dyn_arr_length(arr) == 6,         "arr length should be %d", 6);

    for (i=0; i<dyn_arr_length(arr); i++) {
        str_destroy(arr[i]);
    }
    dyn_arr_destroy(arr);

    return 0;
}

char * test_str_tokenize_file()
{
    size_t i;
    FILE * fp = Fopen("data/hello.txt", "r");
    char * file_contents = str_create();
    while (fgets(buffer, BUFFER_LEN, fp) != NULL)
        str_concat(file_contents, buffer);
    fclose(fp);
    char ** tokens = str_tokenize(file_contents, " \n");
    str_destroy(file_contents);

    //for (i=0; i<dyn_arr_length(tokens); i++)
    //    printf("token[%zu]: '%s'\n", i, tokens[i]);

    for (i=0; i<dyn_arr_length(tokens); i++)
        str_destroy(tokens[i]);
    dyn_arr_destroy(tokens);

    return 0;
}

char * str_tests()
{
    mu_run_test(test_str);
    mu_run_test(test_dyn_arr_str);
    mu_run_test(test_str_tokenize);
    mu_run_test(test_str_tokenize_file);
    return 0;
}
