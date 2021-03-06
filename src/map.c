#include <malloc.h>
#include <string.h>
#include <assert.h>
#include <stdio.h>
#include "dyn_arr.h"
#include "str.h"
#include "map.h"

/* TODO: add typedef for str for clarity */

struct map_t {
    size_t value_size;
    size_t n_entries;
    size_t cap;
    char ** keys;
    void * values;
};

map_t * _map_create(size_t value_size, size_t cap)
{
    size_t i;
    map_t * map = malloc(sizeof(*map));
    map->value_size = value_size;
    map->cap = cap;
    map->n_entries = 0;
    /* NOTE: keys doesn't have to be a dynamic array */
    map->keys = dyn_arr_create_prealloc(char *, cap);
    for (i=0; i<cap; i++)
        dyn_arr_append_rval(map->keys, str_create());
    /*map->values = malloc(value_size*cap); */
    map->values = calloc(cap, value_size);
    return map;
}

void _map_destroy(map_t * map)
{
    size_t i;
    free(map->values);
    for (i=0; i<dyn_arr_length(map->keys); i++)
        str_destroy(map->keys[i]);
    dyn_arr_destroy(map->keys);
    free(map);
}

size_t _map_hash(const char * s)
{
	size_t hash = 5381;
	int c;
	while ((c = *s++))
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
	return hash;
}

size_t _map_index(const map_t * map, const char * key, int * found, int * open)
{
    int checked_all = 0;
    int _found = 0, _open = 0;
    size_t idx = _map_hash(key) % map->cap;
    size_t init_idx = idx;
    /* TODO: replace strcmp with faster comparison function */
    while ( !(_open = (str_length(map->keys[idx]) == 0)) &&
            !(_found = (strcmp(map->keys[idx], key) == 0)) &&
            !checked_all) {
        idx = (idx+1) % map->cap;
        checked_all = (idx == init_idx);
    }

    if (found) *found = _found;
    if (open) *open= _open;

    return idx;
}

void _map_resize(map_t * map)
{
    size_t i;
    size_t new_cap = map->cap * MAP_RESIZE_FACTOR;
    map_t * new_map = _map_create(map->value_size, new_cap);

    for (i=0; i < dyn_arr_length(map->keys); i++) {
        const void * value;
        if (str_length(map->keys[i]) == 0)
            continue;
        value = (char *)(map->values) + i*(map->value_size);
        _map_set(new_map, map->keys[i], value);
    }

    /* destroy old map */
    free(map->values);
    for (i=0; i<dyn_arr_length(map->keys); i++)
        str_destroy(map->keys[i]);
    dyn_arr_destroy(map->keys);

    map->values = new_map->values;
    map->keys = new_map->keys;
    map->cap = new_cap;

    free(new_map);
    /*return new_map; */
}

#if 0
void * _map_get(const map_t * map, const char * key)
{
    int open = 0, found = 0;
    size_t idx = _map_index(map, key, &found, &open);
    return (char *)(map->values) + idx*(map->value_size);
}
#endif

int _map_get(const map_t * map, const char * key, void * dest)
{
    int open = 0, found = 0;
    size_t idx = _map_index(map, key, &found, &open);
    if (found)
        memcpy(dest, (char *)(map->values) + idx*(map->value_size), map->value_size);
    return found;
}

void * _map_get_ptr(const map_t * map, const char * key)
{
    int open = 0, found = 0;
    size_t idx = _map_index(map, key, &found, &open);
    if (found)
        return (char *)(map->values) + idx*(map->value_size);
    else
        return NULL;
}

void _map_set(map_t * map, const char * key, const void * value)
{
    int open = 0, found = 0;
    size_t idx = _map_index(map, key, &found, &open);

    assert(open || found);

    if (open)
        str_copy(map->keys[idx], key);

    if (open || found) {
        size_t value_size = map->value_size;
        memcpy((char *)(map->values) + idx*value_size, value, value_size);
        if (!found) {
            map->n_entries++;
            if (1.0*map->n_entries/map->cap > 0.9)
                _map_resize(map);
        }
    }
}

#if 0
void map_print_keys(const map_t * map)
{
    size_t i;
    for (i=0; i<dyn_arr_length(map->keys); i++) {
        printf("%s\n", map->keys[i]);
    }
}
#endif

/* TODO: allow specifying format string or something */
void map_print_ptr(const map_t * map)
{
    size_t i;
    for (i=0; i<dyn_arr_length(map->keys); i++) {
        if (str_length(map->keys[i]) > 0) {
            void * value = *((void **)((char *)(map->values) + i*map->value_size));
            printf("%s: %p\n", map->keys[i], value);
        }
    }
}

void map_print(const map_t * map)
{
    size_t i;
    for (i=0; i<dyn_arr_length(map->keys); i++) {
        if (str_length(map->keys[i]) > 0) {
            int value = *((int *)((char *)(map->values) + i*map->value_size));
            printf("%s: %d\n", map->keys[i], value);
        }
    }
}

void _map_print_depth(const map_t * map, int depth)
{
    size_t i;
    int j;
    for (i=0; i<dyn_arr_length(map->keys); i++) {
        if (str_length(map->keys[i]) > 0) {
            int value = *((int *)((char *)(map->values) + i*map->value_size));
            for (j=0; j<depth; j++)
                printf("  ");
            printf("%s: %d\n", map->keys[i], value);
        }
    }
}

void map_print_l2(const map_t * map)
{
    size_t i;
    for (i=0; i<dyn_arr_length(map->keys); i++) {
        if (str_length(map->keys[i]) > 0) {
            map_t * value = *((void **)((char *)(map->values) + i*map->value_size));
            printf("%s:\n", map->keys[i]);
            _map_print_depth(value, 1);
        }
    }
}

