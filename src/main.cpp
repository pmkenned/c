#include <iostream>
#include "str.h"

int main()
{
    char * s = str_create_from("Paul");

    std::cout << "hello, " << s << std::endl;
}
