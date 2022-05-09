#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define NELEMS(X) sizeof(X)/sizeof(X[0])

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
    struct dirent *ent_p;
    struct dirent ent;

    if (dir == NULL) {
        perror(dir_name);
        exit(EXIT_FAILURE);
    }

    ent_p = readdir(dir);
    while (ent_p != NULL) {
        memcpy(&ent, ent_p, sizeof(ent));
        struct dirent *next_ent;
        struct stat stat_buf;
        static char path[512];

        sprintf(path, "%s/%s", dir_name, ent.d_name);

        if (stat(path, &stat_buf) != 0) {
            perror(ent.d_name);
            exit(EXIT_FAILURE);
        }

        get_next_ent:
        next_ent = readdir(dir);
        if (next_ent == NULL)
            goto null_ent;
        if (strcmp(next_ent->d_name, ".") == 0)
            goto get_next_ent;
        if (strcmp(next_ent->d_name, "..") == 0)
            goto get_next_ent;
        null_ent:

        if ((strcmp(ent.d_name, ".") == 0) || (strcmp(ent.d_name, "..") == 0)) {
            ent_p = next_ent;
            continue;
        }

        for (int i = 0; i < depth; i++) {
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
            printf("└── %s\n", ent.d_name);
        } else {
            printf("├── %s\n", ent.d_name);
        }
#if _WIN32
        if (S_ISDIR(stat_buf.st_mode))
#else
        if (S_ISDIR(stat_buf.st_mode) && !S_ISLNK(stat_buf.st_mode))
#endif
        {
            if (strcmp(dir_name, ".") != 0) {
                chdir(dir_name);
            }
            print_dir_tree_r(ent.d_name, depth+1);
            if (strcmp(dir_name, ".") != 0) {
                chdir("..");
            }
        }
        ent_p = next_ent;
    }
    closedir(dir);
    finals[depth] = 0;
}

void print_dir_tree(const char * dir_name)
{
    for (size_t i = 0; i < NELEMS(finals); i++) {
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
