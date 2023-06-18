#include "fat12.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

// TODO:
// * Follow cluster chain for files
// * Support subdirectories
// * Support file attributes
// * Support time and date info
// * Clean up functions for dealing with files
// * Support creating output files from image
// * Make sure types are correct (e.g. char vs s8 vs u8)

//assert(sizeof(BootSector) == 512);
//assert(sizeof(DirEntry) == 32);

typedef struct {
    u8 filename[8]; // if byte 0 == 0x2e, this entry is either "." or ".."
    u8 ext[3];
    u8 attrs; // 0x10: this entry describes a subdirectory
    u8 reserved[10];
    u8 tmodified[2];
    u8 dmodified[2];
    u8 start_cluster[2];
    u8 file_size[4];
} DirEntry;

#if 0
void
fat12_load_sect_from_file(FILE * fp, size_t sect_num, size_t num_sectors, void * dst)
{
    fseek(fp, 512*sect_num, SEEK_SET);
    fread(dst, 1, num_sectors*512, fp); 
}
#endif

static u16
get_fat12_entry(u8 * fat, size_t idx)
{
    size_t offset = idx*3/2;
    u8 byte0 = fat[offset];
    u8 byte1 = fat[offset+1];
    u16 rv=0;
    if (idx % 2 == 0) {
        rv = ((byte1&0xf) << 8) | byte0;
    } else {
        rv = (byte1 << 4) | ((byte0 >> 4) & 0xf);
    }
    return rv;
}

static DirEntry
get_dir_entry(u8 * root_dir, size_t idx)
{
    DirEntry entry;

    memcpy((void *)&entry, (void*)root_dir+idx*sizeof(DirEntry), sizeof(DirEntry));

    return entry;
}

static void
get_filename(DirEntry * entry, char * dst)
{
    size_t i;
    for (i=0; i<13; i++) {
        dst[i] = '\0';
    }
    size_t fn_len;
    for (i=0; i<8; i++) {
        u8 c = entry->filename[i];
        if (c == ' ') {
            break;
        }
        dst[i] = c;
    }
    fn_len = i;
    dst[fn_len] = '.';
    for (i=0; i<3; i++) {
        u8 c = entry->ext[i];
        if (c == ' ') {
            break;
        }
        dst[fn_len+1+i] = c;
    }
}

void
fat12_print_fat(BootSector * bsector, u8 * img_contents)
{
    size_t bps = *((u16 *) bsector->bytes_per_sector);
    size_t spf = *((u16 *) bsector->sectors_per_fat);
    u8 * fat = img_contents + bps;

    for (size_t i=0; i<spf*(bps/(16*3)); i++) {
        for (size_t j=0; j<32; j++) {
            u16 fat_entry = get_fat12_entry(fat, i*32+j);
            printf("%03x", fat_entry);
            if (fat_entry == 0xfff) {
                printf(";");
            } else {
                printf(" ");
            }
        }
        printf("\n");
    }
}

void
fat12_list_files(BootSector * bsector, u8 * img_contents)
{
    //size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((u16 *) bsector->bytes_per_sector);
    size_t spf = *((u16 *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((u16 *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    //size_t root_dir_size = sizeof(DirEntry) * max_root_dir_entries;
    //printf("root_dir_offset: %d\n", root_dir_offset);
    //printf("size of root dir: %d\n", root_dir_size);

    u8 * root_dir = img_contents + root_dir_offset;

    for (size_t i=0; i<max_root_dir_entries; i++) {
        DirEntry entry = get_dir_entry(root_dir, i);
        u8 filename[13]; // 8 + '.' + 3 + '\0'
        u8 fsize_str[10];
        u8 start_cluster_str[10];
        get_filename(&entry, (char *) filename);

        if (filename[0] == '\0') { // no more files in this directory
            break;
        } else if (filename[0] == 0x2e) { // either "." or ".."
        } else if (filename[0] == 0xe5) { // available
            continue;
        }

        u32 fsize = *((u32 *) entry.file_size);
        sprintf((char *) fsize_str, "%u", fsize);
        u16 start_cluster = *((u16 *) entry.start_cluster);
        sprintf((char *) start_cluster_str, "%u", start_cluster);
        int fn_len = strlen((char *) filename);
        int fsize_len = strlen((char *) fsize_str);
        if (fn_len > 0) {
            printf("%s", filename);
            for (int j=0; j<15-fn_len; j++) {
                printf(" ");
            }
            printf("%s", fsize_str);
            for (int j=0; j<15-fsize_len; j++) {
                printf(" ");
            }
            printf("%s\n", start_cluster_str);
        }
    }
}

#if 0
static u8 *
get_file_ptr(BootSector * bsector, u8 * img_contents, DirEntry * entry)
{
    size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((u16 *) bsector->bytes_per_sector);
    size_t spf = *((u16 *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((u16 *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    size_t root_dir_size = sizeof(DirEntry) * max_root_dir_entries;

    u16 start_cluster = *((u16 *) entry->start_cluster);

    size_t file_offset = (start_cluster-2)*spc*bps + root_dir_offset + root_dir_size;
    u8 * ptr = (u8 *)(img_contents + file_offset);
    return ptr;
}
#endif

static DirEntry
get_file_entry(BootSector * bsector, u8 * img_contents, const char * fname)
{
    //size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((u16 *) bsector->bytes_per_sector);
    size_t spf = *((u16 *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((u16 *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    //size_t root_dir_size = sizeof(DirEntry) * max_root_dir_entries;
    u8 * root_dir = img_contents + root_dir_offset;

    for (size_t i=0; i<max_root_dir_entries; i++) {
        DirEntry entry = get_dir_entry(root_dir, i);
        char filename[13]; // 8 + '.' + 3 + '\0'
        get_filename(&entry, filename);
        if (strcmp(filename, fname) == 0) {
            return entry;
        }
    }
    DirEntry none_entry;
    none_entry.filename[0] = '\0';
    none_entry.start_cluster[0] = 0;
    none_entry.start_cluster[1] = 0;
    none_entry.file_size[0] = 0;
    none_entry.file_size[1] = 0;
    none_entry.file_size[2] = 0;
    none_entry.file_size[3] = 0;
    return none_entry;
}

static void
get_cluster(BootSector * bsector, u8 * img_contents, u8 * dst, size_t idx, size_t num_bytes)
{
    size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((u16 *) bsector->bytes_per_sector);
    size_t spf = *((u16 *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((u16 *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    size_t root_dir_size = sizeof(DirEntry) * max_root_dir_entries;

    size_t cluster_offset = (idx-2)*spc*bps + root_dir_offset + root_dir_size;
    u8 * ptr = (u8 *)(img_contents + cluster_offset);

    memcpy(dst, ptr, num_bytes);
}

// TODO: handle binary files
u8 *
fat12_get_file_contents(BootSector * bsector, u8 * img_contents, const char * fname)
{
    DirEntry entry = get_file_entry(bsector, img_contents, fname);

    // get 12-bit entry from FAT
    size_t bps = *((u16 *) bsector->bytes_per_sector);
    size_t spc = bsector->sectors_per_cluster;
    size_t bpc = bps*spc;
    u8 * fat = img_contents + bps;
    size_t start_cluster = *((u16 *) entry.start_cluster);
    u16 fat_entry = get_fat12_entry(fat, start_cluster);

    u32 fsize = *((u32 *) entry.file_size);
    u8 * file_contents = malloc(sizeof(*file_contents)*(fsize+1));

    size_t cluster_cnt = 0;
    size_t bytes_remaining = fsize;
    size_t cluster_idx = start_cluster;
    do {
        size_t nbytes = (bpc < bytes_remaining) ? bpc : bytes_remaining;
        get_cluster(bsector, img_contents, file_contents + bpc*cluster_cnt, cluster_idx, nbytes);
        bytes_remaining -= bpc;
        fat_entry = get_fat12_entry(fat, fat_entry);
        cluster_idx = fat_entry;
        cluster_cnt++;
    } while (fat_entry != 0xfff);

    file_contents[fsize] = '\0';
    return file_contents;
}
