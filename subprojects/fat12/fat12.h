#ifndef FAT12_H
#define FAT12_H

#include <stdint.h>

typedef struct bsect
{
    uint8_t jmp[3];
    uint8_t oem_name[8];

    // BIOS parameter block (13 bytes)
    uint8_t bytes_per_sector[2];
    uint8_t sectors_per_cluster;
    uint8_t num_reserved[2];
    uint8_t num_fats;
    uint8_t max_root_dir_entries[2];
    uint8_t num_sectors[2];
    uint8_t media_desc;
    uint8_t sectors_per_fat[2];
    // DOS 3.0 BPB:
    uint8_t sectors_per_track[2];
    uint8_t num_heads[2];
    uint8_t num_hidden_sectors[2];
    // DOS 3.2 BPB:
    uint8_t total_sectors[2];
    // Extended BPB
    uint8_t drive_num;
    uint8_t reserved;
    uint8_t ext_boot_sig;
    uint8_t serial_num[4];
    uint8_t label[11];
    uint8_t fs_type[8];

    // boot code
    uint8_t boot_code[452];

    uint8_t boot_sig[2];
} bsect_t;

void init_bsect(bsect_t * bsector_p);
void print_fat(bsect_t * bsector, uint8_t * img_contents);
void list_files(bsect_t * bsector, uint8_t * img_contents);
unsigned char * get_file_contents(bsect_t * bsector, uint8_t * img_contents, const char * fname);

#endif /* FAT12_H */
