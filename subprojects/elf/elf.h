#ifndef ELF_H
#define ELF_H

#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ELFERR_NONE,
    ELFERR_MAGIC,
    ELFERR_SIZE,
    ELFERR_CLASS,
    ELFERR_DATA,
    ELFERR_VERSION,
    ELFERR_TYPE,
    ELFERR_EHSIZE,
} ElfErr;

enum {
    ELFCLASS_32=1,
    ELFCLASS_64=2
};

enum {
    ELFDATA_LSB=1,
    ELFDATA_MSB=2
};

#define ELF_MACH_LIST \
X(EM_NONE          , 0,   "No machine") \
X(EM_M32           , 1,   "AT&T WE 32100") \
X(EM_SPARC         , 2,   "SPARC") \
X(EM_386           , 3,   "Intel 80386") \
X(EM_68K           , 4,   "Motorola 68000") \
X(EM_88K           , 5,   "Motorola 88000") \
X(EM_IAMCU         , 6,   "Intel MCU") \
X(EM_860           , 7,   "Intel 80860") \
X(EM_MIPS          , 8,   "MIPS I Architecture") \
X(EM_S370          , 9,   "IBM System/370 Processor") \
X(EM_MIPS_RS3_LE   , 10,  "MIPS RS3000 Little-endian") \
X(EM_PARISC        , 15,  "Hewlett-Packard PA-RISC") \
X(EM_VPP500        , 17,  "Fujitsu VPP500") \
X(EM_SPARC32PLUS   , 18,  "Enhanced instruction set SPARC") \
X(EM_960           , 19,  "Intel 80960") \
X(EM_PPC           , 20,  "PowerPC") \
X(EM_PPC64         , 21,  "64-bit PowerPC") \
X(EM_S390          , 22,  "IBM System/390 Processor") \
X(EM_SPU           , 23,  "IBM SPU/SPC") \
X(EM_V800          , 36,  "NEC V800") \
X(EM_FR20          , 37,  "Fujitsu FR20") \
X(EM_RH32          , 38,  "TRW RH-32") \
X(EM_RCE           , 39,  "Motorola RCE") \
X(EM_ARM           , 40,  "ARM 32-bit architecture (AARCH32)") \
X(EM_ALPHA         , 41,  "Digital Alpha") \
X(EM_SH            , 42,  "Hitachi SH") \
X(EM_SPARCV9       , 43,  "SPARC Version 9") \
X(EM_TRICORE       , 44,  "Siemens TriCore embedded processor") \
X(EM_ARC           , 45,  "Argonaut RISC Core, Argonaut Technologies Inc.") \
X(EM_H8_300        , 46,  "Hitachi H8/300") \
X(EM_H8_300H       , 47,  "Hitachi H8/300H") \
X(EM_H8S           , 48,  "Hitachi H8S") \
X(EM_H8_500        , 49,  "Hitachi H8/500") \
X(EM_IA_64         , 50,  "Intel IA-64 processor architecture") \
X(EM_MIPS_X        , 51,  "Stanford MIPS-X") \
X(EM_COLDFIRE      , 52,  "Motorola ColdFire") \
X(EM_68HC12        , 53,  "Motorola M68HC12") \
X(EM_MMA           , 54,  "Fujitsu MMA Multimedia Accelerator") \
X(EM_PCP           , 55,  "Siemens PCP") \
X(EM_NCPU          , 56,  "Sony nCPU embedded RISC processor") \
X(EM_NDR1          , 57,  "Denso NDR1 microprocessor") \
X(EM_STARCORE      , 58,  "Motorola Star*Core processor") \
X(EM_ME16          , 59,  "Toyota ME16 processor") \
X(EM_ST100         , 60,  "STMicroelectronics ST100 processor") \
X(EM_TINYJ         , 61,  "Advanced Logic Corp. TinyJ embedded processor family") \
X(EM_X86_64        , 62,  "AMD x86-64 architecture") \
X(EM_PDSP          , 63,  "Sony DSP Processor") \
X(EM_PDP10         , 64,  "Digital Equipment Corp. PDP-10") \
X(EM_PDP11         , 65,  "Digital Equipment Corp. PDP-11") \
X(EM_FX66          , 66,  "Siemens FX66 microcontroller") \
X(EM_ST9PLUS       , 67,  "STMicroelectronics ST9+ 8/16 bit microcontroller") \
X(EM_ST7           , 68,  "STMicroelectronics ST7 8-bit microcontroller") \
X(EM_68HC16        , 69,  "Motorola MC68HC16 Microcontroller") \
X(EM_68HC11        , 70,  "Motorola MC68HC11 Microcontroller") \
X(EM_68HC08        , 71,  "Motorola MC68HC08 Microcontroller") \
X(EM_68HC05        , 72,  "Motorola MC68HC05 Microcontroller") \
X(EM_SVX           , 73,  "Silicon Graphics SVx") \
X(EM_ST19          , 74,  "STMicroelectronics ST19 8-bit microcontroller") \
X(EM_VAX           , 75,  "Digital VAX") \
X(EM_CRIS          , 76,  "Axis Communications 32-bit embedded processor") \
X(EM_JAVELIN       , 77,  "Infineon Technologies 32-bit embedded processor") \
X(EM_FIREPATH      , 78,  "Element 14 64-bit DSP Processor") \
X(EM_ZSP           , 79,  "LSI Logic 16-bit DSP Processor") \
X(EM_MMIX          , 80,  "Donald Knuth's educational 64-bit processor") \
X(EM_HUANY         , 81,  "Harvard University machine-independent object files") \
X(EM_PRISM         , 82,  "SiTera Prism") \
X(EM_AVR           , 83,  "Atmel AVR 8-bit microcontroller") \
X(EM_FR30          , 84,  "Fujitsu FR30") \
X(EM_D10V          , 85,  "Mitsubishi D10V") \
X(EM_D30V          , 86,  "Mitsubishi D30V") \
X(EM_V850          , 87,  "NEC v850") \
X(EM_M32R          , 88,  "Mitsubishi M32R") \
X(EM_MN10300       , 89,  "Matsushita MN10300") \
X(EM_MN10200       , 90,  "Matsushita MN10200") \
X(EM_PJ            , 91,  "picoJava") \
X(EM_OPENRISC      , 92,  "OpenRISC 32-bit embedded processor") \
X(EM_ARC_COMPACT   , 93,  "ARC International ARCompact processor (old spelling/synonym: EM_ARC_A5)") \
X(EM_XTENSA        , 94,  "Tensilica Xtensa Architecture") \
X(EM_VIDEOCORE     , 95,  "Alphamosaic VideoCore processor") \
X(EM_TMM_GPP       , 96,  "Thompson Multimedia General Purpose Processor") \
X(EM_NS32K         , 97,  "National Semiconductor 32000 series") \
X(EM_TPC           , 98,  "Tenor Network TPC processor") \
X(EM_SNP1K         , 99,  "Trebia SNP 1000 processor") \
X(EM_ST200         , 100, "STMicroelectronics (www.st.com) ST200 microcontroller") \
X(EM_IP2K          , 101, "Ubicom IP2xxx microcontroller family") \
X(EM_MAX           , 102, "MAX Processor") \
X(EM_CR            , 103, "National Semiconductor CompactRISC microprocessor") \
X(EM_F2MC16        , 104, "Fujitsu F2MC16") \
X(EM_MSP430        , 105, "Texas Instruments embedded microcontroller msp430") \
X(EM_BLACKFIN      , 106, "Analog Devices Blackfin (DSP) processor") \
X(EM_SE_C33        , 107, "S1C33 Family of Seiko Epson processors") \
X(EM_SEP           , 108, "Sharp embedded microprocessor") \
X(EM_ARCA          , 109, "Arca RISC Microprocessor") \
X(EM_UNICORE       , 110, "Microprocessor series from PKU-Unity Ltd. and MPRC of Peking University") \
X(EM_EXCESS        , 111, "eXcess: 16/32/64-bit configurable embedded CPU") \
X(EM_DXP           , 112, "Icera Semiconductor Inc. Deep Execution Processor") \
X(EM_ALTERA_NIOS2  , 113, "Altera Nios II soft-core processor") \
X(EM_CRX           , 114, "National Semiconductor CompactRISC CRX microprocessor") \
X(EM_XGATE         , 115, "Motorola XGATE embedded processor") \
X(EM_C166          , 116, "Infineon C16x/XC16x processor") \
X(EM_M16C          , 117, "Renesas M16C series microprocessors") \
X(EM_DSPIC30F      , 118, "Microchip Technology dsPIC30F Digital Signal Controller") \
X(EM_CE            , 119, "Freescale Communication Engine RISC core") \
X(EM_M32C          , 120, "Renesas M32C series microprocessors") \
X(EM_TSK3000       , 131, "Altium TSK3000 core") \
X(EM_RS08          , 132, "Freescale RS08 embedded processor") \
X(EM_SHARC         , 133, "Analog Devices SHARC family of 32-bit DSP processors") \
X(EM_ECOG2         , 134, "Cyan Technology eCOG2 microprocessor") \
X(EM_SCORE7        , 135, "Sunplus S+core7 RISC processor") \
X(EM_DSP24         , 136, "New Japan Radio (NJR) 24-bit DSP Processor") \
X(EM_VIDEOCORE3    , 137, "Broadcom VideoCore III processor") \
X(EM_LATTICEMICO32 , 138, "RISC processor for Lattice FPGA architecture") \
X(EM_SE_C17        , 139, "Seiko Epson C17 family") \
X(EM_TI_C6000      , 140, "The Texas Instruments TMS320C6000 DSP family") \
X(EM_TI_C2000      , 141, "The Texas Instruments TMS320C2000 DSP family") \
X(EM_TI_C5500      , 142, "The Texas Instruments TMS320C55x DSP family") \
X(EM_TI_ARP32      , 143, "Texas Instruments Application Specific RISC Processor, 32bit fetch") \
X(EM_TI_PRU        , 144, "Texas Instruments Programmable Realtime Unit") \
X(EM_MMDSP_PLUS    , 160, "STMicroelectronics 64bit VLIW Data Signal Processor") \
X(EM_CYPRESS_M8C   , 161, "Cypress M8C microprocessor") \
X(EM_R32C          , 162, "Renesas R32C series microprocessors") \
X(EM_TRIMEDIA      , 163, "NXP Semiconductors TriMedia architecture family") \
X(EM_QDSP6         , 164, "QUALCOMM DSP6 Processor") \
X(EM_8051          , 165, "Intel 8051 and variants") \
X(EM_STXP7X        , 166, "STMicroelectronics STxP7x family of configurable and extensible RISC processors") \
X(EM_NDS32         , 167, "Andes Technology compact code size embedded RISC processor family") \
X(EM_ECOG1         , 168, "Cyan Technology eCOG1X family") \
X(EM_MAXQ30        , 169, "Dallas Semiconductor MAXQ30 Core Micro-controllers") \
X(EM_XIMO16        , 170, "New Japan Radio (NJR) 16-bit DSP Processor") \
X(EM_MANIK         , 171, "M2000 Reconfigurable RISC Microprocessor") \
X(EM_CRAYNV2       , 172, "Cray Inc. NV2 vector architecture") \
X(EM_RX            , 173, "Renesas RX family") \
X(EM_METAG         , 174, "Imagination Technologies META processor architecture") \
X(EM_MCST_ELBRUS   , 175, "MCST Elbrus general purpose hardware architecture") \
X(EM_ECOG16        , 176, "Cyan Technology eCOG16 family") \
X(EM_CR16          , 177, "National Semiconductor CompactRISC CR16 16-bit microprocessor") \
X(EM_ETPU          , 178, "Freescale Extended Time Processing Unit") \
X(EM_SLE9X         , 179, "Infineon Technologies SLE9X core") \
X(EM_L10M          , 180, "Intel L10M") \
X(EM_K10M          , 181, "Intel K10M") \
X(EM_AARCH64       , 183, "ARM 64-bit architecture (AARCH64)") \
X(EM_AVR32         , 185, "Atmel Corporation 32-bit microprocessor family") \
X(EM_STM8          , 186, "STMicroeletronics STM8 8-bit microcontroller") \
X(EM_TILE64        , 187, "Tilera TILE64 multicore architecture family") \
X(EM_TILEPRO       , 188, "Tilera TILEPro multicore architecture family") \
X(EM_MICROBLAZE    , 189, "Xilinx MicroBlaze 32-bit RISC soft processor core") \
X(EM_CUDA          , 190, "NVIDIA CUDA architecture") \
X(EM_TILEGX        , 191, "Tilera TILE-Gx multicore architecture family") \
X(EM_CLOUDSHIELD   , 192, "CloudShield architecture family") \
X(EM_COREA_1ST     , 193, "KIPO-KAIST Core-A 1st generation processor family") \
X(EM_COREA_2ND     , 194, "KIPO-KAIST Core-A 2nd generation processor family") \
X(EM_ARC_COMPACT2  , 195, "Synopsys ARCompact V2") \
X(EM_OPEN8         , 196, "Open8 8-bit RISC soft processor core") \
X(EM_RL78          , 197, "Renesas RL78 family") \
X(EM_VIDEOCORE5    , 198, "Broadcom VideoCore V processor") \
X(EM_78KOR         , 199, "Renesas 78KOR family") \
X(EM_56800EX       , 200, "Freescale 56800EX Digital Signal Controller (DSC)") \
X(EM_BA1           , 201, "Beyond BA1 CPU architecture") \
X(EM_BA2           , 202, "Beyond BA2 CPU architecture") \
X(EM_XCORE         , 203, "XMOS xCORE processor family") \
X(EM_MCHP_PIC      , 204, "Microchip 8-bit PIC(r) family") \
X(EM_INTEL205      , 205, "Reserved by Intel") \
X(EM_INTEL206      , 206, "Reserved by Intel") \
X(EM_INTEL207      , 207, "Reserved by Intel") \
X(EM_INTEL208      , 208, "Reserved by Intel") \
X(EM_INTEL209      , 209, "Reserved by Intel") \
X(EM_KM32          , 210, "KM211 KM32 32-bit processor") \
X(EM_KMX32         , 211, "KM211 KMX32 32-bit processor") \
X(EM_KMX16         , 212, "KM211 KMX16 16-bit processor") \
X(EM_KMX8          , 213, "KM211 KMX8 8-bit processor") \
X(EM_KVARC         , 214, "KM211 KVARC processor") \
X(EM_CDP           , 215, "Paneve CDP architecture family") \
X(EM_COGE          , 216, "Cognitive Smart Memory Processor") \
X(EM_COOL          , 217, "Bluechip Systems CoolEngine") \
X(EM_NORC          , 218, "Nanoradio Optimized RISC") \
X(EM_CSR_KALIMBA   , 219, "CSR Kalimba architecture family") \
X(EM_Z80           , 220, "Zilog Z80") \
X(EM_VISIUM        , 221, "Controls and Data Services VISIUMcore processor") \
X(EM_FT32          , 222, "FTDI Chip FT32 high performance 32-bit RISC architecture") \
X(EM_MOXIE         , 223, "Moxie processor family") \
X(EM_AMDGPU        , 224, "AMD GPU architecture") \
X(EM_RISCV         , 243, "RISC-V") \
X(EM_BPF           , 247, "Linux BPF - in-kernel virtual machine") \
X(EM_CSKY          , 252, "C-SKY") \
X(EM_FRV           , 0x5441, "Fujitsu FR-V")

#define X(ENUM, NUM, STR) ENUM = NUM,
enum {
ELF_MACH_LIST
};
#undef X

#define ELF_OSABI_LIST \
X(EO_SYSV,      0x00, "System V") \
X(EO_HPUX,      0x01, "HP-UX") \
X(EO_NETBSD,    0x02, "NetBSD") \
X(EO_LINUX,     0x03, "Linux") \
X(EO_HURD,      0x04, "GNU Hurd") \
X(EO_SOLARIS,   0x06, "Solaris") \
X(EO_AIX,       0x07, "AIX (Monterey)") \
X(EO_IRIX,      0x08, "IRIX") \
X(EO_FREEBSD,   0x09, "FreeBSD") \
X(EO_TRU64,     0x0a, "Tru64") \
X(EO_MODESTO,   0x0b, "Novell Modesto") \
X(EO_OPENBSD,   0x0c, "OpenBSD") \
X(EO_OPENVMS,   0x0d, "OpenVMS") \
X(EO_NONSTOP,   0x0e, "NonStop Kernel") \
X(EO_AROS,      0x0f, "AROS") \
X(EO_FENIX,     0x10, "FenixOS") \
X(EO_NUXI,      0x11, "Nuxi CloudABI") \
X(EO_OPENVOS,   0x12, "Stratus Technologies OpenVOS")

#define X86_64_RELOCATIONS \
X(R_X86_64_NONE,        "R_X86_64_NONE",        0,   None) \
X(R_X86_64_64,          "R_X86_64_64",          1,   qword) \
X(R_X86_64_PC32,        "R_X86_64_PC32",        2,   dword) \
X(R_X86_64_GOT32,       "R_X86_64_GOT32",       3,   dword) \
X(R_X86_64_PLT32,       "R_X86_64_PLT32",       4,   dword) \
X(R_X86_64_COPY,        "R_X86_64_COPY",        5,   None) \
X(R_X86_64_GLOB_DAT,    "R_X86_64_GLOB_DAT",    6,   qword) \
X(R_X86_64_JUMP_SLOT,   "R_X86_64_JUMP_SLOT",   7,   qword) \
X(R_X86_64_RELATIVE,    "R_X86_64_RELATIVE",    8,   qword) \
X(R_X86_64_GOTPCREL,    "R_X86_64_GOTPCREL",    9,   dword) \
X(R_X86_64_32,          "R_X86_64_32",          10,  dword) \
X(R_X86_64_32S,         "R_X86_64_32S",         11,  dword) \
X(R_X86_64_16,          "R_X86_64_16",          12,  word) \
X(R_X86_64_PC16,        "R_X86_64_PC16",        13,  word) \
X(R_X86_64_8,           "R_X86_64_8",           14,  word8) \
X(R_X86_64_PC8,         "R_X86_64_PC8",         15,  word8) \
X(R_X86_64_PC64,        "R_X86_64_PC64",        24,  qword) \
X(R_X86_64_GOTOFF64,    "R_X86_64_GOTOFF64",    25,  qword) \
X(R_X86_64_GOTPC32,     "R_X86_64_GOTPC32",     26,  dword) \
X(R_X86_64_SIZE32,      "R_X86_64_SIZE32",      32,  dword) \
X(R_X86_64_SIZE64,      "R_X86_64_SIZE64",      33,  qword) \

#define X(ENUM, STR, NUM, FIELD) ENUM = NUM,
enum {
X86_64_RELOCATIONS
};
#undef X

#define X(ENUM, NUM, STR) ENUM = NUM,
enum {
ELF_OSABI_LIST
};
#undef X

enum {
    PT_NULL,
    PT_LOAD,
    PT_DYNAMIC,
    PT_INTERP,
    PT_NOTE,
    PT_SHLIB,
    PT_PHDR,
    PT_TLS,
    PT_GNU_EH_FRAME = 1685382480,
    PT_GNU_STACK    = 1685382481,
    PT_GNU_RELRO    = 1685382482,
    PT_GNU_PROPERTY = 1685382483,
};

enum {
    SHT_NULL            = 0x0,
    SHT_PROGBITS        = 0x1,
    SHT_SYMTAB          = 0x2,
    SHT_STRTAB          = 0x3,
    SHT_RELA            = 0x4,
    SHT_HASH            = 0x5,
    SHT_DYNAMIC         = 0x6,
    SHT_NOTE            = 0x7,
    SHT_NOBITS          = 0x8,
    SHT_REL             = 0x9,
    SHT_SHLIB           = 0x0A,
    SHT_DYNSYM          = 0x0B,
    SHT_INIT_ARRAY      = 0x0E,
    SHT_FINI_ARRAY      = 0x0F,
    SHT_PREINIT_ARRAY   = 0x10,
    SHT_GROUP           = 0x11,
    SHT_SYMTAB_SHNDX    = 0x12,
    SHT_NUM             = 0x13,
    SHT_LOOS            = 0x60000000,
    SHT_GNU_HASH        = 0x6ffffff6,
    SHT_VERNEED         = 0x6ffffffe,
    SHT_VERSYM          = 0x6fffffff,
};

enum {
    SHF_WRITE               = 0x1,         // Writable
    SHF_ALLOC               = 0x2,         // Occupies memory during execution
    SHF_EXECINSTR           = 0x4,         // Executable
    SHF_MERGE               = 0x10,        // Might be merged
    SHF_STRINGS             = 0x20,        // Contains null-terminated strings
    SHF_INFO_LINK           = 0x40,        // 'sh_info' contains SHT index
    SHF_LINK_ORDER          = 0x80,        // Preserve order after combining
    SHF_OS_NONCONFORMING    = 0x100,       // Non-standard OS specific handling required
    SHF_GROUP               = 0x200,       // Section is member of a group
    SHF_TLS                 = 0x400,       // Section hold thread-local data
    SHF_MASKOS              = 0x0FF00000,  // OS-specific
    SHF_MASKPROC            = 0xF0000000,  // Processor-specific
    SHF_ORDERED             = 0x4000000,   // Special ordering requirement (Solaris)
    SHF_EXCLUDE             = 0x8000000,   // Section is excluded unless referenced or allocated (Solaris) 
};

enum {
    STT_NOTYPE,
    STT_OBJECT,
    STT_FUNC,
    STT_SECTION,
    STT_FILE,
    STT_COMMON,
};

enum {
    SHN_UNDEF   = 0,
    SHN_ABS     = 0xfff1,
};

enum {
    STB_LOCAL,
    STB_GLOBAL,
    STB_WEAK,
};

enum {
    STV_DEFAULT,
    STV_INTERNAL,
    STV_HIDDEN,
    STV_PROTECTED,
};

typedef struct {
    struct {
        uint8_t ei_mag[4];
        uint8_t ei_class;
        uint8_t ei_data;
        uint8_t ei_version;
        uint8_t ei_osabi;
        uint8_t ei_abiversion;
    } e_ident;
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ElfHeader;

typedef struct {
    uint32_t p_type;
    uint32_t p_flags;
    uint64_t p_offset;
    uint64_t p_vaddr;
    uint64_t p_paddr;
    uint64_t p_filesz;
    uint64_t p_memsz;
    uint64_t p_align;
} ElfProgramHeader;

typedef struct {
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} ElfSectionHeader;

typedef struct {
    uint32_t st_name;
    uint8_t  st_info;
    uint8_t  st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
} ElfSymbolTable;

typedef struct {
    bool valid;
    ElfErr error;
    ElfHeader header;
    ElfProgramHeader * program_headers;
    ElfSectionHeader * section_headers;
    char ** segments;
    char ** sections;
} Elf;

const char * elferrstr(ElfErr errnum);
Elf read_elf(const char * filename);
void destroy_elf(Elf elf);
void print_elf(Elf elf, FILE * fp);

#endif /* ELF_H */
