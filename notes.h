#if NOTES

typedef enum {
    MST_INT,
    MST_FLOAT,
    MST_COUNT
} MyStructType;

typedef struct {
    char  name[32];
    MyStructType type;
} MyHeader;

typedef struct {
    MyHeader head;
    int value;
} MyStructInt;

typedef struct {
    MyHeader head;
    float vlaue;
} MyStructFloat;

void my_set_value(MyHeader * h, double value)
{
    if (h->type == MST_INT)
        ((MyStructInt *)h)->value = (int) value;
    else if (h->type == MST_FLOAT)
        ((MyStructFloat *)h)->value = (float) value;
}

// https://www.youtube.com/watch?v=443UNeGrFoM
// https://www.youtube.com/watch?v=JbHmin2Wtmc
// https://www.youtube.com/watch?v=M6RCUiZzl8Y
// https://stackoverflow.com/questions/1079832/how-can-i-configure-my-makefile-for-debug-and-release-builds
// https://www.linuxjournal.com/magazine/vim-c-programmers
// https://vim.fandom.com/wiki/Errorformat_and_makeprg
// https://pseudomuto.com/2013/05/implementing-a-generic-linked-list-in-c/
// https://www.icosaedro.it/c-modules.html
// https://www.cs.princeton.edu/courses/archive/spring03/cs217/lectures/Modules.pdf
// https://www.cs.bu.edu/teaching/c/separate-compilation/
// https://stackoverflow.com/questions/385975/error-handling-in-c-code
// https://dept-info.labri.fr/~strandh/Teaching/PFS/Common/Strandh-Tutorial/uniform-reference-semantics.html
// https://stackoverflow.com/questions/26205329/interface-implementation-in-ansi-c
// https://stackoverflow.com/questions/20735026/making-c-dynamic-array-generic

#endif
