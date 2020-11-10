#ifndef PARSE_OPTIONS_H
#define PARSE_OPTIONS_H

#include <stddef.h>

enum {
    ARG_NONE = 0,
    ARG_OPT,
    ARG_MAN
};

typedef enum {
    ARG_TYPE_NONE,
    ARG_TYPE_STR,
    ARG_TYPE_INT,
    ARG_TYPE_FLAG
} arg_type_t;

struct my_option {
    const char *    long_opt;
    char            short_opt;
    const char *    description;
    int             has_arg;
    arg_type_t      arg_type;
    void *          optarg;
};

int     parse_options(int argc, char * argv[], struct my_option * options, size_t num_options, char *** non_option_args, int * num_non_option_args);
char *  gen_options_str(struct my_option * options, size_t num_options);

#endif /* PARSE_OPTIONS_H */
