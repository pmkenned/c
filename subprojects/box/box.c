#include "box.h"
#include <stdarg.h>
#include <assert.h>

#define DO_N(X,N) for (int i = 0; i < N; i++) { X; }

enum { BOX_HOR, BOX_VER, BOX_T, BOX_CCT, BOX_CT, BOX_UPT, BOX_CROSS, BOX_LLC, BOX_LRC, BOX_ULC, BOX_URC, BOX_NUM };

char * table[][BOX_NUM] = {
    { "\u2500", "\u2502", "\u252c", "\u251c", "\u2524", "\u2534", "\u253c", "\u2514", "\u2518", "\u250c", "\u2510", },
    { "\u2550", "\u2551", "\u2566", "\u2560", "\u2563", "\u2569", "\u256c", "\u255a", "\u255d", "\u2554", "\u2557", }
};

void
fprintf_in_box(FILE * fp, int border, const char * fmt, ...)
{
    assert(border == 1 || border == 2);
    border--;

    // how wide should the box be?
    va_list ap;
    va_start(ap, fmt);
    int l = vsnprintf(NULL, 0, fmt, ap);
    va_end(ap);

    // top border
    fputs(table[border][BOX_ULC], fp);
    DO_N(fputs(table[border][BOX_HOR], fp), l)
    fputs(table[border][BOX_URC], fp);
    fputs("\n", fp);

    // print formatted content
    fputs(table[border][BOX_VER], fp);
    va_start(ap, fmt);
    vfprintf(fp, fmt, ap);
    va_end(ap);
    fputs(table[border][BOX_VER], fp);
    fputs("\n", fp);

    // bottom border
    fputs(table[border][BOX_LLC], fp);
    DO_N(fputs(table[border][BOX_HOR], fp), l)
    fputs(table[border][BOX_LRC], fp);
    fputs("\n", fp);
}
