#include "fat12.h"
#include <stdio.h>
#include <string.h>
#include <stdint.h>

void print_bytes(uint8_t * ptr, size_t num_bytes, size_t offset) {
    for(size_t i=0; i<num_bytes/16; i++) {
        printf("%08x: ", i*16+offset);
        for(size_t j=0; j<2; j++) {
            for(size_t k=0; k<8; k++) {
                size_t l = i*16+j*8+k;
                printf("%02x ", *(ptr+offset+l));
            }
            printf(" ");
        }
        printf("\n");
    }
}

void * load_file_into_buffer(FILE * fp) {
    uint8_t * buffer;
    fseek(fp, 0, SEEK_END);
    long fsize = ftell(fp);
    rewind(fp);
    buffer = malloc(sizeof(*buffer)*fsize);
    for(long i = 0; i < fsize; i++) {
        fread(buffer+i, 1, 1, fp); 
    }
    return buffer;
}

int main(int argc, char * argv[]) {

    bsect_t bsector;

    init_bsect(&bsector);

    FILE * fp = fopen(argv[1], "rb");

    if(fp == NULL) {
        perror(argv[0]);
        return 1;
    }

    uint8_t * img_contents = load_file_into_buffer(fp);

    //load_sect_from_file(fp, 0, 1, (uint8_t *) &bsector);

    memcpy((void*)&bsector, img_contents, 512);

    //print_bytes((uint8_t *)&bsector, 512, 0);
    print_bytes(img_contents, 512, 512);

    printf("================================\n");
    printf("bytes_per_sector: %d\n", *((uint16_t *) bsector.bytes_per_sector));
    printf("sectors_per_cluster: %d\n", bsector.sectors_per_cluster);
    printf("num_reserved: %d\n", *((uint16_t *) bsector.num_reserved));
    printf("num_fats: %d\n", bsector.num_fats);
    printf("max_root_dir_entries: %d\n", *((uint16_t *) bsector.max_root_dir_entries));
    printf("num_sectors: %d\n", *((uint16_t *) bsector.num_sectors));
    printf("media_desc: %02x\n", bsector.media_desc);
    printf("sectors_per_fat: %d\n", *((uint16_t *) bsector.sectors_per_fat));
    printf("================================\n");

    print_fat(&bsector, img_contents);

    list_files(&bsector, img_contents);

    if(1) {
        unsigned char * file_contents = get_file_contents(&bsector, img_contents, "README.");
        printf("===\n%s\n===\n", file_contents);
        free(file_contents);
    }

    fclose(fp);

    free(img_contents);

    return 0;
}
