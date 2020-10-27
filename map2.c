#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include "map2.h"

struct map2_t {
    size_t value_size;
    size_t cap;
    char * keys[1];
};

char * hello = "hello";

map2_t * _map2_create(size_t value_size, size_t cap)
{
    map2_t * map = malloc(sizeof(*map) + sizeof(char *)*(cap-1) + value_size*cap);

    map->value_size = value_size;
    map->cap = cap;
    memset(map->keys, 0, (sizeof(char *)+value_size)*cap);

    //for (i=0; i<cap; i++)
    //    map->keys[i] = NULL;
    //memset(MAP2_VALUES(map), 0, cap*value_size);

    //map->keys[2] = hello;
    //((int *)MAP2_VALUES(map))[2] = 3;

    return map;
}

void map2_destroy(map2_t * map)
{
    free(map);
}

// TODO: fix memory leak
void map2_set(map2_t * map, const char * key, const void * value)
{
    size_t idx = key[0] % map->cap; // TODO
    map->keys[idx] = malloc(strlen(key)+1);
    strcpy(map->keys[idx], key);
    MAP2_VALUES_AS(map, char *)[idx] = malloc(strlen(value)+1);
    strcpy(MAP2_VALUES_AS(map, char *)[idx], value);
}

void map2_print_str(map2_t * map)
{
    size_t i;
    char ** values = MAP2_VALUES(map);
    for (i=0; i<map->cap; i++) {
        if (map->keys[i] == NULL)
            continue;
        printf("%s: %s\n", map->keys[i], values[i]);
    }
}

void map2_print(map2_t * map)
{
    size_t i;
    int * values = MAP2_VALUES(map);
    for (i=0; i<map->cap; i++) {
        if (map->keys[i] == NULL)
            continue;
        printf("%s: %d\n", map->keys[i], values[i]);
    }

}

/* type b */

//// TODO: think about padding
//typedef struct {
//    char * key;
//    int value;
////    void * value;
//} kv_pair_t;
//
//struct map2b_t {
//    type_t type;
//    size_t cap;
//    kv_pair_t kvs[1];
//};

struct map2b_t {
    type_t type;
    size_t cap;
};

// INT

typedef struct {
    char * key;
    int value;
} kv_int_t;

typedef struct {
    type_t type;
    size_t cap;
    kv_int_t kvs[1];
} map2b_int_t;

// PTR

typedef struct {
    char * key;
    const void * value;
} kv_ptr_t;

typedef struct {
    type_t type;
    size_t cap;
    kv_ptr_t kvs[1];
} map2b_ptr_t;

// C_STR

typedef struct {
    char * key;
    char * value;
} kv_c_str_t;

typedef struct {
    type_t type;
    size_t cap;
    kv_c_str_t kvs[1];
} map2b_c_str_t;

//map2b_t * _map2b_create(type_t type, size_t cap)
//{
//    map2b_t * map = malloc(sizeof(*map) + sizeof(kv_pair_t)*(cap-1));
//
//    map->type = type;
//    map->cap = cap;
//    memset(map->kvs, 0, sizeof(kv_pair_t)*cap);
//
//    return map;
//}

map2b_t * _map2b_create(type_t type, size_t cap)
{
    map2b_t * map;

    switch (type) {
        case TYPE_INT:
            map = malloc(sizeof(map2b_int_t) + sizeof(kv_int_t)*(cap-1));
            memset(((map2b_int_t *)map)->kvs, 0, sizeof(kv_int_t)*cap);
            break;
        case TYPE_C_STR:
            map = malloc(sizeof(map2b_c_str_t) + sizeof(kv_c_str_t)*(cap-1));
            memset(((map2b_c_str_t *)map)->kvs, 0, sizeof(kv_c_str_t)*cap);
            break;
        case TYPE_PTR:
            map = malloc(sizeof(map2b_ptr_t) + sizeof(kv_ptr_t)*(cap-1));
            memset(((map2b_ptr_t *)map)->kvs, 0, sizeof(kv_ptr_t)*cap);
            break;
        default:
            map = NULL;
            return NULL;
            break;
    }

    map->type = type;
    map->cap = cap;

    return map;
}

void map2b_destroy(map2b_t * map)
{
    size_t i;
    for (i=0; i<map->cap; i++) {
        switch (map->type) {
            case TYPE_INT:
                if (((map2b_int_t *)map)->kvs[i].key != NULL)
                    free(((map2b_int_t *)map)->kvs[i].key);
                break;
            case TYPE_C_STR:
                if (((map2b_c_str_t *)map)->kvs[i].key != NULL)
                    free(((map2b_c_str_t *)map)->kvs[i].key);
                if (((map2b_c_str_t *)map)->kvs[i].value != NULL)
                    free(((map2b_c_str_t *)map)->kvs[i].value);
                break;
            case TYPE_PTR:
                if (((map2b_ptr_t *)map)->kvs[i].key != NULL)
                    free(((map2b_ptr_t *)map)->kvs[i].key);
            default:
                break;
        }
        //if (map->kvs[i].value != NULL)
        //    free(map->kvs[i].value);
    }
}

void map2b_set(map2b_t * map, const char * key, const void * value)
{
    size_t idx = key[0] % map->cap; // TODO
    //map->kvs[idx].key = malloc(strlen(key)+1);
    //strcpy(map->kvs[idx].key, key);
    switch (map->type) {
        case TYPE_INT: {
            map2b_int_t * _map = (map2b_int_t *) map;
            _map->kvs[idx].key = malloc(strlen(key)+1);
            strcpy(_map->kvs[idx].key, key);
            _map->kvs[idx].value = *((int*)value);
            }
            break;
        case TYPE_C_STR: {
            map2b_c_str_t * _map = (map2b_c_str_t *) map;
            _map->kvs[idx].key = malloc(strlen(key)+1);
            strcpy(_map->kvs[idx].key, key);
            _map->kvs[idx].value = malloc(strlen(value)+1);
            strcpy(_map->kvs[idx].value, value);
            }
            break;
        case TYPE_PTR: {
            map2b_ptr_t * _map = (map2b_ptr_t *) map;
            _map->kvs[idx].key = malloc(strlen(key)+1);
            strcpy(_map->kvs[idx].key, key);
            _map->kvs[idx].value = value;
            }
            break;
        default:
            break;
    }
    //map->kvs[idx].value = malloc(strlen(value)+1);
    //strcpy(map->kvs[idx].value, value);
}

void map2b_print(map2b_t * map)
{
    size_t i;
    for (i=0; i<map->cap; i++) {
        //if (map->kvs[i].key == NULL)
        //    continue;
        switch (map->type) {
            case TYPE_INT:
                if (((map2b_int_t *)map)->kvs[i].key == NULL)
                    continue;
                printf("%s: %d\n", ((map2b_int_t *)map)->kvs[i].key, ((map2b_int_t *)map)->kvs[i].value);
                break;
            case TYPE_C_STR:
                if (((map2b_c_str_t *)map)->kvs[i].key == NULL)
                    continue;
                printf("%s: %s\n", ((map2b_c_str_t *)map)->kvs[i].key, ((map2b_c_str_t *)map)->kvs[i].value);
                break;
            case TYPE_PTR:
                if (((map2b_ptr_t *)map)->kvs[i].key == NULL)
                    continue;
                printf("%s: %p\n", ((map2b_ptr_t *)map)->kvs[i].key, ((map2b_ptr_t *)map)->kvs[i].value);
                break;
            default:
                //printf("%s: %p\n", map->kvs[i].key, (const char *)(map->kvs[i].value));
                break;
        }
    }
}
