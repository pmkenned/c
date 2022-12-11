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

#define X(ENUM, STR, NUM, FIELD) case NUM: return STR;
static const char *
elf_relocation_string(int num) {
    switch (num) {
X86_64_RELOCATIONS
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
        case PT_GNU_EH_FRAME:   return "GNU_EH_FRAME";
        case PT_GNU_STACK:      return "GNU_STACK";
        case PT_GNU_RELRO:      return "GNU_RELRO";
        case PT_GNU_PROPERTY:   return "GNU_PROPERTY";
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
        case SHT_GNU_HASH        : return "GNU_HASH";
        case SHT_VERSYM          : return "VERSYM";
        case SHT_VERNEED         : return "VERNEED";
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
        elf.section_headers[i].sh_name      = unpack_le(p, 4);
        elf.section_headers[i].sh_type      = unpack_le(p + 4,                  4);
        elf.section_headers[i].sh_flags     = unpack_le(p + 8,                  8);
        elf.section_headers[i].sh_addr      = unpack_le(p + (_64bit ? 16 : 12), 8);
        elf.section_headers[i].sh_offset    = unpack_le(p + (_64bit ? 24 : 16), 8);
        elf.section_headers[i].sh_size      = unpack_le(p + (_64bit ? 32 : 20), 8);
        elf.section_headers[i].sh_link      = unpack_le(p + (_64bit ? 40 : 24), 4);
        elf.section_headers[i].sh_info      = unpack_le(p + (_64bit ? 44 : 28), 4);
        elf.section_headers[i].sh_addralign = unpack_le(p + (_64bit ? 48 : 32), 8);
        elf.section_headers[i].sh_entsize   = unpack_le(p + (_64bit ? 56 : 36), 8);
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

static const char *
elf_section_name(Elf elf, size_t i)
{
    return elf.sections[elf.header.e_shstrndx] + elf.section_headers[i].sh_name;
}

static void
elf_shdr_flag_str(char s[], uint32_t sh_flags)
{
    size_t i = 0;
    if (sh_flags & 1)  s[i++] = 'W';
    if (sh_flags & 2)  s[i++] = 'A';
    if (sh_flags & 4)  s[i++] = 'X';
    if (sh_flags & 16) s[i++] = 'M';
    if (sh_flags & 32) s[i++] = 'S';
    if (sh_flags & 64) s[i++] = 'I';
    s[i] = '\0';
}

static void
elf_phdr_flag_str(char s[], uint32_t p_flags)
{
    s[0] = (p_flags & 4) ? 'R' : ' ';
    s[1] = (p_flags & 2) ? 'W' : ' ';
    s[2] = (p_flags & 1) ? 'E' : ' ';
    s[3] = '\0';
}

static const char *
stt_string(uint8_t st_type)
{
    switch (st_type) {
        case STT_NOTYPE:    return "NOTYPE";
        case STT_OBJECT:    return "OBJECT";
        case STT_FUNC:      return "FUNC";
        case STT_SECTION:   return "SECTION";
        case STT_FILE:      return "FILE";
        case STT_COMMON:    return "COMMON";
    }
    return "";
}

static const char *
stb_string(uint8_t st_bind)
{
    switch (st_bind) {
        case STB_LOCAL:     return "LOCAL";
        case STB_GLOBAL:    return "GLOBAL";
        case STB_WEAK:      return "WEAK";
    }
    return "";
}

static const char *
stv_string(uint8_t st_other)
{
    switch (st_other) {
        case STV_DEFAULT:   return "DEFAULT";
        case STV_INTERNAL:  return "INTERNAL";
        case STV_HIDDEN:    return "HIDDEN";
        case STV_PROTECTED: return "PROTECTED";
    }
    return "";
}

void
print_elf(Elf elf, FILE * fp)
{
    if (elf.valid == 0) {
        fprintf(fp, "error: %s\n", elferrstr(elf.error));
        return;
    }
    fprintf(fp, "ELF Header:\n");
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
        ElfSectionHeader * sh = &elf.section_headers[i];
        char sh_flags[7];
        elf_shdr_flag_str(sh_flags, sh->sh_flags);
        fprintf(fp, "  [%2zu] ", i);
        fprintf(fp, "%-17s ", elf_section_name(elf, i));
        fprintf(fp, "%-15s ", elf_sect_type_string(sh->sh_type));
        fprintf(fp, "%016" PRIx64 " ",  sh->sh_addr);
        fprintf(fp, "%06"  PRIx64 " ",  sh->sh_offset);
        fprintf(fp, "%06"  PRIx64 " ",  sh->sh_size);
        fprintf(fp, "%02"  PRIx64 " ",  sh->sh_entsize);
        fprintf(fp, "%3s ", sh_flags);
        fprintf(fp, "%2"   PRId32 " ",  sh->sh_link);
        fprintf(fp, "%3"   PRId32 " ",  sh->sh_info);
        fprintf(fp, "%2"   PRId64 "\n", sh->sh_addralign);
    }

    fprintf(fp, "Program Headers:\n");
    fprintf(fp, "  Type           Offset   VirtAddr           PhysAddr           FileSiz  MemSiz   Flg Align\n");
    for (size_t i = 0; i < elf.header.e_phnum; i++) {
        ElfProgramHeader * ph = &elf.program_headers[i];
        char p_flags[4];
        elf_phdr_flag_str(p_flags, ph->p_flags);
        fprintf(fp, "  %-15s", elf_phdr_type_string(ph->p_type));
        fprintf(fp, "0x%06"  PRIx64 " ",  ph->p_offset);
        fprintf(fp, "0x%016" PRIx64 " ",  ph->p_vaddr);
        fprintf(fp, "0x%016" PRIx64 " ",  ph->p_paddr);
        fprintf(fp, "0x%06"  PRIx64 " ",  ph->p_filesz);
        fprintf(fp, "0x%06"  PRIx64 " ",  ph->p_memsz);
        fprintf(fp, "%-3s ", p_flags);
        fprintf(fp, "0x%"    PRIx64 "\n", ph->p_align);
    }
    fprintf(fp, "\n");

    fprintf(fp, " Section to Segment mapping:\n");
    fprintf(fp, "  Segment Sections...\n");
    for (size_t i = 0; i < elf.header.e_phnum; i++) {
        fprintf(fp, "   %02zu\n", i); // TODO: determine which sections are in each segment
    }
    fprintf(fp, "\n");

    for (size_t shi = 0; shi < elf.header.e_shnum; shi++) {
        ElfSectionHeader * sh = &elf.section_headers[shi];
        // TODO: also SHT_REL
        if (sh->sh_type == SHT_RELA) {
            size_t num_relocs = sh->sh_size / sh->sh_entsize; // TODO: check for divide by zero
            fprintf(fp, "Relocation section '%s' at offset 0x%" PRIx64 " contains %zu entries:\n", elf_section_name(elf, shi), sh->sh_offset, num_relocs);
            fprintf(fp, "    Offset             Info             Type               Symbol's Value  Symbol's Name + Addend\n");
            for (size_t i = 0; i < num_relocs; i++) {
                char * entry = elf.sections[shi] + sh->sh_entsize*i;
                uint64_t rela_offset    = unpack_le(entry + 0, 8);
                uint64_t rela_info      = unpack_le(entry + 8, 8);
                fprintf(fp, "%016" PRIx64 "  %016" PRIx64 " %s                         %d\n", rela_offset, rela_info, elf_relocation_string(rela_info & 0xff), 0);
            }
            fprintf(fp, "\n");
        }
    }

    for (size_t shi = 0; shi < elf.header.e_shnum; shi++) {
        ElfSectionHeader * sh = &elf.section_headers[shi];
        if (sh->sh_type == SHT_SYMTAB || sh->sh_type == SHT_DYNSYM) {
            size_t num_symbols = sh->sh_size / sh->sh_entsize; // TODO: check for divide by zero
            fprintf(fp, "Symbol table '%s' contains %zu entries:\n", elf_section_name(elf, shi), num_symbols);
            fprintf(fp, "   Num:    Value          Size Type    Bind   Vis      Ndx Name\n");
            for (size_t i = 0; i < num_symbols; i++) {
                ElfSymbolTable st;
                char * entry = elf.sections[shi] + sh->sh_entsize*i;
                st.st_name    = unpack_le(entry + 0, 4);
                st.st_info    = unpack_le(entry + 4, 1);
                st.st_other   = unpack_le(entry + 5, 1);
                st.st_shndx   = unpack_le(entry + 6, 2);
                st.st_value   = unpack_le(entry + 8, 8);
                st.st_size    = unpack_le(entry + 16, 8);
                uint8_t st_bind = st.st_info >> 4;
                uint8_t st_type = st.st_info & 0xf;
                fprintf(fp, "%6zu: %016" PRIx64 " %5" PRId64 " %-7s %-6s %-8s ", i, st.st_value, st.st_size, stt_string(st_type), stb_string(st_bind), stv_string(st.st_other));
                if (st.st_shndx == SHN_UNDEF) {
                    fprintf(fp, "UND ");
                } else if (st.st_shndx == SHN_ABS) {
                    fprintf(fp, "ABS ");
                } else {
                    fprintf(fp, "%3" PRId16 " ", st.st_shndx);
                }
                fprintf(fp, "%s\n", elf.sections[sh->sh_link] + st.st_name);
            }
            fprintf(fp, "\n");
        }
    }
}
