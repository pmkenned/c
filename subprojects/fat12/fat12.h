#ifndef FAT12_H
#define FAT12_H

#include "common.h"

typedef struct {
    u8 jmp[3];
    u8 oem_name[8];

    // BIOS parameter block (13 bytes)
    u8 bytes_per_sector[2];
    u8 sectors_per_cluster;
    u8 num_reserved[2];
    u8 num_fats;
    u8 max_root_dir_entries[2];
    u8 num_sectors[2];
    u8 media_desc;
    u8 sectors_per_fat[2];
    // DOS 3.0 BPB:
    u8 sectors_per_track[2];
    u8 num_heads[2];
    u8 num_hidden_sectors[2];
    // DOS 3.2 BPB:
    u8 total_sectors[2];
    // Extended BPB
    u8 drive_num;
    u8 reserved;
    u8 ext_boot_sig;
    u8 serial_num[4];
    u8 label[11];
    u8 fs_type[8];

    // boot code
    u8 boot_code[452];

    u8 boot_sig[2];
} BootSector;

void    fat12_print_fat             (BootSector * bsector, u8 * img_contents);
void    fat12_list_files            (BootSector * bsector, u8 * img_contents);
Buffer  fat12_get_file_contents     (BootSector * bsector, u8 * img_contents, const char * fname);
//void    fat12_load_sect_from_file   (FILE * fp, size_t sect_num, size_t num_sectors, void * dst);

#endif /* FAT12_H */
