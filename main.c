#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "parse_options.h"

static int help_flag = 0;
static int verbose = 0;
static char * out_file = NULL;
static char * foo_arg  = NULL;
static int    n        = 0;

static char * program_name;
static char * options_str;
 
static void
usage(int exit_code)
{
    fprintf(stderr, "usage: %s [OPTION]... [FILE]...\n", program_name);
    fprintf(stderr, "%s", options_str);
    exit(exit_code);
}

int
main(int argc, char ** argv)
{
    size_t i;
    program_name = argv[0];

    struct my_option my_options[] = {
        {"help",    'h', "show this help",          ARG_NONE, ARG_TYPE_FLAG, &help_flag},
        {"verbose", 'v', "verbose mode",            ARG_NONE, ARG_TYPE_FLAG, &verbose  },
        {"output",  'o', "specify output file",     ARG_MAN,  ARG_TYPE_STR,  &out_file },
        {"foobar",  'f', "wacky option",            ARG_OPT,  ARG_TYPE_STR,  &foo_arg  },
        {"number",  'n', "how many times",          ARG_MAN,  ARG_TYPE_INT,  &n        },
    };

    const size_t num_options = sizeof(my_options)/sizeof(my_options[0]);

    options_str = gen_options_str(my_options, num_options);

    char ** non_option_args;
    int num_non_option_args;

    if (parse_options(argc, argv, my_options, num_options, &non_option_args, &num_non_option_args))
        usage(EXIT_FAILURE);

    for (i = 0; i < num_non_option_args; i++)
        printf("%s\n", non_option_args[i]);

    if (help_flag)
        usage(EXIT_SUCCESS);

    printf("n: %d\n", n);

    return 0;
}
