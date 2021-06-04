#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

//enum { BUFFER_SIZE = 1024 };
//char buffer[BUFFER_SIZE];

//      0123456789abcdef
// 250* ─ │         ┌   
// 251* ┐   └   ┘   ├   
// 252*     ┤       ┬   
// 253*     ┴       ┼   
// 254* 
// 255* ═║╒╓╔╕╖╗╘╙╚╛╜╝╞╟
// 256* ╠╡╢╣╤╥╦╧╨╩╪╫╬   

// ┌ ─ ┬ ─ ┐
//          
// ├       │
//          
// │       │
//          
// ┤       │
//          
// ┼       │
//          
// └ ─ ┴ ─ ┘

#define BOX_HOR     "\u2500"
#define BOX_VER     "\u2502"
#define BOX_T       "\u252c"
#define BOX_CCT     "\u251c"
#define BOX_CT      "\u2524"
#define BOX_UPT     "\u2534"
#define BOX_CROSS   "\u253c"
#define BOX_LLC     "\u2514"
#define BOX_LRC     "\u2518"
#define BOX_ULC     "\u250c"
#define BOX_URC     "\u2510"

int finals[128];

void print_dir_tree_r(const char * dir_name, int depth)
{
    DIR *dir = opendir(dir_name);
    struct dirent *ent;
    int i;
    char path[512];

    if (dir == NULL) {
        perror(dir_name);
        exit(EXIT_FAILURE);
    }

    ent = readdir(dir);
    while (ent != NULL) {
        struct dirent *next_ent;
        struct stat buf;

        sprintf(path, "%s/%s", dir_name, ent->d_name);

        if (stat(path, &buf) != 0) {
            perror(ent->d_name);
            exit(EXIT_FAILURE);
        }

        do  {
            next_ent = readdir(dir);
        } while ((next_ent != NULL) && ((strcmp(next_ent->d_name, ".") == 0) || (strcmp(next_ent->d_name, "..") == 0)));

        if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0)) {
            for (i=0; i<depth; i++) {
                if (finals[i]) {
                    printf("    ");
                } else {
                    printf("│   ");
                }
            }
            if (next_ent == NULL) {
                finals[depth] = 1;
            }
            if (finals[depth]) {
                printf("└── %s\n", ent->d_name);
            } else {
                printf("├── %s\n", ent->d_name);
            }
            if (S_ISDIR(buf.st_mode) && !S_ISLNK(buf.st_mode)) {
                if (strcmp(dir_name, ".") != 0) {
                    chdir(dir_name);
                }
                print_dir_tree_r(ent->d_name, depth+1);
                if (strcmp(dir_name, ".") != 0) {
                    chdir("..");
                }
            }
        }
        ent = next_ent;
    }
    closedir(dir);
    finals[depth] = 0;
}

void print_dir_tree(const char * dir_name)
{
    size_t i;
    for (i=0; i<sizeof(finals)/sizeof(finals[0]); i++) {
        finals[i] = 0;
    }
    printf("%s\n", dir_name);
    print_dir_tree_r(dir_name, 0);
}

int main(int argc, char * argv[])
{
    const char * dir_name = (argc > 1) ? argv[1] : ".";
    print_dir_tree(dir_name);
    return 0;
}
