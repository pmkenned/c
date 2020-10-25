#ifndef LIST_H
#define LIST_H

extern int list_error;

#define LIST_ERR_INVALID_INDEX 1
#define LIST_ERR_NULL_LIST_PTR 2

// NOTE: a list of chars is 4 (or 8) bytes each...
typedef union {
    void * ptr;
    short s;
    int i;
    unsigned int ui;
    long l;
    unsigned long ul;
    char c;
    double d;
    float f;
} node_data_t;

// TODO: add field identifying data type
// TODO: make this opaque?
typedef struct list_node {
    node_data_t data;
    struct list_node * next;
} list_node_t;

// TODO: consider this way of separating interface from impl:
// struct list_t;
// typedef struct list_t * list_t;

typedef struct {
    list_node_t * head;
} list_t;

list_t      list_create();
node_data_t list_get(const list_t * list, size_t index);
void        list_set(list_t * list, size_t index, node_data_t x);
void        list_append(list_t * list, node_data_t x);
void        list_insert(list_t * list, size_t index, node_data_t x);
void        list_remove(list_t * list, size_t index);
void        list_destroy(list_t * list);
size_t      list_length(const list_t * list);
list_t      list_dup(const list_t * src);
char *      list_to_str(const list_t * list);

#endif
