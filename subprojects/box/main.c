#include "box.h"

int main(int argc, char * argv[])
{
    fprintf_in_box(stdout, 2, "this program: %s", argv[0]);
    return 0;
}
