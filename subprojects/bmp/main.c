#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
//#include <signal.h>

static void create_gradient(const char * filename, int w, int h)
{
    FILE * fp;
    int r, c;
    pixel_t px;
    bmp_t * bmp = bmp_create_w_h(w, h);

    for (r=0; r<h; r++) {
        for (c=0; c<w; c++) {
            px = PX_RGB(0x0, c, r);
            bmp_set_px(bmp, r, c, px);
        }
    }

    fp = fopen(filename, "w");
    bmp_write_file(bmp, fp);
    fclose(fp);

    bmp_destroy(bmp);
    bmp = NULL;
}

static void read_write_two()
{
    FILE *fp0, *fp1;
    bmp_t *bmp0, *bmp1;

    create_gradient("bye.bmp", 256, 256);

    bmp0 = bmp_create();
    bmp1 = bmp_create();

    fp0 = fopen("hello.bmp", "r");
    if (fp0 == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    fp1 = fopen("bye.bmp", "r");
    if (fp1 == NULL) {
        perror("error");
        fclose(fp0);
        exit(EXIT_FAILURE);
    }
    bmp_read_file(bmp0, fp0);
    bmp_read_file(bmp1, fp1);
    fclose(fp0);
    fclose(fp1);

    fp0 = fopen("hello2.bmp", "w");
    if (fp0 == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    fp1 = fopen("bye2.bmp", "r");
    if (fp1 == NULL) {
        perror("error");
        fclose(fp0);
        exit(EXIT_FAILURE);
    }
    bmp_write_file(bmp0, fp0);
    bmp_write_file(bmp1, fp1);
    fclose(fp0);
    fclose(fp1);

    bmp_destroy(bmp0);
    bmp_destroy(bmp1);
}

#if 0
void resize_handler(int sig)
{
    int nh, nw;
    getmaxyx(stdscr, nh, nw);
    printf("%d, %d\n", nh, nw);
}
#endif

int main()
{
    //signal(SIGWINCH, resize_handler);

    initscr();

    printw("%d %d\n", LINES, COLS);
    refresh();
    int x = getch();
    printw("%d\n", x);
    getch();
    printw("%d %d\n", LINES, COLS);
    refresh();
    getch();

    endwin();

    return 0;
}
