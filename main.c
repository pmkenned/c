#include <stddef.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "wrapper.h"
#include "parse_options.h"
#include "dyn_arr.h"
#include "str.h"
#include "map.h"

static int help_flag = 0;
static int version_flag = 0;
static int verbose = 0;
static char * out_filename;
static int n = 10;
static int seed;

static char version_str[] = "0.1";
static char * program_name;
static char * options_str;
static FILE * out_fp;
static FILE * in_fp;

enum {
    BUFFER_LEN = 1024
};
static char buffer[BUFFER_LEN];
 
static void
usage(int exit_code)
{
    fprintf(stderr, "Usage: %s [OPTION]... [FILE]...\n", program_name);
    fprintf(stderr, "%s", options_str);
    exit(exit_code);
}

int
main(int argc, char ** argv)
{
    size_t i;
    FILE * default_fp = Fopen("/usr/share/dict/words", "r"); /* stdin */
    char * file_contents;
    char ** non_option_args;
    char ** tokens;
    map_t * token_map = map_create_prealloc(int, 10000);
    size_t num_non_option_args;
    option_t options[] = {
        {"help",    'h',  "show this help",         ARG_NONE, ARG_TYPE_FLAG, &help_flag     },
        {"version", ' ',  "version number",         ARG_NONE, ARG_TYPE_FLAG, &version_flag  },
        {"verbose", 'v',  "verbose mode",           ARG_NONE, ARG_TYPE_FLAG, &verbose       },
        {"output",  'o',  "specify output file",    ARG_MAN,  ARG_TYPE_STR,  &out_filename  },
        {"number",  'n',  "how many times",         ARG_MAN,  ARG_TYPE_INT,  &n             },
        {"seed",    's',  "PRNG seed",              ARG_MAN,  ARG_TYPE_INT,  &seed          },
    };
    const size_t num_options = sizeof(options)/sizeof(options[0]);

    seed = time(NULL);

    program_name = argv[0];
    out_fp = stdout;

    options_str = gen_options_str(options, num_options);

    if (parse_options(argc, argv, options, num_options, &non_option_args, &num_non_option_args))
        usage(EXIT_FAILURE);

    if (help_flag)
        usage(EXIT_SUCCESS);

    if (version_flag) {
        printf("version v%s\n", version_str);
        exit(EXIT_SUCCESS);
    }

    if (out_filename)
        out_fp = Fopen(out_filename, "w");

    /*
    for (i = 0; i < num_non_option_args; i++)
        ;
    */

    in_fp = (num_non_option_args == 0) ? default_fp : Fopen(non_option_args[0], "r");

    file_contents = str_create();

    while (fgets(buffer, BUFFER_LEN, in_fp) != NULL)
        str_concat(file_contents, buffer);
    fclose(in_fp);
    tokens = str_tokenize(file_contents, " \n?.!");

    /* printf("num tokens: %lu\n", (unsigned long) dyn_arr_length(tokens)); */

    srand(seed);
    for (i = 0; i < (size_t) n; i++) {
        size_t mod = dyn_arr_length(tokens);
        size_t r = rand() % mod;
        /* fprintf(out_fp, "%s\n", tokens[r]); */
        int * x_p = map_get_ptr(token_map, tokens[r]);
        if (x_p == NULL)
            map_set_rvalue(token_map, tokens[r], 1);
        else
            (*x_p)++;
    }
    map_print(token_map);

    return 0;
}
