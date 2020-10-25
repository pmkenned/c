#include <malloc.h>
#include <string.h>
#include <ctype.h>
#include "str.h"
#include "dyn_arr.h"

// TODO: require cap of at least 1
char * str_create_prealloc(size_t cap)
{
    char * s = malloc(STR_HEADER_SIZE + sizeof(*s)*(cap+1)) + STR_HEADER_SIZE;
    s[0] = '\0';

    _str_field_set(s, STR_CAPACITY, cap);
    _str_field_set(s, STR_LENGTH, 0);

    return s;
}

char * str_create_from(const char * s)
{
    size_t l = strlen(s);
    char * new_s = str_create_prealloc(l);
    strcpy(new_s, s);
    _str_field_set(new_s, STR_LENGTH, l);
    _str_field_set(new_s, STR_CAPACITY, l);
    return new_s;
}

char * _str_copy(char * dst, const char * src)
{
    size_t len = strlen(src);
    if (len > str_capacity(dst))
        dst = _str_resize(dst, len);
    strcpy(dst, src);
    _str_field_set(dst, STR_LENGTH, len);
    return dst;
}

char ** str_tokenize(const char * s, const char * delim)
{
    size_t l = strlen(s);

    char * temp = malloc(sizeof(*temp)*(l+1));
    strcpy(temp, s);

    char ** str_arr = dyn_arr_create(char *);

    char * tok = strtok(temp, delim);
    while (tok != NULL) {
        char * new_s = str_create_from(tok);
        dyn_arr_append(str_arr, new_s);
        tok = strtok(NULL, delim);
    }

    free(temp);

    return str_arr;
}

char * _str_resize(char * s, size_t new_cap)
{
    s = realloc(s - STR_HEADER_SIZE, STR_HEADER_SIZE + sizeof(*s)*(new_cap+1)) + STR_HEADER_SIZE;
    _str_field_set(s, STR_CAPACITY, new_cap);
    return s;
}

char * _str_concat(char * dst, const char * src)
{
    size_t dst_l = str_length(dst);
    size_t src_l = strlen(src);
    if (dst_l + src_l > str_capacity(dst))
        dst = _str_resize(dst, dst_l + src_l);
    strcpy(dst + dst_l, src);
    _str_field_set(dst, STR_LENGTH, dst_l + src_l);
    return dst;
}

void str_destroy(char * s)
{
    free(s - sizeof(size_t)*STR_FIELDS);
}

size_t _str_field_get(const char * s, size_t field)
{
    return ((size_t *)s - STR_FIELDS)[field];
}

void _str_field_set(char * s, size_t field, size_t value)
{
    ((size_t *)s - STR_FIELDS)[field] = value;
}

void str_tolower(char * s)
{
    size_t i;
    for (i=0; s[i]; i++)
        s[i] = tolower(s[i]);
}
