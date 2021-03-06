#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>
#include <stdio.h>
#include "list.h"

/* TODO: variadic append
 * TODO: write inline error handling wrapper functions
 * TODO: use DEBUG
 * TODO: sort function, reverse, contains, index_of, unique
 * TODO: consider adding some spare node pointers
 * TODO: separate interface from implementation
 */

int list_error = 0;

list_t list_create() {
    list_t new_list;
    new_list.head = NULL;
    return new_list;
}

static list_node_t * list_traverse(const list_t * list, size_t index) {

    list_node_t * node;
    int get_last;
    list_error = 0;

    if (list == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return NULL;
    }

    node = list->head;

    if (node == NULL) {
        list_error = LIST_ERR_INVALID_INDEX;
        return NULL;
    }

    get_last = (index == SIZE_MAX) ? 1 : 0;

    while (index > 0) {
        if (node->next == NULL) {
            if (get_last)
                break;
            list_error = LIST_ERR_INVALID_INDEX;
            return NULL;
        }
        node = node->next;
        index--;
    }

    return node;
}

node_data_t list_get(const list_t * list, size_t index) {

    const list_node_t * node;
    list_error = 0;

    if (list == NULL) {
        node_data_t dummy = { 0 };
        list_error = LIST_ERR_NULL_LIST_PTR;
        return dummy;
    }

    node = list_traverse(list, index);

    if (node == NULL) {
        node_data_t dummy = { 0 };
        list_error = LIST_ERR_INVALID_INDEX;
        return dummy;
    }

    return node->data;
}

void list_set(list_t * list, size_t index, node_data_t x) {

    list_node_t * node;
    list_error = 0;

    if (list == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return;
    }

    node = list_traverse(list, index);
    if (node == NULL && list_error == LIST_ERR_INVALID_INDEX) {
        return;
    }
    node->data = x;
}

/* TODO: think about error handling */
void list_append(list_t * list, node_data_t x) {
    list_node_t * new_node;
    list_error = 0;
    if (list == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return;
    }

    new_node = malloc(sizeof(*new_node));

    new_node->data = x;
    new_node->next = NULL;

    if (list->head == NULL) {
        list->head = new_node;
    } else {
        list_node_t * node = list_traverse(list, SIZE_MAX);
        node->next = new_node;
    }
}

void list_insert(list_t * list, size_t index, node_data_t x) {
    list_error = 0;
    if (list == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return;
    }

    if (index == 0) {
        list_node_t * new_node = malloc(sizeof(*new_node));
        new_node->data = x;
        new_node->next = list->head;
        list->head = new_node;
    } else {
        list_node_t * node = list_traverse(list, index-1);
        list_node_t * new_node;
        if (node == NULL)
            return;
        new_node = malloc(sizeof(*new_node));
        new_node->data = x;
        new_node->next = node->next;
        node->next = new_node;
    }
}

void list_remove(list_t * list, size_t index) {
    list_error = 0;
    if (list == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return;
    }
    if (list->head == NULL) {
        list_error = LIST_ERR_INVALID_INDEX;
        return;
    }

    if (index == 0) {
        list_node_t * old_head = list->head;
        list->head = old_head->next;
        free(old_head);
    } else {
        list_node_t * node = list_traverse(list, index-1);
        list_node_t * old_node;
        if (node == NULL || node->next == NULL) {
            list_error = LIST_ERR_INVALID_INDEX;
            return;
        }
        old_node = node->next;
        node->next = old_node->next;
        free(old_node);
    }
}

static void list_destroy_recursive(list_node_t * node) {
    if (node == NULL)
        return;

    if (node->next != NULL) {
        list_destroy_recursive(node->next);
        node->next = NULL;
    }
    free(node);
}

void list_destroy(list_t * list) {
    list_error = 0;
    if (list == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return;
    }
    list_destroy_recursive(list->head);
    list->head = NULL;
}

void list_clear(list_t * list) {
    list_destroy(list);
}

/* TODO: should this return error if list == NULL? */
size_t list_length(const list_t * list) {
    size_t n;
    list_node_t * node;
    if (list == NULL)
        return 0;

    node = list->head;

    for (n=0; node != NULL; n++) {
        node = node->next;
    }

    return n;
}

list_t list_dup(const list_t * src) {

    list_t dst = list_create();
    const list_node_t * src_node_ptr;
    list_node_t * prev_dst_node_ptr = NULL;
    list_error = 0;

    if (src == NULL) {
        list_error = LIST_ERR_NULL_LIST_PTR;
        return dst;
    }

    src_node_ptr = src->head;

    while (src_node_ptr != NULL) {
        list_node_t * dst_node_ptr = malloc(sizeof(*dst_node_ptr));
        dst_node_ptr->next = NULL;
        dst_node_ptr->data = src_node_ptr->data;
        if (prev_dst_node_ptr == NULL)
            dst.head = dst_node_ptr;
        else
            prev_dst_node_ptr->next = dst_node_ptr;
        prev_dst_node_ptr = dst_node_ptr;
        src_node_ptr = src_node_ptr->next;
    }

    return dst;
}

/* TODO: make string big enough */
/* TODO: should maybe accept function pointer to stringify each item */
char * list_to_str(const list_t * list) {
    char * str;
    char * buffer;
    const list_node_t * node;
    int remaining;

    if (list == NULL)
        return NULL;

    str = malloc(sizeof(*str)*1024);
    buffer = str;
    node = list->head;
    buffer[0] = '[';
    buffer[1] = '\0';
    buffer++;
    remaining = 1023;
    while (node != NULL) {
        const char * fmt = (node->next == NULL) ? "%d" : "%d, ";
        /* int num_bytes = snprintf(buffer, remaining, fmt, node->data.i); */
        int num_bytes = sprintf(buffer, fmt, node->data.i);
        node = node->next;
        buffer += num_bytes;
        remaining -= num_bytes;
    }
    sprintf(buffer, "]");
    return str;
}
