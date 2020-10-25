#include <stdio.h>
#include <getopt.h>
#include <assert.h>
#include "list.h"

// TODO:
// * Data structures
//     * doubly linked list
//     * dynamic array
//     * strings
//     * hashmap
//     * graph
//     * tree
// * Network code

// TODO:
// * write documentation
// * malloc debug

// Tools to learn:
// * make
// * git
// * valgrind
// * splint
// * makeprg (vim)

int verbose = 0;

int main(int argc, char ** argv) {

    int c;
    int digit_optind = 0;

    while (1) {
        int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"add",     required_argument, NULL,        0 },
            {"append",  no_argument,       NULL,        0 },
            {"delete",  required_argument, NULL,        0 },
            {"verbose", no_argument,       &verbose,    1 },
            {"create",  required_argument, NULL,        'c'},
            {"file",    required_argument, NULL,        0 },
            {0,         0,                 NULL,        0 }
        };

        c = getopt_long(argc, argv, "abc:d:012", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
        case 0:
            printf("option %s", long_options[option_index].name);
            if (optarg)
                printf(" with arg %s", optarg);
            printf("\n");
            break;

        case '0':
        case '1':
        case '2':
            if (digit_optind != 0 && digit_optind != this_option_optind)
                printf("digits occur in two different argv-elements.\n");
            digit_optind = this_option_optind;
            printf("option %c\n", c);
            break;

        case 'a':
            printf("option a\n");
            break;

        case 'b':
            printf("option b\n");
            break;

        case 'c':
            printf("option c with value '%s'\n", optarg);
            break;

        case 'd':
            printf("option d with value '%s'\n", optarg);
            break;

        case '?':
            break;

        default:
            printf("?? getopt returned character code 0%o ??\n", c);
        }
    }


    if (verbose) {
        printf("verbose\n");
    }

    if (optind < argc) {
        printf("non-option ARGV-elements: ");
        while (optind < argc)
            printf("%s ", argv[optind++]);
        printf("\n");
    }

    list_t new_list = list_create();
    list_t * new_list_ptr = &new_list;

    list_append(new_list_ptr, (node_data_t) 1);

    node_data_t n0 = list_get(new_list_ptr, 0);

    printf("list[0]: %d\n", n0.i);

    assert(1==0);

#ifdef DEBUG
    printf("debug\n");
#else
    printf("no debug\n");
#endif

    return 0;
}
