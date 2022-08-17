// https://resources.sei.cmu.edu/asset_files/Presentation/2016_017_101_484207.pdf
// https://wiki.sei.cmu.edu/confluence/display/c/ERR02-C.+Avoid+in-band+error+indicators
// https://www.kernel.org/doc/html/v4.10/process/coding-style.html#function-return-values-and-names
// https://www.reddit.com/r/C_Programming/comments/waknaz/the_different_ways_to_handle_errors_in_c/
// https://www.reddit.com/r/programming/comments/w9vsar/the_different_ways_to_handle_errors_in_c/
// https://mccue.dev/pages/7-27-22-c-errors

// * return code: in-band error code
//   * NULL
//   * negative
// * setjmp/longjmp
// * function pointer for error handler
// * struct with valid boolean / error enum
// * wrapper function that exits on error (e.g. xmalloc)
// * global error variable (e.r. errno)
// * exceptions
// * goto
// * strtol

#include <stdio.h>

int main()
{
    return 0;
}
