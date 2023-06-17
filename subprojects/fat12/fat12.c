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
// * Mak sure types are correct (e.g. signed vs unsigned char)

//assert(sizeof(bsect_t) == 512);
//assert(sizeof(dir_entry_t) == 32);

typedef struct dir_entry
{
    uint8_t filename[8]; // if byte 0 == 0x2e, this entry is either "." or ".."
    uint8_t ext[3];
    uint8_t attrs; // 0x10: this entry describes a subdirectory
    uint8_t reserved[10];
    uint8_t tmodified[2];
    uint8_t dmodified[2];
    uint8_t start_cluster[2];
    uint8_t file_size[4];
} dir_entry_t;

void init_bsect(bsect_t * bsector_p) {
    uint8_t * bs_ptr = (uint8_t *) bsector_p;
    for(size_t i=0; i<512; i++) {
        *(bs_ptr+i) = 0;
    }
}

void load_sect_from_file(FILE * fp, size_t sect_num, size_t num_sectors, void * dst) {
    fseek(fp, 512*sect_num, SEEK_SET);
    fread(dst, 1, num_sectors*512, fp); 
}

uint16_t get_fat12_entry(uint8_t * fat, size_t idx) {
    size_t offset = idx*3/2;
    uint8_t byte0 = fat[offset];
    uint8_t byte1 = fat[offset+1];
    uint16_t rv=0;
    if(idx % 2 == 0) {
        rv = ((byte1&0xf) << 8) | byte0;
    } else {
        rv = (byte1 << 4) | ((byte0 >> 4) & 0xf);
    }
    return rv;
}

dir_entry_t get_dir_entry(uint8_t * root_dir, size_t idx) {
    dir_entry_t entry;

    memcpy((void *)&entry, (void*)root_dir+idx*sizeof(dir_entry_t), sizeof(dir_entry_t));

    return entry;
}

void get_filename(dir_entry_t * entry, char * dst) {
    size_t i;
    for(i=0; i<13; i++) {
        dst[i] = '\0';
    }
    size_t fn_len;
    for(i=0; i<8; i++) {
        uint8_t c = entry->filename[i];
        if(c == ' ') {
            break;
        }
        dst[i] = c;
    }
    fn_len = i;
    dst[fn_len] = '.';
    for(i=0; i<3; i++) {
        uint8_t c = entry->ext[i];
        if(c == ' ') {
            break;
        }
        dst[fn_len+1+i] = c;
    }
}

void print_fat(bsect_t * bsector, uint8_t * img_contents) {
    size_t bps = *((uint16_t *) bsector->bytes_per_sector);
    size_t spf = *((uint16_t *) bsector->sectors_per_fat);
    uint8_t * fat = img_contents + bps;

    for(size_t i=0; i<spf*(bps/(16*3)); i++) {
        for(size_t j=0; j<32; j++) {
            uint16_t fat_entry = get_fat12_entry(fat, i*32+j);
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

void list_files(bsect_t * bsector, uint8_t * img_contents) {

    //size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((uint16_t *) bsector->bytes_per_sector);
    size_t spf = *((uint16_t *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((uint16_t *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    //size_t root_dir_size = sizeof(dir_entry_t) * max_root_dir_entries;
    //printf("root_dir_offset: %d\n", root_dir_offset);
    //printf("size of root dir: %d\n", root_dir_size);

    uint8_t * root_dir = img_contents + root_dir_offset;

    for(size_t i=0; i<max_root_dir_entries; i++) {
        dir_entry_t entry = get_dir_entry(root_dir, i);
        unsigned char filename[13]; // 8 + '.' + 3 + '\0'
        unsigned char fsize_str[10];
        unsigned char start_cluster_str[10];
        get_filename(&entry, (char *) filename);

        if(filename[0] == '\0') { // no more files in this directory
            break;
        } else if(filename[0] == 0x2e) { // either "." or ".."
        } else if(filename[0] == 0xe5) { // available
            continue;
        }

        uint32_t fsize = *((uint32_t *) entry.file_size);
        sprintf((char *) fsize_str, "%u", fsize);
        uint16_t start_cluster = *((uint16_t *) entry.start_cluster);
        sprintf((char *) start_cluster_str, "%u", start_cluster);
        int fn_len = strlen((char *) filename);
        int fsize_len = strlen((char *) fsize_str);
        if(fn_len > 0) {
            printf("%s", filename);
            for(int j=0; j<15-fn_len; j++) {
                printf(" ");
            }
            printf("%s", fsize_str);
            for(int j=0; j<15-fsize_len; j++) {
                printf(" ");
            }
            printf("%s\n", start_cluster_str);
        }
    }
}

unsigned char * get_file_ptr(bsect_t * bsector, uint8_t * img_contents, dir_entry_t * entry) {

    size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((uint16_t *) bsector->bytes_per_sector);
    size_t spf = *((uint16_t *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((uint16_t *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    size_t root_dir_size = sizeof(dir_entry_t) * max_root_dir_entries;

    uint16_t start_cluster = *((uint16_t *) entry->start_cluster);

    size_t file_offset = (start_cluster-2)*spc*bps + root_dir_offset + root_dir_size;
    unsigned char * ptr = (unsigned char *)(img_contents + file_offset);
    return ptr;
}

dir_entry_t get_file_entry(bsect_t * bsector, uint8_t * img_contents, const char * fname) {

    //size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((uint16_t *) bsector->bytes_per_sector);
    size_t spf = *((uint16_t *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((uint16_t *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    //size_t root_dir_size = sizeof(dir_entry_t) * max_root_dir_entries;
    uint8_t * root_dir = img_contents + root_dir_offset;

    for(size_t i=0; i<max_root_dir_entries; i++) {
        dir_entry_t entry = get_dir_entry(root_dir, i);
        char filename[13]; // 8 + '.' + 3 + '\0'
        get_filename(&entry, filename);
        if(strcmp(filename, fname) == 0) {
            return entry;
        }
    }
    dir_entry_t none_entry;
    none_entry.filename[0] = '\0';
    none_entry.start_cluster[0] = 0;
    none_entry.start_cluster[1] = 0;
    none_entry.file_size[0] = 0;
    none_entry.file_size[1] = 0;
    none_entry.file_size[2] = 0;
    none_entry.file_size[3] = 0;
    return none_entry;
}

void get_cluster(bsect_t * bsector, uint8_t * img_contents, uint8_t * dst, size_t idx, size_t num_bytes) {

    size_t spc = bsector->sectors_per_cluster;
    size_t bps = *((uint16_t *) bsector->bytes_per_sector);
    size_t spf = *((uint16_t *) bsector->sectors_per_fat);
    size_t num_fats = bsector->num_fats;
    size_t max_root_dir_entries = *((uint16_t *) bsector->max_root_dir_entries);

    size_t root_dir_offset = (1+spf*num_fats)*bps; // +1 for boot sector
    size_t root_dir_size = sizeof(dir_entry_t) * max_root_dir_entries;

    size_t cluster_offset = (idx-2)*spc*bps + root_dir_offset + root_dir_size;
    unsigned char * ptr = (unsigned char *)(img_contents + cluster_offset);

    memcpy(dst, ptr, num_bytes);
}

// TODO: handle binary files
unsigned char * get_file_contents(bsect_t * bsector, uint8_t * img_contents, const char * fname) {
    dir_entry_t entry = get_file_entry(bsector, img_contents, fname);

    // get 12-bit entry from FAT
    size_t bps = *((uint16_t *) bsector->bytes_per_sector);
    size_t spc = bsector->sectors_per_cluster;
    size_t bpc = bps*spc;
    uint8_t * fat = img_contents + bps;
    size_t start_cluster = *((uint16_t *) entry.start_cluster);
    uint16_t fat_entry = get_fat12_entry(fat, start_cluster);

    uint32_t fsize = *((uint32_t *) entry.file_size);
    unsigned char * file_contents = malloc(sizeof(*file_contents)*(fsize+1));

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
    } while(fat_entry != 0xfff);

    file_contents[fsize] = '\0';
    return file_contents;
}
