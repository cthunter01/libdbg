#ifndef ELFFILE_H_
#define ELFFILE_H_

#include "BinaryFile.h"

#include <vector>
#include <stdint.h>

// Make sure these structs are 1 byte aligned. Don't let compiler insert
// extraneous padding. I tested this with and without the pragma and it doesn't
// change the size, but I'm leaving this in to be sure.
#pragma pack(push)
#pragma pack(1)
typedef struct Elf64FileHeader
{
    unsigned char ei_magic[4];
    uint8_t ei_class;
    uint8_t ei_data;
    uint8_t ei_version;
    uint8_t ei_osabi;
    uint8_t ei_abiversion;
    unsigned char ei_padding[7]; // padding. unused
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
} Elf64FileHeader;

typedef struct Elf64ProgramHeader
{
	uint32_t p_type;
	uint32_t p_flags;
	uint64_t p_offset;
	uint64_t p_vaddr;
	uint64_t p_paddr;
	uint64_t p_filesz;
	uint64_t p_memsz;
	uint64_t p_align;
} Elf64ProgramHeader;

typedef struct Elf64SectionHeader
{
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
} Elf64SectionHeader;

typedef struct Elf64Section
{
	Elf64SectionHeader* sectionHeader;
	void* data;
} Elf64Section;

typedef struct Elf64Segment
{
	Elf64ProgramHeader* segmentHeader;
	void* data;
} Elf64Segment;
#pragma pack(pop)

class Elf64File : public BinaryFile
{
public:
	Elf64File(const std::string& inFileName);
	virtual ~Elf64File() { }

	//std::vector<Elf64Segment> getSegments() { return segments; }

//private:

	Elf64FileHeader* fileHeader;
	std::vector<Elf64Segment> segments;
	std::vector<Elf64Section> sections;


};

#endif /* ELFFILE_H_ */
