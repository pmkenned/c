#include <stdlib.h>
#include <stdio.h>
#include "except.h"

const Except_T My_Exception1 = { "My exception! (1)" };
const Except_T My_Exception2 = { "My exception! (2)" };

void edit(int argc, char ** argv) {

    TRY
        fprintf(stderr, "try\n");
        RAISE(My_Exception2);
    EXCEPT(My_Exception1)
        fprintf(stderr, "except\n");
    /*ELSE*/
    /*    fprintf(stderr, "else\n");*/
    FINALLY
        fprintf(stderr, "finally\n");
    END_TRY;
}

int main(int argc, char * argv[]) {
    TRY
        edit(argc, argv);
    ELSE
        fprintf(stderr, "An internal error has occurred from which there is no recovery.\n\n");
        RERAISE;
    END_TRY;
    return EXIT_SUCCESS;
}
