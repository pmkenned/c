#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char num_char_arr[] = "512";                // .data
static const char num_const_char_arr[] = "123";    // .rodata
static const char * num_const_char_star = "789";

static int g(int n)
{
    if (n > 0)
        return n+g(n-1);
    else
        return 0;
}

static int f(const char * s)
{
    return atoi(s);
}

int main(int argc, char * argv[])
{
    const char * num_argv = (argc > 1) ? argv[1] : "0";

    char * num_malloc = malloc(sizeof(*num_malloc)*10);
    strncpy(num_malloc, "1701", 10);

    int x;

    x = f(num_char_arr);
    printf("%d\n", x);

    x = f(num_const_char_arr);
    printf("%d\n", x);

    x = f(num_const_char_star);
    printf("%d\n", x);

    x = f("456");
    printf("%d\n", x);

    x = f(num_argv);
    printf("%d\n", x);

    x = f(num_malloc);
    printf("%d\n", x);

    x = g(10000);
    printf("%d\n", x);

    return 0;
}
