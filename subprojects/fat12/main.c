#include "common.h"
#include "fat12.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <assert.h>

int main(int argc, char * argv[]) {

    if (argc < 2)
        die("usage: %s FILE\n", argv[0]);

    Buffer img_contents = read_entire_file(argv[1]);

    //fat12_load_sect_from_file(fp, 0, 1, (u8 *) &boot_sector);

    assert(img_contents.len > 512);

    BootSector * boot_sector = img_contents.data;

    //print_bytes((u8 *)&boot_sector, 512, 0);
    print_bytes(img_contents.data, 512, 512);

    printf("================================\n");
    printf("bytes_per_sector:       %" PRIu64 "\n",     extract_bytes_le(&boot_sector->bytes_per_sector,        2));
    printf("sectors_per_cluster:    %" PRIu64 "\n",     extract_bytes_le(&boot_sector->sectors_per_cluster,     1));
    printf("num_reserved:           %" PRIu64 "\n",     extract_bytes_le(&boot_sector->num_reserved,            2));
    printf("num_fats:               %" PRIu64 "\n",     extract_bytes_le(&boot_sector->num_fats,                1));
    printf("max_root_dir_entries:   %" PRIu64 "\n",     extract_bytes_le(&boot_sector->max_root_dir_entries,    2));
    printf("num_sectors:            %" PRIu64 "\n",     extract_bytes_le(&boot_sector->num_sectors,             2));
    printf("media_desc:             %02" PRIx64 "\n",   extract_bytes_le(&boot_sector->media_desc,              1));
    printf("sectors_per_fat:        %" PRId64 "\n",     extract_bytes_le(&boot_sector->sectors_per_fat,         2));
    printf("================================\n");

    fat12_print_fat(boot_sector, img_contents.data);

    fat12_list_files(boot_sector, img_contents.data);

    if (1) {
        Buffer file_contents = fat12_get_file_contents(boot_sector, img_contents.data, "README.");
        printf("===\n%s\n===\n", (char *) file_contents.data);
        free(file_contents.data);
    }

    free(img_contents.data);

    return 0;
}
