#include <stdio.h>
#include <stddef.h>
#include <stdint.h>

typedef struct 
#if 0
__attribute__((packed))
#endif
{
    char d;
    int x;
    long y;
    char c;
} foo_t ;

int main()
{
    size_t i;
    foo_t f;
    printf("size: %zu alignment: %zu\n", sizeof(foo_t), _Alignof(foo_t));
    printf("f.y: size: %zu alignment: %zu\n", sizeof(f.y), _Alignof(f.y));
    printf("f.x: size: %zu alignment: %zu\n", sizeof(f.x), _Alignof(f.x));
    printf("f.d: size: %zu alignment: %zu\n", sizeof(f.d), _Alignof(f.d));
    printf("f.c: size: %zu alignment: %zu\n", sizeof(f.c), _Alignof(f.c));

    ptrdiff_t pd = (intptr_t)&f.c - (intptr_t)&f.y;
    printf("f.c: size: %td\n", pd);

#if 0
    for (i = 0; i < sizeof(char); i++)
        printf("d");
    for (i = 0; i < sizeof(int)-sizeof(char); i++)
        printf("-");
    for (i = 0; i < sizeof(int); i++)
        printf("x");
    for (i = 0; i < sizeof(long); i++)
        printf("y");
    for (i = 0; i < sizeof(char); i++)
        printf("c");
    for (i = 0; i < sizeof(foo_t)-(sizeof(int)*2+sizeof(long)+sizeof(char)); i++)
        printf("-");
    printf("\n");
#endif

}
