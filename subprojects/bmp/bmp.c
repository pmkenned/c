/* TODO
 * - use dynamic allocation instead of a static buffer
 * - debug
 */

/* ==== includes ==== */

#include "bmp.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>

/* ==== macros ==== */

#define KB(n) ((n) * (1<<10))
#define MB(n) ((n) * (1<<20))

/* ==== types ==== */

struct bmp_t {
    int width;
    int height;
    pixel_t * px_data;
    /* TODO: color depth */
};

enum {
    FILE_HEADER_SIZE = 14,
    DIB_HEADER_SIZE = 40,
    BITS_PER_PIXEL = 24
};

enum { BUFFER_SIZE = MB(1) };

/* ==== data ==== */

static struct {

    struct {
        uint16_t signature;
        uint32_t size;
        uint32_t px_offset;
    } file_header;

    struct {
        uint32_t header_size;
        uint32_t width;
        uint32_t height;
        uint16_t bits_per_px;
    } dib_header;

    uint8_t * px_array;

} bmp_file = {0};

static uint8_t buffer[BUFFER_SIZE];

/* ==== functions ==== */

/* private functions */

static uint32_t unpack_uint16_little_endian(const uint8_t * p) {
    uint32_t x = 0;
    size_t i;
    for (i=0; i<2; i++)
        x += (p[i] << 8*i);
    return x;
}

static uint32_t unpack_uint24_little_endian(const uint8_t * p) {
    uint32_t x = 0;
    size_t i;
    for (i=0; i<3; i++)
        x += (p[i] << 8*i);
    return x;
}

static void pack_uint24_little_endian(uint8_t * p, uint32_t x) {
    size_t i;
    for (i=0; i<3; i++)
        p[i] = (x >> 8*i) & 0xff;
}

static uint32_t unpack_uint32_little_endian(const uint8_t * p) {
    uint32_t x = 0;
    size_t i;
    for (i=0; i<4; i++)
        x += (p[i] << 8*i);
    return x;
}

#if 0
static void print_bytes(const void * p, size_t n) {
    size_t i;
    for (i=0; i<n; i++) {
        printf("%02hhx ", ((uint8_t *) p)[i]);
        if ((i+1) % 16 == 0)
            printf("\n");
    }
    printf("\n");
}
#endif

static int get_row_size(int bits_per_px, int width) {
    return 4*((bits_per_px * width + 31)/32);
}

static void bmp_px_data_to_px_array(const bmp_t * bmp) {
    size_t r, c;
    size_t num_rows = bmp->height;
    size_t num_cols = bmp->width;
    size_t row_size = get_row_size(BITS_PER_PIXEL, bmp->width);
    bmp_file.px_array = buffer; /* TODO: think about this */
    for (r=0; r<num_rows; r++) {
        for (c=0; c<num_cols; c++) {
            pixel_t px = bmp_get_px(bmp, r, c);
            pack_uint24_little_endian(bmp_file.px_array + (num_rows-r-1)*row_size + 3*c, px);
        }
    }
}

static void px_array_to_bmp_px_data(bmp_t * bmp) {
    size_t r, c;
    size_t num_rows = bmp_file.dib_header.height;
    size_t num_cols = bmp_file.dib_header.width;
    size_t row_size = get_row_size(BITS_PER_PIXEL, num_cols);
    for (r=0; r<num_rows; r++) {
        for (c=0; c<num_cols; c++) {
            pixel_t px = unpack_uint24_little_endian(bmp_file.px_array + (num_rows-r-1)*row_size + 3*c);
            bmp_set_px(bmp, r, c, px);
        }
    }
}

/* public interface */

bmp_t * bmp_create() {
    return calloc(1, sizeof(bmp_t));
}

bmp_t * bmp_create_w_h(int w, int h) {
    bmp_t * bmp_new = calloc(1, sizeof(*bmp_new));
    bmp_new->width = w;
    bmp_new->height = h;
    bmp_new->px_data = calloc(w*h, sizeof(*bmp_new->px_data));
    return bmp_new;
}

void bmp_destroy(bmp_t * bmp) {
    free(bmp->px_data);
    free(bmp);
}

pixel_t bmp_get_px(const bmp_t * bmp, int r, int c) {
    assert(c < bmp->width);
    assert(r < bmp->height);
    return bmp->px_data[r*bmp->width + c];
}

int bmp_get_w(const bmp_t * bmp) {
    return bmp->width;
}

int bmp_get_h(const bmp_t * bmp) {
    return bmp->height;
}
        
void bmp_set_px(bmp_t * bmp, int r, int c, pixel_t px) {
    assert(c < bmp->width);
    assert(r < bmp->height);
    bmp->px_data[r*bmp->width + c] = px;
}

void bmp_set_w_h(bmp_t * bmp, int w, int h) {
    assert(w*h > 0);
    bmp->width = w;
    bmp->height = h;
    bmp->px_data = realloc(bmp->px_data, sizeof(bmp->px_data)*bmp->width*bmp->height);
}

void bmp_set_w(bmp_t * bmp, int w) {
    assert(w > 0);
    bmp->width = w;
    bmp->px_data = realloc(bmp->px_data, sizeof(bmp->px_data)*bmp->width*bmp->height);
}

void bmp_set_h(bmp_t * bmp, int h) {
    assert(h > 0);
    bmp->height = h;
    bmp->px_data = realloc(bmp->px_data, sizeof(bmp->px_data)*bmp->width*bmp->height);
}

void bmp_read_file(bmp_t * bmp, FILE * fp) {
    size_t n = fread(buffer, 1, sizeof(buffer), fp);
    assert(n < sizeof(buffer));
    assert(memcmp(buffer, "BM", 2) == 0);

    bmp_file.file_header.signature  = unpack_uint16_little_endian(buffer+0);
    bmp_file.file_header.size       = unpack_uint32_little_endian(buffer+2);
    bmp_file.file_header.px_offset  = unpack_uint32_little_endian(buffer+10);

    bmp_file.dib_header.header_size = unpack_uint32_little_endian(buffer+14);
    assert(bmp_file.dib_header.header_size == DIB_HEADER_SIZE);
    bmp_file.dib_header.width       = unpack_uint32_little_endian(buffer+18);
    bmp_file.dib_header.height      = unpack_uint32_little_endian(buffer+22);
    bmp_file.dib_header.bits_per_px = unpack_uint16_little_endian(buffer+28);
    assert(bmp_file.dib_header.bits_per_px == BITS_PER_PIXEL);

    bmp_file.px_array = buffer+bmp_file.file_header.px_offset;

    bmp_set_w_h(bmp, bmp_file.dib_header.width, bmp_file.dib_header.height);

    px_array_to_bmp_px_data(bmp);
}

void bmp_write_file(const bmp_t * bmp, FILE * fp) {

    /* TODO: confirm expected number of bytes written to file */

    char     signature[2]   = "BM";
    uint16_t RESERVED_16    = 0;
    uint32_t fsize          = 0; /* TODO */
    uint32_t px_offset      = FILE_HEADER_SIZE + DIB_HEADER_SIZE;

    uint32_t dib_size       = DIB_HEADER_SIZE;
    uint32_t width          = bmp->width;
    uint32_t height         = bmp->height;
    uint16_t color_planes   = 1;
    uint16_t bits_per_px    = BITS_PER_PIXEL;
    uint32_t compression    = 0;
    uint32_t img_size       = 0; /* TODO (optional) */
    int32_t  hor_px_per_m   = 0; /* TODO? */
    int32_t  ver_px_per_m   = 0; /* TODO? */
    uint32_t num_colors     = 0;
    uint32_t important      = 0;

    size_t row_size = get_row_size(bits_per_px, bmp->width);
    size_t num_rows = bmp->height;

    /* file header */

    fwrite(&signature,      1, sizeof(signature),       fp);
    fwrite(&fsize,          1, sizeof(fsize),           fp);
    fwrite(&RESERVED_16,    1, sizeof(RESERVED_16),     fp);
    fwrite(&RESERVED_16,    1, sizeof(RESERVED_16),     fp);
    fwrite(&px_offset,      1, sizeof(px_offset),       fp);

    /* dib header */

    fwrite(&dib_size,       1, sizeof(dib_size),        fp);
    fwrite(&width,          1, sizeof(width),           fp);
    fwrite(&height,         1, sizeof(height),          fp);
    fwrite(&color_planes,   1, sizeof(color_planes),    fp);
    fwrite(&bits_per_px,    1, sizeof(bits_per_px),     fp);
    fwrite(&compression,    1, sizeof(compression),     fp);
    fwrite(&img_size,       1, sizeof(img_size),        fp);
    fwrite(&hor_px_per_m,   1, sizeof(hor_px_per_m),    fp);
    fwrite(&ver_px_per_m,   1, sizeof(ver_px_per_m),    fp);
    fwrite(&num_colors,     1, sizeof(num_colors),      fp);
    fwrite(&important,      1, sizeof(important),       fp);

    /* px array */
    bmp_px_data_to_px_array(bmp);
    fwrite(bmp_file.px_array, 1, row_size*num_rows, fp);
}
