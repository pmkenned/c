#ifndef MAP_H
#define MAP_H

#ifdef __cplusplus
extern "C" {
#endif

#define MAP_INIT_CAP 10
#define MAP_RESIZE_FACTOR 2

/* TODO:
   remove
   clear
 */

/* TODO: currently, if you want to 'get' the value or create it if it isn't already present,
   you must first get and then set, thus requiring two index's. make this more efficient */

typedef struct map_t map_t;

map_t * _map_create(size_t value_size, size_t cap);
void _map_destroy(map_t * map);

/* void *_map_get(const map_t * map, const char * key); */
int _map_get(const map_t * map, const char * key, void * dest);
void * _map_get_ptr(const map_t * map, const char * key);
void _map_set(map_t * map, const char * key, const void * value);

/* void map_print_keys(const map_t * map); */
void map_print(const map_t * map);
void map_print_l2(const map_t * map);

#define map_create(value_type)                  _map_create(sizeof(value_type), MAP_INIT_CAP)
#define map_create_prealloc(value_type, cap)    _map_create(sizeof(value_type), cap)
#define map_destroy(map)                        _map_destroy(map)

/* #define map_get(map, key, type)                 (*((type *) _map_get(map, key))) */
#define map_get(map, key, dest)                 _map_get(map, key, dest)
#define map_get_ptr(map, key)                   _map_get_ptr(map, key)
#define map_set(map, key, value)                _map_set(map, key, &value)
#define map_set_rvalue(map, key, value) \
    do { \
        __auto_type temp = value; \
        _map_set(map, key, &temp); \
    } while (0)

#ifdef __cplusplus
}
#endif

#endif /* MAP_H */
