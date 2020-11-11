#include <stddef.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "dyn_arr.h"

/* TODO: think carefully about alignment requirements */
void * _dyn_arr_create(size_t init_cap, size_t stride)
{
    size_t header_size = DYN_ARR_FIELDS * sizeof(size_t);
    size_t arr_size = stride * init_cap;
    size_t * arr = malloc(header_size + arr_size);
    arr[DYN_ARR_LENGTH] = 0;
    arr[DYN_ARR_CAPACITY] = init_cap;
    arr[DYN_ARR_STRIDE] = stride;

    return (void *) (arr + DYN_ARR_FIELDS);
}

void _dyn_arr_destroy(void * arr)
{
    free((size_t *)(arr) - DYN_ARR_FIELDS);
}

size_t _dyn_arr_field_get(const void * arr, size_t field)
{
    return ((size_t *)(arr) - DYN_ARR_FIELDS)[field];
}

void _dyn_arr_field_set(void * arr, size_t field, size_t value)
{
    ((size_t *)(arr) - DYN_ARR_FIELDS)[field] = value;
}

void * _dyn_arr_resize(void * arr)
{
    size_t header_size = DYN_ARR_FIELDS * sizeof(size_t);
    size_t capacity = dyn_arr_capacity(arr);
    size_t stride = dyn_arr_stride(arr);
    capacity *= DYN_ARR_RESIZE_FACTOR;
    arr = (char *) realloc((size_t *)arr - DYN_ARR_FIELDS, header_size + capacity*stride) + header_size;
    _dyn_arr_field_set(arr, DYN_ARR_CAPACITY, capacity);
    return arr;
}

void * _dyn_arr_append(void * arr, void * x)
{
    if (dyn_arr_length(arr) >= dyn_arr_capacity(arr))
        arr = _dyn_arr_resize(arr);
    memcpy((char *)arr + dyn_arr_stride(arr)*dyn_arr_length(arr), x, dyn_arr_stride(arr));
    _dyn_arr_field_set(arr, DYN_ARR_LENGTH, dyn_arr_length(arr)+1);
    return arr;
}

void * _dyn_arr_append_arr(void * dst, void * src)
{
    size_t i;
    size_t src_l = dyn_arr_length(src);
    size_t stride = dyn_arr_stride(src);
    for (i=0; i<src_l; i++) {
        char ** x = (char **) ((char *)src + i*stride);
        dyn_arr_append_rval(dst, *x);
    }
    return dst;
}
