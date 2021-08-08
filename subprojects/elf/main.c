#include "elf.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
    if (argc < 2) {
        fprintf(stderr, "usage: %d [ELF]\n", argv[0]);
        return 1;
    }

    elf_t my_elf = read_elf(argv[1]);

    return 0;
}
