#ifndef STR_H
#define STR_H

// TODO: create string from sprintf
// TODO: char n times

#define STR_INIT_CAP 16

enum {
    STR_CAPACITY,
    STR_LENGTH,
    STR_FIELDS
};

#define STR_HEADER_SIZE sizeof(size_t)*STR_FIELDS

char * str_create_prealloc(size_t cap);
#define str_create() str_create_prealloc(STR_INIT_CAP)

char * str_create_from(const char * s);

char * _str_copy(char * dst, const char * src);

char ** str_tokenize(const char * s, const char * delim);

char * _str_concat(char * dst, const char * src);

char * _str_resize(char * s, size_t new_cap);

size_t _str_field_get(const char * s, size_t field);
void _str_field_set(char * s, size_t field, size_t value);

void str_tolower(char * s);

#define str_copy(dst, src) dst = _str_copy(dst, src)
#define str_concat(dst, src) dst = _str_concat(dst, src)
#define str_capacity(arr) _str_field_get(arr, STR_CAPACITY)
#define str_length(arr) _str_field_get(arr, STR_LENGTH)

void str_destroy(char * s);

#endif /* STR_H */
