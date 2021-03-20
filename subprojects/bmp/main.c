#define _POSIX_C_SOURCE 199309L
#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <assert.h>
#include <time.h>
#include <math.h>

#define ASCII_PX_RATIO 1.71

#define THOUSAND(n) n ## 000L
#define MILLION(n) n ## 000000L

static void
to_grayscale(bmp_t * bmp)
{
    int r, c;
    for (r = 0; r < bmp_get_h(bmp); r++) {
        for (c = 0; c < bmp_get_w(bmp); c++) {
            pixel_t px = bmp_get_px(bmp, r, c);
            int red = PX_GET_RED(px);
            int green = PX_GET_GREEN(px);
            int blue = PX_GET_BLUE(px);
            /*int avg = (red + green + blue) / 3.0;*/
            int avg = 0.21*red + 0.72*green + 0.07*blue;
            px = PX_RGB(avg, avg, avg);
            bmp_set_px(bmp, r, c, px);
        }
    }
}

static void
create_gradient(const char * filename, int w, int h)
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

static void
read_write_two()
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

static FILE *
Fopen(const char * filename, const char * mode)
{
    FILE *fp = fopen(filename, mode);
    if (fp == NULL) {
        perror("error");
        exit(EXIT_FAILURE);
    }
    return fp;
}

#define ASCII_SHORT "@%#*+=-:. "
#define ASCII_LONG "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. "

const char ascii_grayscale[] = ASCII_SHORT;
size_t ascii_grayscale_n = sizeof(ascii_grayscale)-1;

static struct timespec
gettime()
{
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime()");
        exit(EXIT_FAILURE);
    }
    return ts;
}

static long long int
diff_ns(struct timespec end, struct timespec begin)
{
    long long int sec = end.tv_sec - begin.tv_sec;
    long long int nsec = end.tv_nsec - begin.tv_nsec;
    return sec*1000000000 + nsec;
}

static int
color_to_ascii_idx(int color)
{
#if 0
    if (color >= 0 && color < 25)
        return 0;
    else if (color >= 25 && color < 50)
        return 1;
    else if (color >= 50 && color < 75)
        return 2;
    else if (color >= 75 && color < 100)
        return 3;
    else if (color >= 100 && color < 125)
        return 4;
    else if (color >= 125 && color < 150)
        return 5;
    else if (color >= 150 && color < 175)
        return 6;
    else if (color >= 175 && color < 200)
        return 7;
    else if (color >= 200 && color < 225)
        return 8;
    else if (color >= 225 && color < 256)
        return 9;
#else
    size_t i;
    for (i = 0; i < ascii_grayscale_n; i++) {
        if ((color >= (1.0*i*256/ascii_grayscale_n)) && (color < 1.0*(i+1)*256/ascii_grayscale_n))
            return ascii_grayscale_n-i-1;
    }
#endif
    assert(0);
    return 0;
}

float
calc_scale_factor(int w, int h)
{
    float hr = (LINES*ASCII_PX_RATIO)/h;
    float wr = 1.0*COLS/w;
    float scale_factor = (hr < wr) ? hr : wr;
    return scale_factor;
}

int
main()
{
    int done = 0;

    bmp_t *bmp = bmp_create();
    FILE *fp = Fopen("wolf3d.bmp", "r");
    bmp_read_file(bmp, fp);
    fclose(fp);

    to_grayscale(bmp);

    int w = bmp_get_w(bmp);
    int h = bmp_get_h(bmp);

    /* clear image */
    //{
    //    int r, c;
    //    for (r = 0; r < h; r++) {
    //        for (c = 0; c < w; c++) {
    //            bmp_set_px(bmp, r, c, PX_RGB(0, 0, 0));
    //        }
    //    }
    //}

    initscr();
    curs_set(0);

    float scale_factor = calc_scale_factor(w, h);

#if 0
    printf("h: %d\n\r", h);
    printf("LINES: %d (%d)\n\r", LINES, (int)(LINES*ASCII_PX_RATIO));
    printf("w: %d\n\r", w);
    printf("COLS: %d\n\r", COLS);
    printf("hr: %f\n\r", hr);
    printf("wr: %f\n\r", wr);
    printf("scale_factor: %f\n\r", scale_factor);
    printf("%d %d\r\n", (int)((LINES-1)*ASCII_PX_RATIO/scale_factor), (int)((COLS-1)/scale_factor));
    getchar();
    endwin();
    return 0;
#endif

    timeout(0);

    int sq_w = 30;
    int sq_h = 35;
    int sq_x = 100;
    int sq_y = 100;

    struct timespec prev_ts = gettime();
    long long int dt_ns;
    long long int sleep_dur;

    int frame_num = 0;

    while (!done) {
        frame_num++;

        int r, c;
        if (frame_num % 30 == 0) {
            int x = getch();

            if (x == -1) {
            } else if (x == KEY_RESIZE) {
                scale_factor = calc_scale_factor(w, h);
                mvprintw(0, 0, "resize: %d %d      \n", LINES, COLS);
            } else if (x == 'q') {
                done = 1;
            } else {
                /*mvprintw(0, 0, "%d    \n", x);*/
            }
        }

        /* render */
        //{
        //    for (r = 0; r < h; r++) {
        //        for (c = 0; c < w; c++) {
        //            bmp_set_px(bmp, r, c, PX_RGB(0, 0, 0));
        //            //bmp_set_px(bmp, r, c, 0);
        //        }
        //    }
        //    for (r = sq_y; r < sq_y + sq_h; r++) {
        //        for (c = sq_x; c < sq_x + sq_w; c++) {
        //            bmp_set_px(bmp, r, c, PX_RGB(255, 255, 255));
        //        }
        //    }
        //}

        /* update */
        {
            sq_x = 100 + 100*sin(frame_num/10.0);
            sq_y = 100 + 100*cos(frame_num/10.0);
        }

        for (r = 0; r < LINES; r++) {
            for (c = 0; c < COLS; c++) {
                pixel_t px = bmp_get_px(bmp, r*ASCII_PX_RATIO/scale_factor, c/scale_factor);
                int idx = color_to_ascii_idx(PX_GET_RED(px));
                mvprintw(r, c, "%c", ascii_grayscale[idx]);
            }
        }


        struct timespec curr_ts = gettime();
        dt_ns = diff_ns(curr_ts, prev_ts);
        prev_ts = curr_ts;
        mvprintw(0, 0, "dt_ms: %ld  ", (long) (dt_ns/1000000.0));
        mvprintw(1, 0, "sleep_dur: %ld  ", (long) (sleep_dur/1000000.0));
        refresh();

        int dropped = (dt_ns > THOUSAND(33300))? 1 : 0;
        sleep_dur = dropped ? 0 : THOUSAND(33300) - dt_ns;

        struct timespec req, rem;
        req.tv_sec  = 0;
        req.tv_nsec = sleep_dur;
        nanosleep(&req, &rem);

    }

    endwin();

    printf("dt_ms: %ld\n", (long) (dt_ns/1000000.0));

    return 0;
}
