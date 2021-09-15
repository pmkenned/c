#include "box.h"
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

void
printf_in_box(const char * fmt, ...)
{
    va_list ap, ap_copy;
    va_start(ap, fmt);
    va_copy(ap_copy, ap);
    int i, l = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    printf(BOX_ULC); for (i = 0; i < l; i++) { printf(BOX_HOR); } printf(BOX_URC "\n");
    printf(BOX_VER);
    vprintf(fmt, ap_copy);
    printf(BOX_VER "\n");
    printf(BOX_LLC); for (i = 0; i < l; i++) { printf(BOX_HOR); } printf(BOX_LRC "\n");
    va_end(ap_copy);
}

void
printf_in_dbox(const char * fmt, ...)
{
    va_list ap, ap_copy;
    va_start(ap, fmt);
    va_copy(ap_copy, ap);
    int i, l = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);
    printf(BOX_DULC); for (i = 0; i < l; i++) { printf(BOX_DHOR); } printf(BOX_DURC "\n");
    printf(BOX_DVER);
    vprintf(fmt, ap_copy);
    printf(BOX_DVER "\n");
    printf(BOX_DLLC); for (i = 0; i < l; i++) { printf(BOX_DHOR); } printf(BOX_DLRC "\n");
    va_end(ap_copy);
}
