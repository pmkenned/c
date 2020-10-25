#include <stdio.h>
#include <string.h>
#include <time.h>
#include "dyn_arr.h"
#include "str.h"
#include "map.h"
#include "minunit.h"

//char * test_str();
//char * test_dyn_arr_str();
//char * test_str_tokenize();
//char * test_str_tokenize_file();

char * test_str()
{
    char * s = str_create_from("hello, ");
    str_concat(s, "world!");
    mu_assert_old("s should be \"hello, world\"", strcmp(s, "hello, world!") == 0);
    size_t len = str_length(s);
    snprintf(err_msg, ERR_MSG_LEN, "length of s should be %zu, not %zu", sizeof("hello, world!")-1, len);
    mu_assert_old(err_msg, len == sizeof("hello, world!")-1);
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

    mu_assert_old("arr[0] should be \"hello\"", strcmp(arr[0], "hello") == 0);
    mu_assert_old("arr[1] should be \"good morning\"", strcmp(arr[1], "good morning") == 0);

    str_destroy(s1);
    str_destroy(s2);
    dyn_arr_destroy(arr);
    return 0;
}

char * test_str_tokenize()
{
    size_t i;
    char ** arr = str_tokenize("hi there, what are you doing?", " ");

    //for (i=0; i < dyn_arr_length(arr); i++) {
    //    printf("%s\n", arr[i]);
    //}

    mu_assert_old("arr[0] should be \"hi\"",        strcmp(arr[0], "hi") == 0);
    mu_assert_old("arr[1] should be \"there,\"",    strcmp(arr[1], "there,") == 0);
    mu_assert_old("arr[2] should be \"what\"",      strcmp(arr[2], "what") == 0);
    mu_assert_old("arr[3] should be \"are\"",       strcmp(arr[3], "are") == 0);
    mu_assert_old("arr[4] should be \"you\"",       strcmp(arr[4], "you") == 0);
    mu_assert_old("arr[5] should be \"doing\"",     strcmp(arr[5], "doing?") == 0);
    mu_assert_old("arr length should be 6",         dyn_arr_length(arr) == 6);

    for (i=0; i<dyn_arr_length(arr); i++) {
        str_destroy(arr[i]);
    }
    dyn_arr_destroy(arr);

    return 0;
}

char * test_str_tokenize_file()
{
    size_t i;
    FILE * fp = fopen("data/hello.txt", "r");
    mu_assert_old("cannot open file", fp != NULL);
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
