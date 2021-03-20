#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdio.h>

#define PX_RW 8
#define PX_GW 8
#define PX_BW 8

#define PX_R_LSB 16
#define PX_G_LSB 8
#define PX_B_LSB 0

#define PX_R_MASK 0x00ff0000
#define PX_G_MASK 0x0000ff00
#define PX_B_MASK 0x000000ff

#define PX_R_IMASK ~PX_R_MASK 
#define PX_G_IMASK ~PX_G_MASK 
#define PX_B_IMASK ~PX_B_MASK 

#define PX_GET_RED(px)      (((px) >> PX_R_LSB) & ((1 << PX_RW)-1))
#define PX_GET_GREEN(px)    (((px) >> PX_G_LSB) & ((1 << PX_GW)-1))
#define PX_GET_BLUE(px)     (((px) >> PX_B_LSB) & ((1 << PX_BW)-1))

#define PX_SET_RED(px, r)   (((px) & PX_R_IMASK) | (((r) & ((1 << PX_RW)-1)) << PX_R_LSB))
#define PX_SET_GREEN(px, g) (((px) & PX_G_IMASK) | (((g) & ((1 << PX_GW)-1)) << PX_G_LSB))
#define PX_SET_BLUE(px, b)  (((px) & PX_B_IMASK) | (((b) & ((1 << PX_BW)-1)) << PX_B_LSB))

#define PX_RGB(r, g, b) ((((r) & ((1 << PX_RW)-1)) << PX_R_LSB) | (((g) & ((1 << PX_GW)-1)) << PX_G_LSB) | (((b) & ((1 << PX_BW)-1)) << PX_B_LSB))

struct bmp_t;
typedef struct bmp_t bmp_t;
typedef uint32_t pixel_t ;

extern bmp_t * bmp_create();
extern bmp_t * bmp_create_w_h(int w, int h);
extern bmp_t * bmp_create_copy(bmp_t * bmp);
extern void    bmp_destroy(bmp_t * bmp);

extern pixel_t bmp_get_px(const bmp_t * bmp, int r, int c);
extern int     bmp_get_w(const bmp_t * bmp);
extern int     bmp_get_h(const bmp_t * bmp);

extern void    bmp_set_px(bmp_t * bmp, int r, int c, pixel_t px);
extern void    bmp_set_w(bmp_t * bmp, int w);
extern void    bmp_set_h(bmp_t * bmp, int h);
extern void    bmp_set_w_h(bmp_t * bmp, int w, int h);

extern void    bmp_read_file(bmp_t * bmp, FILE * fp);
extern void    bmp_write_file(const bmp_t * bmp, FILE * fp);

#endif /* BMP_H */
