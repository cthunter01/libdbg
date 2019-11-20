#include "ElfFile.h"

Elf64File::Elf64File(const std::string& inFileName) :
	BinaryFile(inFileName),
	fileHeader(NULL),
	segments(),
	sections()
{
	// Naively assume that the file was read in without error...

	fileHeader = reinterpret_cast<Elf64FileHeader*>(retrieve(0, sizeof(Elf64FileHeader)));

	// Read in segments (program header table)
	for(unsigned int i = 0; i < fileHeader->e_phnum; ++i)
	{
		Elf64Segment seg;
		off_t baseOffset = fileHeader->e_phoff + i * fileHeader->e_phentsize;
		seg.segmentHeader = reinterpret_cast<Elf64ProgramHeader*>(
				retrieve(baseOffset, fileHeader->e_phentsize));
		segments.push_back(seg);
	}

	// Read in sections (Section Header table)
	for(unsigned int i = 0; i < fileHeader->e_shnum; ++i)
	{
		Elf64Section sec;
		off_t baseOffset = fileHeader->e_shoff + i * fileHeader->e_shentsize;
		sec.sectionHeader = reinterpret_cast<Elf64SectionHeader*>(
				retrieve(baseOffset, fileHeader->e_shentsize));
		sections.push_back(sec);
	}


}
