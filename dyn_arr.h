#ifndef DYN_ARR_H
#define DYN_ARR_H

// TODO:
// * append (push_back, push_front, pop_back, pop_front)
// * concat
// * get by index
// * set by index
// * remove by index
// * insert at index
// * reserve
// * get capacity
// * resize
// * sort (qsort?)
// * clear
// * contains
// * index of
// * copy
// * get data ptr?

#define DYN_ARR_INIT_CAP 10
#define DYN_ARR_RESIZE_FACTOR 2

enum {
    DYN_ARR_CAPACITY,
    DYN_ARR_LENGTH,
    DYN_ARR_STRIDE,
    DYN_ARR_FIELDS
};

void *      _dyn_arr_create(size_t init_cap, size_t stride);
void        _dyn_arr_destroy(void * dyn_arr);
size_t      _dyn_arr_field_get(const void * dyn_arr, size_t field);
void        _dyn_arr_field_set(void * dyn_arr, size_t field, size_t value);
void *      _dyn_arr_append(void * dyn_arr, void * x);
void *      _dyn_arr_append_arr(void * dst, void * src);

#define dyn_arr_capacity(arr)               _dyn_arr_field_get(arr, DYN_ARR_CAPACITY)
#define dyn_arr_length(arr)                 _dyn_arr_field_get(arr, DYN_ARR_LENGTH)
#define dyn_arr_stride(arr)                 _dyn_arr_field_get(arr, DYN_ARR_STRIDE)

#define dyn_arr_create(type)                _dyn_arr_create(DYN_ARR_INIT_CAP, sizeof(type))
#define dyn_arr_create_prealloc(type, cap)  _dyn_arr_create(cap, sizeof(type))
#define dyn_arr_destroy(arr)                _dyn_arr_destroy(arr)

#define dyn_arr_append(arr, x)              arr = _dyn_arr_append(arr, &x)
#define dyn_arr_append_rval(arr, x) \
    do { \
        __auto_type temp = x; \
        arr = _dyn_arr_append(arr, &temp); \
    } while(0)

#define dyn_arr_append_arr(dst, src)        dst = _dyn_arr_append_arr(dst, src)

#endif /* DYN_ARR_H */
