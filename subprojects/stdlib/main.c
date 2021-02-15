#include <assert.h>      /*          Conditionally compiled macro that compares its argument to zero */
#include <ctype.h>       /*          Functions to determine the type contained in character data */
#include <errno.h>       /*          Macros reporting error conditions */
#include <float.h>       /*          Limits of float types */
#include <limits.h>      /*          Sizes of basic types */
#include <locale.h>      /*          Localization utilities */
#include <math.h>        /*          Common mathematics functions */
#include <setjmp.h>      /*          Nonlocal jumps */
#include <signal.h>      /*          Signal handling */
#include <stdarg.h>      /*          Variable arguments */
#include <stddef.h>      /*          Common macro definitions */
#include <stdio.h>       /*          Input/output */
#include <stdlib.h>      /*          misc: mem management, utilities, str conversions, rand numbers, algo */
#include <string.h>      /*          String handling */
#include <time.h>        /*          Time/date utilities */

/* C95 */
#include <iso646.h>      /* (C95)    Alternative operator spellings */
#include <wchar.h>       /* (C95)    Extended multibyte and wide character utilities */
#include <wctype.h>      /* (C95)    Functions to determine the type contained in wide character data */

/* C99 */
#include <complex.h>     /* (C99)    Complex number arithmetic */
#include <fenv.h>        /* (C99)    Floating-point environment */
#include <inttypes.h>    /* (C99)    Format conversion of integer types */
#include <stdbool.h>     /* (C99)    Macros for boolean type */
#include <stdint.h>      /* (C99)    Fixed-width integer types */
#include <tgmath.h>      /* (C99)    Type-generic math (macros wrapping math.h and complex.h) */

/* C11 */
#include <stdalign.h>    /* (C11)    alignas and alignof convenience macros */
#include <stdatomic.h>   /* (C11)    Atomic types */
#include <stdnoreturn.h> /* (C11)    noreturn convenience macros */
#include <threads.h>     /* (C11)    Thread library */
#include <uchar.h>       /* (C11)    UTF-16 and UTF-32 character utilities */

void assert_function()
{
}

void ctype_function()
{
#if 0
    isalnum();
    isalpha();
    iscntrl();
    isdigit();
    isgraph();
    islower();
    isprint();
    ispunct();
    isspace();
    isupper();
    isxdigit();
#endif
}

void errno_function()
{
}

void float_function()
{
}

void limits_function()
{
}

void locale_function()
{
}

void math_function()
{
}

void setjmp_function()
{
}

void signal_function()
{
}

void stdarg_function()
{
}

void stddef_function()
{
}

void stdio_function()
{
#if 0
    /* file operations */
    fopen();
    freopen();
    fflush();
    fclose();
    remove();
    rename();
    tmpfile();
    tmpnam();
    setvbuf();
    setbuf();
    /* formatted output */
    fprintf();
    printf();
    sprintf();
    vprintf();
    vfprintf();
    vsprintf();
    /* formatted input */
    fscanf();
    scanf();
    sscanf();
    /* character input and output functions */
    fgetc();
    fgets();
    fputc();
    fputs();
    getc();
    getchar();
    gets();
    putc();
    putchar();
    puts();
    ungetc();
    /* direct input and output functions */
    fread();
    fwrite();
    /* file positioning functions */
    fseek();
    ftell();
    rewind();
    fgetpos();
    fsetpos();
    /* error functions */
    clearerr();
    feof();
    ferror();
    perror();
#endif
}

void stdlib_function()
{
#if 0
#endif
}

void string_function()
{
#if 0
    /* */
#endif
}

void time_function()
{
#if 0
#endif
}

/* C95 */

void iso646_function()
{
}

void wchar_function()
{
}

void wctype_function()
{
}

/* C99 */

void complex_function()
{
}

void fenv_function()
{
}

void inttypes_function()
{
}

void stdbool_function()
{
}

void stdint_function()
{
}

void tgmath_function()
{
}

/* C11 */

void stdalign_function()
{
}

void stdatomic_function()
{
}

void stdnoreturn_function()
{
}

void threads_function()
{
}

void uchar_function()
{
}

int main(int argc, char * argv[])
{
    assert_function();
    ctype_function();
    errno_function();
    float_function();
    limits_function();
    locale_function();
    math_function();
    setjmp_function();
    signal_function();
    stdarg_function();
    stddef_function();
    stdio_function();
    stdlib_function();
    string_function();
    time_function();
    /* C95 */
    iso646_function();
    wchar_function();
    wctype_function();
    /* C99 */
    complex_function();
    fenv_function();
    inttypes_function();
    stdbool_function();
    stdint_function();
    tgmath_function();
    /* C11 */
    stdalign_function();
    stdatomic_function();
    stdnoreturn_function();
    threads_function();
    uchar_function();
    return 0;
}
