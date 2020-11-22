#include <stdio.h>
#include <str.h>

int main()
{
    char * s = str_create_from("Paul");

    printf("hello, %s\n", s);

    return 0;
}
