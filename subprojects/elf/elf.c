#include "elf.h"
#include "common.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>

#define X(ENUM, NUM, STR) case NUM: return STR;
static const char *
elf_mach_string(int num) {
    switch (num) {
ELF_MACH_LIST
    }
    return "";
};
#undef X

#define X(ENUM, NUM, STR) case NUM: return STR;
static const char *
elf_osabi_string(int num) {
    switch (num) {
ELF_OSABI_LIST
    }
    return "";
};
#undef X

static const char *
elf_type_string(int num)
{
    switch (num) {
        case 0: return "None";
        case 1: return "REL (Relocatable file)";
        case 2: return "EXEC (Executable file)";
        case 3: return "DYN (Shared object file)";
        case 4: return "CORE";
    }
    return "";
}

static const char *
elf_phdr_type_string(int num)
{
    switch (num) {
        case PT_NULL    : return "NULL";
        case PT_LOAD    : return "LOAD";
        case PT_DYNAMIC : return "DYNAMIC";
        case PT_INTERP  : return "INTERP";
        case PT_NOTE    : return "NOTE";
        case PT_SHLIB   : return "SHLIB";
        case PT_PHDR    : return "PHDR";
        case PT_TLS     : return "TLS";
        case 1685382480 : return "GNU_EH_FRAME";
        case 1685382481 : return "GNU_STACK";
        case 1685382482 : return "GNU_RELRO";
        case 1685382483 : return "GNU_PROPERTY";
    }
    return "";
}

static const char *
elf_sect_type_string(int num)
{
    switch (num) {
        case SHT_NULL            : return "NULL";
        case SHT_PROGBITS        : return "PROGBITS";
        case SHT_SYMTAB          : return "SYMTAB";
        case SHT_STRTAB          : return "STRTAB";
        case SHT_RELA            : return "RELA";
        case SHT_HASH            : return "HASH";
        case SHT_DYNAMIC         : return "DYNAMIC";
        case SHT_NOTE            : return "NOTE";
        case SHT_NOBITS          : return "NOBITS";
        case SHT_REL             : return "REL";
        case SHT_SHLIB           : return "SHLIB";
        case SHT_DYNSYM          : return "DYNSYM";
        case SHT_INIT_ARRAY      : return "INIT_ARRAY";
        case SHT_FINI_ARRAY      : return "FINI_ARRAY";
        case SHT_PREINIT_ARRAY   : return "PREINIT_ARRAY";
        case SHT_GROUP           : return "GROUP";
        case SHT_SYMTAB_SHNDX    : return "SYMTAB_SHNDX";
        case SHT_NUM             : return "NUM";
        case SHT_LOOS            : return "LOOS";
    }
    return "";
}

const char *
elferrstr(ElfErr errnum)
{
    switch (errnum) {
        case ELFERR_MAGIC:      return "incorrect magic number";
        case ELFERR_SIZE:       return "incorrect file size";
        case ELFERR_CLASS:      return "invalid class field";
        case ELFERR_DATA:       return "invalid data field";
        case ELFERR_VERSION:    return "invalid version";
        case ELFERR_TYPE:       return "invalid type";
        case ELFERR_EHSIZE:     return "invalid elf header size";
        default:                return "";
    }
    return "";
}

Elf
read_elf(const char * filename)
{
    Elf elf = (Elf) {0};
    Buffer buffer = read_file(filename);

    if (buffer.len < 52) {
        elf.error = ELFERR_SIZE;
        return elf;
    }

    memcpy(elf.header.e_ident.ei_mag, buffer.p, 4);

    const uint8_t magic[] = {0x7f, 'E', 'L', 'F'};
    if (memcmp(elf.header.e_ident.ei_mag, magic, 4) != 0) {
        elf.error = ELFERR_MAGIC;
        return elf;
    }

    elf.header.e_ident.ei_class    = unpack_le(buffer.p + 4, 1);

    bool _64bit = elf.header.e_ident.ei_class == 2;

    if (_64bit && (buffer.len < 64)) {
        elf.error = ELFERR_SIZE;
        return elf;
    }

    elf.header.e_ident.ei_data          = unpack_le(buffer.p + 5,                   1);
    elf.header.e_ident.ei_version       = unpack_le(buffer.p + 6,                   1);
    elf.header.e_ident.ei_osabi         = unpack_le(buffer.p + 7,                   1);
    elf.header.e_ident.ei_abiversion    = unpack_le(buffer.p + 8,                   1);
    elf.header.e_type                   = unpack_le(buffer.p + 16,                  2);
    elf.header.e_machine                = unpack_le(buffer.p + 18,                  2);
    elf.header.e_version                = unpack_le(buffer.p + 20,                  4);
    elf.header.e_entry                  = unpack_le(buffer.p + 24,                  _64bit ? 8 : 4);
    elf.header.e_phoff                  = unpack_le(buffer.p + (_64bit ? 32 : 28),  _64bit ? 8 : 4);
    elf.header.e_shoff                  = unpack_le(buffer.p + (_64bit ? 40 : 32),  _64bit ? 8 : 4);
    elf.header.e_flags                  = unpack_le(buffer.p + (_64bit ? 48 : 36),  4);
    elf.header.e_ehsize                 = unpack_le(buffer.p + (_64bit ? 52 : 40),  2);
    elf.header.e_phentsize              = unpack_le(buffer.p + (_64bit ? 54 : 42),  2);
    elf.header.e_phnum                  = unpack_le(buffer.p + (_64bit ? 56 : 44),  2);
    elf.header.e_shentsize              = unpack_le(buffer.p + (_64bit ? 58 : 46),  2);
    elf.header.e_shnum                  = unpack_le(buffer.p + (_64bit ? 60 : 48),  2);
    elf.header.e_shstrndx               = unpack_le(buffer.p + (_64bit ? 62 : 50),  2);

    if ((elf.header.e_ident.ei_class != 1) && (elf.header.e_ident.ei_class != 2)) {
        elf.error = ELFERR_CLASS;
        return elf;
    }

    if ((elf.header.e_ident.ei_data != 1) && (elf.header.e_ident.ei_data != 2)) {
        elf.error = ELFERR_DATA;
        return elf;
    }

    if (elf.header.e_ident.ei_version != 1) {
        elf.error = ELFERR_VERSION;
        return elf;
    }

    if ((elf.header.e_type < 1) || (elf.header.e_type > 4)) {
        elf.error = ELFERR_TYPE;
        return elf;
    }

    if ((!_64bit && elf.header.e_ehsize != 52) || (_64bit && elf.header.e_ehsize != 64)) {
        elf.error = ELFERR_EHSIZE;
        return elf;
    }

    // TODO: error-checking for e_phentsize and e_shentsize

    if (buffer.len < elf.header.e_phoff + elf.header.e_phnum * elf.header.e_phentsize) {
        elf.error = ELFERR_SIZE;
        return elf;
    }

    elf.program_headers = malloc(sizeof(*elf.program_headers) * elf.header.e_phnum);
    for (size_t i = 0; i < elf.header.e_phnum; i++) {
        char * p = buffer.p + elf.header.e_phoff + i*elf.header.e_phentsize;
        elf.program_headers[i].p_type   = unpack_le(p, 4);
        elf.program_headers[i].p_flags  = unpack_le(p + (_64bit ? 4 : 24),  4);
        elf.program_headers[i].p_offset = unpack_le(p + (_64bit ? 8 : 4),   _64bit ? 8 : 4);
        elf.program_headers[i].p_vaddr  = unpack_le(p + (_64bit ? 16 : 8),  _64bit ? 8 : 4);
        elf.program_headers[i].p_paddr  = unpack_le(p + (_64bit ? 24 : 12), _64bit ? 8 : 4);
        elf.program_headers[i].p_filesz = unpack_le(p + (_64bit ? 32 : 16), _64bit ? 8 : 4);
        elf.program_headers[i].p_memsz  = unpack_le(p + (_64bit ? 40 : 20), _64bit ? 8 : 4);
        elf.program_headers[i].p_align  = unpack_le(p + (_64bit ? 48 : 28), _64bit ? 8 : 4);
    }

    if (buffer.len < elf.header.e_shoff + elf.header.e_shnum * elf.header.e_shentsize) {
        elf.error = ELFERR_SIZE;
        return elf;
    }

    elf.section_headers = malloc(sizeof(*elf.section_headers) * elf.header.e_shnum);
    for (size_t i = 0; i < elf.header.e_shnum; i++) {
        char * p = buffer.p + elf.header.e_shoff + i*elf.header.e_shentsize;
        //size_t sh_name = unpack_le(p, 4); // TODO
        elf.section_headers[i].sh_name      = unpack_le(p, 4);
        //elf.section_headers[i].sh_name      = strdup(buffer.p + elf.header.e_shstrndx + sh_name);
        //elf.section_headers[i].sh_name      = strdup("test");
        elf.section_headers[i].sh_type      = unpack_le(p + 4,                  4);
        elf.section_headers[i].sh_flags     = unpack_le(p,                      8);
        elf.section_headers[i].sh_addr      = unpack_le(p + (_64bit ? 16 : 12), 8);
        elf.section_headers[i].sh_offset    = unpack_le(p + (_64bit ? 24 : 16), 8);
        elf.section_headers[i].sh_size      = unpack_le(p + (_64bit ? 32 : 20), 8);
        elf.section_headers[i].sh_link      = unpack_le(p + (_64bit ? 40 : 24), 4);
        elf.section_headers[i].sh_info      = unpack_le(p + (_64bit ? 44 : 28), 4);
        elf.section_headers[i].sh_addralign = unpack_le(p + (_64bit ? 48 : 32), 8);
        elf.section_headers[i].sh_entsize   = unpack_le(p + (_64bit ? 56 : 36), 8);
    }

    for (size_t i = 0; i < elf.header.e_shnum; i++) {
        printf("FOO: %s\n", buffer.p + elf.section_headers[elf.header.e_shstrndx].sh_offset + elf.section_headers[i].sh_name);
    }

    elf.segments = malloc(sizeof(*elf.segments) * elf.header.e_phnum);
    elf.sections = malloc(sizeof(*elf.sections) * elf.header.e_shnum);

    for (size_t i = 0; i < elf.header.e_phnum; i++) {
        elf.segments[i] = malloc(elf.program_headers[i].p_filesz); // may be 0
        memcpy(elf.segments[i], buffer.p + elf.program_headers[i].p_offset, elf.program_headers[i].p_filesz);
    }

    for (size_t i = 0; i < elf.header.e_shnum; i++) {
        elf.sections[i] = malloc(elf.section_headers[i].sh_size); // may be 0
        memcpy(elf.sections[i], buffer.p + elf.section_headers[i].sh_offset, elf.section_headers[i].sh_size);
    }

    free(buffer.p);

    elf.valid = 1;
    elf.error = 0;
    return elf;
}

void
destroy_elf(Elf elf)
{
    for (size_t i = 0; i < elf.header.e_phnum; i++)
        free(elf.segments[i]);
    for (size_t i = 0; i < elf.header.e_shnum; i++)
        free(elf.sections[i]);
    free(elf.segments);
    free(elf.sections);
    free(elf.program_headers);
    elf.program_headers = NULL;
    free(elf.section_headers);
    elf.section_headers = NULL;
}

#if 1
static const char *
elf_section_name(Elf elf, size_t i)
{
    return elf.sections[elf.header.e_shstrndx] + elf.section_headers[i].sh_name;
}
#endif

void
print_elf(Elf elf, FILE * fp)
{
    if (elf.valid == 0) {
        fprintf(fp, "error: %s\n", elferrstr(elf.error));
        return;
    }
    fprintf(fp, "ELF Header\n");
    fprintf(fp, "  Magic: ");
    for (size_t i = 0; i < 4; i++)
        fprintf(fp, "%02" PRIx8 " ", elf.header.e_ident.ei_mag[i]);
    fprintf(fp, "\n");
    fprintf(fp, "  Class:                             ELF%d\n", elf.header.e_ident.ei_class == 1 ? 32 : 64);
    fprintf(fp, "  Data:                              %s endian\n", elf.header.e_ident.ei_data == 1 ? "little" : "big");
    fprintf(fp, "  Version:                           %" PRId8 "\n", elf.header.e_ident.ei_version);
    fprintf(fp, "  OS/ABI:                            %s\n", elf_osabi_string(elf.header.e_ident.ei_osabi));
    fprintf(fp, "  ABI Version:                       %" PRId8 "\n", elf.header.e_ident.ei_abiversion);

    fprintf(fp, "  Type:                              %s\n", elf_type_string(elf.header.e_type));
    fprintf(fp, "  Machine:                           %s\n", elf_mach_string(elf.header.e_machine));
    fprintf(fp, "  Version:                           %" PRId32 "\n", elf.header.e_version      );
    fprintf(fp, "  Entry point address:               0x%" PRIx64 "\n", elf.header.e_entry      );
    fprintf(fp, "  Start of program headers:          %" PRId64 "\n", elf.header.e_phoff        );
    fprintf(fp, "  Start of section headers:          %" PRId64 "\n", elf.header.e_shoff        );
    fprintf(fp, "  Flags:                             %" PRId32 "\n", elf.header.e_flags        );
    fprintf(fp, "  Size of this header:               %" PRId16 "\n", elf.header.e_ehsize       );
    fprintf(fp, "  Size of program headers:           %" PRId16 "\n", elf.header.e_phentsize    );
    fprintf(fp, "  Number of program headers:         %" PRId16 "\n", elf.header.e_phnum        );
    fprintf(fp, "  Size of section headers:           %" PRId16 "\n", elf.header.e_shentsize    );
    fprintf(fp, "  Number of section headers:         %" PRId16 "\n", elf.header.e_shnum        );
    fprintf(fp, "  Section header string table index: %" PRId16 "\n", elf.header.e_shstrndx     );
    fprintf(fp, "\n");

    fprintf(fp, "Section Headers:\n");
    fprintf(fp, "  [Nr] Name              Type            Address          Off    Size   ES Flg Lk Inf Al\n");
    for (size_t i = 0; i < elf.header.e_shnum; i++) {
        fprintf(fp, "  [%2zu] ", i);
        fprintf(fp, "%-17s ", elf_section_name(elf, i)); // TODO
        //fprintf(fp, "%-18" PRId32, elf.section_headers[i].sh_name); // TODO
        //fprintf(fp, "%-18s", elf.section_headers[i].sh_name);
        fprintf(fp, "%-15s ", elf_sect_type_string(elf.section_headers[i].sh_type));
        fprintf(fp, "%016" PRIx64 " ", elf.section_headers[i].sh_addr);
        fprintf(fp, "%06" PRIx64 " ", elf.section_headers[i].sh_offset);
        fprintf(fp, "%06" PRIx64 " ", elf.section_headers[i].sh_size);
        fprintf(fp, "%02" PRIx64 " ", elf.section_headers[i].sh_entsize);
        fprintf(fp, "%" PRId64 "\t", elf.section_headers[i].sh_flags); // TODO
        fprintf(fp, "%" PRId32 " ", elf.section_headers[i].sh_link);
        fprintf(fp, "%" PRId32 " ", elf.section_headers[i].sh_info);
        fprintf(fp, "%" PRId64 "\n", elf.section_headers[i].sh_addralign);
    }

    fprintf(fp, "Program Headers\n");
    fprintf(fp, "  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
    for (size_t i = 0; i < elf.header.e_phnum; i++) {
        fprintf(fp, "  %-15s", elf_phdr_type_string(elf.program_headers[i].p_type));
        fprintf(fp, "0x%06"  PRIx64 " ",  elf.program_headers[i].p_offset);
        fprintf(fp, "0x%016" PRIx64 " ",  elf.program_headers[i].p_vaddr);
        fprintf(fp, "0x%016" PRIx64 " ",  elf.program_headers[i].p_paddr);
        fprintf(fp, "0x%06"  PRIx64 " ",  elf.program_headers[i].p_filesz);
        fprintf(fp, "0x%06"  PRIx64 " ",  elf.program_headers[i].p_memsz);
        fprintf(fp, "0x%"    PRIx32 " ",  elf.program_headers[i].p_flags); // TODO
        fprintf(fp, "0x%"    PRIx64 "\n", elf.program_headers[i].p_align);
    }
}
