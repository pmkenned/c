#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>

// NOTE: getopt_long sucks so I'm not using it but here's a sloppy example in case I need it

// extern char *optarg: getopt() places a pointer to the following text in the same argv-element, or the text of the following argv-element, in optarg.
// extern int optind: the index of the next element to be processed in argv. The system initializes this value to 1. The caller can reset it to 1 to restart scanning of the same argv, or when scanning a new argument vector.
// extern int opterr: if an error was detected, and the first character of optstring is not a colon, and the external variable opterr is nonzero
// extern int optopt: if getopt() does not recognize an option character, it prints an error message to stderr, stores the character in optopt, and returns '?'

static int help_flag = 0;
static int verbose = 0;
static int flag = 0;

static char * program_name;
static char * options_str;
 
static void
usage(int exit_code)
{
    fprintf(stderr, "usage: %s [OPTION]... [FILE]...\n", program_name);
    fprintf(stderr, "%s", options_str);
    exit(exit_code);
}

void
use_getopt_long(int argc, char ** argv)
{
    int c;
    //int digit_optind = 0;

    //print_argv(argc, argv);

    while (1) {
        //int this_option_optind = optind ? optind : 1;
        int option_index = 0;
        static struct option long_options[] = {
            {"help",    no_argument,       NULL,        'h' },
            {"verbose", no_argument,       &verbose,     1  },
            {"flag",    required_argument, &flag,       'f' },
            {"create",  required_argument, NULL,        'c' },
            //{"file",    required_argument, NULL,        0 },
            {0,         0,                 NULL,         0 }
        };

        c = getopt_long(argc, argv, "vcf", long_options, &option_index);
        if (c == -1)
            break;

        switch (c) {
            case 0:
                printf("option %s", long_options[option_index].name);
                if (optarg)
                    printf(" with arg %s", optarg);
                printf("\n");
                break;

            case 'f':
                flag = 1;
                break;

            case 'v':
                verbose = 1;
                break;

            case 'h':
                //usage(argv[0]);
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

    //print_argv(argc, argv);

    if (flag) {
        printf("flag\n");
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
}

//void print_argv(int argc, char * argv[])
//{
//    int i;
//    printf("argv\n");
//    for (i = 0; i < argc; i++) {
//        printf("%d: %s\n", i, argv[i]);
//    }
//    printf("\n");
//}


