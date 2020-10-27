#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "minunit.h"

//char * test_empty_list();
//char * test_list_length_1();
//char * test_list_length_n();
//char * test_list_insert();
//char * test_list_remove();
//char * test_list_set();
//char * test_list_dup();
//char * test_list_insert_invalid();
//char * test_list_remove_invalid();
//char * test_list_get_set_invalid();
//char * test_list_to_str();

char * test_empty_list()
{
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;
    size_t ll = list_length(new_list_ptr);
    mu_assert(ll == 0, "empty list should have length of 0, not %zu", ll);

    return 0;
}

char * test_list_length_1() {
    size_t ll;
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    list_append(new_list_ptr, (node_data_t) 1);
    mu_assert(list_get(new_list_ptr, 0).i == 1, "should be %d", 1);

    ll = list_length(new_list_ptr);
    mu_assert(ll == 1, "should be 1, is %zu", ll);

    list_destroy(new_list_ptr);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 0, "should be 0, is %zu", ll);

    return 0;
}

// TODO: can this be parameterized?
char * test_list_length_n() {
    size_t n = 5;
    size_t ll;
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    for(size_t i=0; i<n; i++) {
        list_append(new_list_ptr, (node_data_t) i);
        int data = list_get(new_list_ptr, i).i;
        mu_assert(data == i, "value at %zu should be %ld, is %d", i, i, data);

        ll = list_length(new_list_ptr);
        mu_assert(ll == i+1, "length should be %zu, is %zu", i+1, ll);
    }

    list_destroy(new_list_ptr);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 0, "should be 0, is %zu", ll);

    return 0;
}

char * test_list_insert() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;
    size_t ll;

    // [5] -> [4,5]
    list_append(new_list_ptr, (node_data_t) 5);
    list_insert(new_list_ptr, 0, (node_data_t) 4);

    // confirm length == 2
    ll = list_length(new_list_ptr);
    mu_assert(ll == 2, "length should be %u, is %zu", 2U, ll);

    // confirm list[0] == 4
    int n0 = list_get(new_list_ptr, 0).i;
    mu_assert(n0==4, "should be 4, not %d", n0);

    // confirm list[0] == 5
    int n1 = list_get(new_list_ptr, 1).i;
    mu_assert(n1==5, "should be 5, not %d", n1);

    // [4,5] -> [4,3,5]
    list_insert(new_list_ptr, 1, (node_data_t) 3);
    // confirm list[1] == 3
    int n2 = list_get(new_list_ptr, 1).i;
    mu_assert(n2==3, "should be 3, not %d", n2);

    // [4,3,5] -> [4,3,5,6]
    list_insert(new_list_ptr, 3, (node_data_t) 6);
    // confirm list[3] == 6
    int n3 = list_get(new_list_ptr, 3).i;
    mu_assert(n3==6, "should be 3, not %d", n3);

    // confirm length == 4
    ll = list_length(new_list_ptr);
    mu_assert(ll == 4, "length should be %u, is %zu", 4U, ll);

    list_destroy(new_list_ptr);

    ll = list_length(new_list_ptr);
    mu_assert(ll == 0, "length should be %u, is %zu", 0U, ll);

    return 0;
}

char * test_list_remove() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;
    size_t ll;
    int data;

    // list = [1,2,3,4]
    list_append(new_list_ptr, (node_data_t) 1);
    list_append(new_list_ptr, (node_data_t) 2);
    list_append(new_list_ptr, (node_data_t) 3);
    list_append(new_list_ptr, (node_data_t) 4);

    // remove list[1]
    list_remove(new_list_ptr, 1);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 3, "length should be %u, is %zu", 3U, ll);

    // confirm list[0]
    data = list_get(new_list_ptr, 0).i;
    mu_assert(data == 1, "data at %u should be %u, is %d", 0U, 1, data);

    // confirm list[1]
    data = list_get(new_list_ptr, 1).i;
    mu_assert(data == 3, "data at %u should be %u, is %d", 1U, 3, data);

    // confirm list[2]
    data = list_get(new_list_ptr, 2).i;
    mu_assert(data == 4, "data at %u should be %u, is %d", 2U, 4, data);


    // remove list[2]
    list_remove(new_list_ptr, 2);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 2, "length should be %u, is %zu", 2U, ll);


    // remove list[0]
    list_remove(new_list_ptr, 0);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 1, "length should be %u, is %zu", 1U, ll);


    // remove list[0]
    list_remove(new_list_ptr, 0);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 0, "length should be %u, is %zu", 0U, ll);


    // destroy
    list_destroy(new_list_ptr);
    ll = list_length(new_list_ptr);
    mu_assert(ll == 0, "length should be %u, is %zu", 0U, ll);

    return 0;
}

char * test_list_set() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;
    int data;

    list_append(new_list_ptr, (node_data_t) 1);
    list_set(new_list_ptr, 0, (node_data_t) 2);
    data = list_get(new_list_ptr, 0).i;
    mu_assert(data == 2, "data at %u should be %u, is %d", 0U, 2, data);

    list_set(new_list_ptr, 1, (node_data_t) 3);
    mu_assert(list_error == LIST_ERR_INVALID_INDEX, "list_set with index %u should be an error", 1U);

    list_destroy(new_list_ptr);

    return 0;
}

char * test_list_dup() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;
    int data;

    list_append(new_list_ptr, (node_data_t) 1);
    list_append(new_list_ptr, (node_data_t) 2);
    list_append(new_list_ptr, (node_data_t) 3);

    list_t dup_list = list_dup(new_list_ptr);
    list_t * dup_list_ptr = &dup_list;

    data = list_get(dup_list_ptr, 0).i;
    mu_assert(data == 1, "data at %u should be %u, is %d", 0U, 1, data);

    data = list_get(dup_list_ptr, 1).i;
    mu_assert(data == 2, "data at %u should be %u, is %d", 1U, 2, data);

    data = list_get(dup_list_ptr, 2).i;
    mu_assert(data == 3, "data at %u should be %u, is %d", 2U, 3, data);

    list_destroy(new_list_ptr);
    list_destroy(dup_list_ptr);

    return 0;
}

char * test_list_insert_invalid() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    list_insert(new_list_ptr, 1, (node_data_t) 1);
    mu_assert(list_error != 0, "list_insert with index %u should be an error", 1U);

    list_destroy(new_list_ptr);

    return 0;
}

char * test_list_remove_invalid() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    list_append(new_list_ptr, (node_data_t) 1);
    list_remove(new_list_ptr, 1);
    mu_assert(list_error != 0, "list_remove with index %u should be an error", 1U);

    list_append(new_list_ptr, (node_data_t) 2);
    list_remove(new_list_ptr, 3);
    mu_assert(list_error != 0, "list_remove with index %u should be an error", 2U);

    list_remove(new_list_ptr, 0);
    list_remove(new_list_ptr, 0);
    mu_assert(list_error == 0, "list_remove with index %u should NOT be an error", 0U);
    list_remove(new_list_ptr, 0);
    mu_assert(list_error != 0, "list_remove with index %u should be an error", 1U);

    list_destroy(new_list_ptr);

    return 0;
}

char * test_list_get_set_invalid() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    list_append(new_list_ptr, (node_data_t) 1);
    list_append(new_list_ptr, (node_data_t) 2);
    list_append(new_list_ptr, (node_data_t) 3);

    int n0 = list_get(new_list_ptr, 3).i;
    mu_assert(list_error != 0, "list_remove with index %u should be an error (%d)", 3U, n0);

    list_set(new_list_ptr, 4, (node_data_t) 0);
    mu_assert(list_error != 0, "list_remove with index %u should be an error", 4U);

    list_destroy(new_list_ptr);

    return 0;
}

char * test_list_to_str() {
    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    char * my_str = list_to_str(new_list_ptr);
    int result = strcmp(my_str, "[]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_append(new_list_ptr, (node_data_t) 1);
    my_str = list_to_str(new_list_ptr);
    result = strcmp(my_str, "[1]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_append(new_list_ptr, (node_data_t)2);
    my_str = list_to_str(new_list_ptr);
    result = strcmp(my_str, "[1, 2]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_append(new_list_ptr, (node_data_t)3);
    my_str = list_to_str(new_list_ptr);
    result = strcmp(my_str, "[1, 2, 3]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_remove(new_list_ptr, 1);
    my_str = list_to_str(new_list_ptr);
    result = strcmp(my_str, "[1, 3]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_remove(new_list_ptr, 1);
    my_str = list_to_str(new_list_ptr);
    result = strcmp(my_str, "[1]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_remove(new_list_ptr, 0);
    my_str = list_to_str(new_list_ptr);
    result = strcmp(my_str, "[]");
    mu_assert(result == 0, "string compare should be %d", 0);
    free(my_str);

    list_destroy(new_list_ptr);

    return 0;
}

char * list_tests()
{
    mu_run_test(test_empty_list);
    mu_run_test(test_list_length_1);
    mu_run_test(test_list_length_n);
    mu_run_test(test_list_insert);
    mu_run_test(test_list_remove);
    mu_run_test(test_list_set);
    mu_run_test(test_list_dup);
    mu_run_test(test_list_insert_invalid);
    mu_run_test(test_list_remove_invalid);
    mu_run_test(test_list_get_set_invalid);
    mu_run_test(test_list_to_str);

    return 0;
}
