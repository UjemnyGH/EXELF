#ifndef __ELF_FORMAT_
#define __ELF_FORMAT_

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define ELF_TYPE_NONE           0x0
#define ELF_TYPE_RELOCATABLE    0x1
#define ELF_TYPE_EXECUTABLE     0x2
#define ELF_TYPE_SHADERD_OBJECT 0x3
#define ELF_TYPE_CORE_FILE      0x4
// Dont know what is this for
#define ELF_TYPE_LOOS           0xfe00
#define ELF_TYPE_HIOS           0xfeff
#define ELF_TYPE_LOPROC         0xff00
#define ELF_TYPE_HIPROC         0xffff

#define ELF_MACHINE_X86         0x3
#define ELF_MACHINE_IA_64       0x32
#define ELF_MACHINE_AMD_X86_64  0x3e
#define ELF_MACHINE_ARM_V8      0xb7

typedef struct ELFHeader_s {
    uint8_t magicNumber[4]; // 0x7f + "ELF"
    uint8_t bitFormat; // 32 (1) or 64 (2) bit format
    uint8_t dataEndiness; // little (1) or big (2) endian
    uint8_t version; // always set to 1
    uint8_t osAbi; // usually 0x3 (Linux) 
    uint8_t abiVersion; // idk?
    uint8_t padding[7]; // useless?
    uint16_t type;
    uint16_t machine; // 0x3 (x86), 0x3e (AMD x86-64) or 0x32 (IA-64), I don`t think we use Z80 as linux capable cpu, could be also 0xb7 (Arm 64 bit (like Arm v8/AArch64))
    uint32_t anotherVersion; // set to 1
    uint64_t entry; // memory address of entry point, process execution start
    uint64_t programHeaderPointer; // program header table (usually 0x40 or 0x34 (depends on bitFormat))
    uint64_t sectionHeaderPointer; // section header table pointer
    uint32_t flags; // "Interpretation of this field depends on the target architecture." - Wikipedia 2024
    uint16_t elfHeaderSize; // 64 (64 bit format) or 52 (32 bit format)
    uint16_t programHeaderTableSize; // as name sugest, could be 0x20 (32 bit arch) or 0x38 (64 bit arch)
    uint16_t programHeaderEntries; // contains the number of entries in the program header table
    uint16_t sectionHeaderTableSize; // section header size 0x28 (32 bit) or 0x40 (64 bit)
    uint16_t sectionHeaderEntries; // contains the number of entries in the section header table
    uint16_t sectionNamesIndex; // contains index of the section header table entry that contains the section names
    // end 0x40 at max
} ELFHeader_t;

#define ELF_PROG_FLAGS_EXECUTABLE   0x1
#define ELF_PROG_FLAGS_WRITABLE     0x2
#define ELF_PROG_FLAGS_READABLE     0x4

typedef struct ELFProgramHeader_s {
    uint32_t type; // segment type
    uint32_t flags64; // segment dependent flags (does not exist in 32 bit mode in this place)
    uint64_t segmentOffset; // offset of the segment in the file image
    uint64_t segmentVirtualAddress; // virtual address of the segment in memory
    uint64_t segmentPhysicalAddress; // usually didn`t used
    uint64_t fileImageSize; // size in bytes of the segment in the file image (may be 0)
    uint64_t memorySize; // size in bytes of the segment in memory (may be 0)
    uint32_t flags32; // segment dependent flags (does not exist in 64 bit mode in this place)
    uint64_t alignment; // please be 0 or 1, I don`t want to align stuff!
    // end 0x38 at max
} ELFProgramHeader_t;

// Section header table entry unused
#define ELF_SECT_TYPE_NULL              0x0 
// Program data
#define ELF_SECT_TYPE_PROGBITS          0x1 
// Symbol table
#define ELF_SECT_TYPE_SYMTAB            0x2 
// String table
#define ELF_SECT_TYPE_STRTAB            0x3 
// Relocation entries with addends
#define ELF_SECT_TYPE_RELA              0x4 
// Symbol hash table
#define ELF_SECT_TYPE_HASH              0x5 
// Dynamic linking information
#define ELF_SECT_TYPE_DYNAMIC           0x6 
// Notes
#define ELF_SECT_TYPE_NOTE              0x7 
// Program space with no data (bss)
#define ELF_SECT_TYPE_NOBITS            0x8 
// Relocation entries, no addends
#define ELF_SECT_TYPE_REL               0x9 
// Reserved
#define ELF_SECT_TYPE_SHLIB             0xA
// Dynamic linker symbol table
#define ELF_SECT_TYPE_DYNSYM            0xB
// Array of constructors
#define ELF_SECT_TYPE_INIT_ARRAY        0xE
// Array of destructors
#define ELF_SECT_TYPE_FINI_ARRAY        0xF
// Array of pre-constructors
#define ELF_SECT_TYPE_PREINIT_ARRAY     0x10
// Section group
#define ELF_SECT_TYPE_GROUP             0x11
// Extended section indices
#define ELF_SECT_TYPE_SYMTAB_SHNDX      0x12
// Number of defined types. 
#define ELF_SECT_TYPE_NUM               0x13

// Writable
#define ELF_SECT_FLAG_WRITE               0x1
// Occupies memory during execution
#define ELF_SECT_FLAG_ALLOC               0x2
// Executable
#define ELF_SECT_FLAG_EXECINSTR           0x4
// Might be merged
#define ELF_SECT_FLAG_MERGE               0x10
// Contains null-terminated strings
#define ELF_SECT_FLAG_STRINGS             0x20
// 'sh_info' contains SHT index
#define ELF_SECT_FLAG_INFO_LINK           0x40
// Preserve order after combining
#define ELF_SECT_FLAG_LINK_ORDER          0x80
// Non-standard OS specific handling required
#define ELF_SECT_FLAG_OS_NONCONFORMING    0x100
// Section is member of a group
#define ELF_SECT_FLAG_GROUP               0x200
// Section hold thread-local data
#define ELF_SECT_FLAG_TLS                 0x400
// OS-specific
#define ELF_SECT_FLAG_MASKOS              0x0FF00000
// Processor-specific
#define ELF_SECT_FLAG_MASKPROC            0xF0000000
// Special ordering requirement (Solaris)
#define ELF_SECT_FLAG_ORDERED             0x4000000
// Section is excluded unless referenced or allocated (Solaris) 
#define ELF_SECT_FLAG_EXCLUDE             0x8000000

typedef struct ELFSectionHeader_s {
    uint32_t name; // offset to string in .shstrtab section
    uint32_t type; // type of header
    uint64_t flags;
    uint64_t memoryVirtualAddress;
    uint64_t fileImageSectionOffset;
    uint64_t sectionSize;
    uint32_t sectionIndex; // sh_link
    uint32_t sectionInfo;
    uint64_t sectionAlign;
    uint64_t sectionEntrySize;
} ELFSectionHeader_t;

typedef struct ELFData_s {
    ELFHeader_t elfHeader;
    ELFProgramHeader_t programHeader;
    ELFSectionHeader_t sectionHeaders[256];
    uint8_t *data;
    uint64_t dataSize;
} ELFData_t;

void LoadELFData(ELFData_s* elf, const char* path) {
    FILE* f = fopen(path, "rb+");

    fseek(f, 0, 2);
    elf->dataSize = ftell(f);
    fseek(f, 0, 0);

    elf->data = (uint8_t*)malloc(elf->dataSize);

    fread(elf->data, 1, elf->dataSize, f);

    fseek(f, 0, 0);

    elf->elfHeader.magicNumber[0] = fgetc(f);
    elf->elfHeader.magicNumber[1] = fgetc(f);
    elf->elfHeader.magicNumber[2] = fgetc(f);
    elf->elfHeader.magicNumber[3] = fgetc(f);

    elf->elfHeader.bitFormat = fgetc(f);
    elf->elfHeader.dataEndiness = fgetc(f);
    elf->elfHeader.version = fgetc(f);
    elf->elfHeader.osAbi = fgetc(f);
    elf->elfHeader.abiVersion = fgetc(f);
    elf->elfHeader.type = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.machine = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.anotherVersion = fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;

    // '1' = 32 bit file format 
    if(elf->elfHeader.bitFormat == 1) {
        elf->elfHeader.entry = fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;
        elf->elfHeader.programHeaderPointer = fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;
        elf->elfHeader.sectionHeaderPointer = fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;
    }
    else {
        elf->elfHeader.entry = (uint64_t)fgetc(f) | (uint64_t)fgetc(f) << 8 | (uint64_t)fgetc(f) << 16 | (uint64_t)fgetc(f) << 24 | (uint64_t)fgetc(f) << 32 | (uint64_t)fgetc(f) << 40 | (uint64_t)fgetc(f) << 48 | (uint64_t)fgetc(f) << 56;
        elf->elfHeader.programHeaderPointer = (uint64_t)fgetc(f) | (uint64_t)fgetc(f) << 8 | (uint64_t)fgetc(f) << 16 | (uint64_t)fgetc(f) << 24 | (uint64_t)fgetc(f) << 32 | (uint64_t)fgetc(f) << 40 | (uint64_t)fgetc(f) << 48 | (uint64_t)fgetc(f) << 56;
        elf->elfHeader.sectionHeaderPointer = (uint64_t)fgetc(f) | (uint64_t)fgetc(f) << 8 | (uint64_t)fgetc(f) << 16 | (uint64_t)fgetc(f) << 24 | (uint64_t)fgetc(f) << 32 | (uint64_t)fgetc(f) << 40 | (uint64_t)fgetc(f) << 48 | (uint64_t)fgetc(f) << 56;
    }

    elf->elfHeader.flags = fgetc(f) | fgetc(f) << 8 | fgetc(f) << 16 | fgetc(f) << 24;
    elf->elfHeader.elfHeaderSize = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.programHeaderTableSize = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.programHeaderEntries = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.sectionHeaderTableSize = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.sectionHeaderEntries = fgetc(f) | fgetc(f) << 8;
    elf->elfHeader.sectionNamesIndex = fgetc(f) | fgetc(f) << 8;
}

#endif