#ifndef MAP2_H
#define MAP2_H

#define MAP2_INIT_CAP 10

typedef struct map2_t map2_t;

map2_t * _map2_create(size_t value_size, size_t cap);
void map2_destroy(map2_t * map);

void map2_set(map2_t * map, const char * key, const void * value);

void map2_print(map2_t * map);
void map2_print_str(map2_t * map);

#define map2_create(type) _map2_create(sizeof(type), MAP2_INIT_CAP)

#define MAP2_VALUES_AS(map, type) ((type *)((map) + sizeof(*(map)) + sizeof(char *)*((map)->cap-1)))
#define MAP2_VALUES(map) ((void*)((map) + sizeof(*(map)) + sizeof(char *)*((map)->cap-1)))

/* type b */

// TODO: consider: bool, long long, size_t, ptrdiff_t, function pointers
typedef enum {
    TYPE_SHORT,
    TYPE_USHORT,
    TYPE_INT,
    TYPE_UINT,
    TYPE_LONG,
    TYPE_ULONG,
    TYPE_CHAR,
    TYPE_UCHAR,
    TYPE_SCHAR,
    TYPE_FLOAT,
    TYPE_DOUBLE,
    TYPE_C_STR,
    TYPE_PTR,
    TYPE_DYN_ARR,
    TYPE_MAP,
    TYPE_LIST,
    TYPE_COUNT
} type_t;

//// TODO
//enum {
//    TYPE_SHORT_SIZE = sizeof(short),
//    TYPE_USHORT_SIZE = sizeof(unsigned short),
//    TYPE_INT = sizeof(int)
//};

extern const size_t type_sizes[TYPE_COUNT];

#define type_size(type) type_sizes[type]

typedef struct map2b_t map2b_t;

map2b_t * _map2b_create(type_t type, size_t cap);
void map2b_destroy(map2b_t * map);

void map2b_set(map2b_t * map, const char * key, const void * value);

#define map2b_set_rvalue(map, key, value) \
    do { \
        __auto_type temp = value; \
        map2b_set(map, key, &temp); \
    } while (0)

void map2b_print(map2b_t * map);

#define map2b_create(type) _map2b_create(type, MAP2_INIT_CAP)

#endif /* MAP2_H */
