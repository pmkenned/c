#include "elf.h"
#include "common.h"
#include <stdio.h>

int main(int argc, char * argv[])
{
    if (argc < 2)
        die("usage: %s ELF\n", argv[0]);

    Elf elf = read_elf(argv[1]);
    print_elf(elf, stdout);
    destroy_elf(elf);

    return 0;
}
