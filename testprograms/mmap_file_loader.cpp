#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>

// Make sure these structs are 1 byte aligned. Don't let compiler insert
// extraneous padding. I tested this with and without the pragma and it doesn't
// change the size, but I'm leaving this in to be sure.
#pragma pack(push)
#pragma pack(1)
typedef struct Elf64FileHeader
{
    unsigned char ei_magic[4];
    unsigned char ei_class;
    unsigned char ei_data;
    unsigned char ei_version;
    unsigned char ei_osabi;
    unsigned char ei_abiversion;
    unsigned char ei_padding[7]; // padding. unused
    unsigned char e_type[2];
    unsigned char e_machine[2];
    unsigned char e_version[4];
    unsigned char e_entry[8];
    unsigned char e_phoff[8];
    unsigned char e_shoff[8];
    unsigned char e_flags[4];
    unsigned char e_ehsize[2];
    unsigned char e_phentsize[2];
    unsigned char e_phnum[2];
    unsigned char e_shentsize[2];
    unsigned char e_shnum[2];
    unsigned char e_shstrndx[2];
} Elf64FileHeader;

typedef struct Elf64ProgramHeader
{
    unsigned char p_type[4];
    unsigned char p_flags[4];
    unsigned char p_offset[8];
    unsigned char p_vaddr[8];
    unsigned char p_paddr[8];
    unsigned char p_filesz[8];
    unsigned char p_memsz[8];
    unsigned char p_align[8];
} Elf64ProgramHeader;

typedef struct Elf64SectionHeader
{
    unsigned char sh_name[4];
    unsigned char sh_type[4];
    unsigned char sh_flags[8];
    unsigned char sh_addr[8];
    unsigned char sh_offset[8];
    unsigned char sh_size[8];
    unsigned char sh_link[4];
    unsigned char sh_info[4];
    unsigned char sh_addralign[8];
    unsigned char sh_entsize[8];
} Elf64SectionHeader;
#pragma pack(pop)

struct ElfSection
{

    ElfSection(const Elf& elf, const void* hdr)
    {


    Elf64SectionHeader hdr;
    const void* data;

    const Elf64SectionHeader& getHeader() const
    {
        return hdr;
    }

    const std::string getName() const
    {
        return std::string(this->sh_name);
    }
private:
    ElfSection() { }
}
struct Elf
{
public:
    explicit Elf(int inFd);

    ~Elf() { }

private:
    int fd;

    Elf64FileHeader fileHeader;
    Elf64ProgramHeader programHeader;

    std::vector
    void* basePtr;
};

Elf::Elf(int inFd) : fd(inFd), basePtr(NULL)
{

}

const void* load(void* basePtr, int offset)
{
    return (const char*)basePtr + offset;
}

int main(int argc, char *argv[])
{
    std::cout << std::hex << "sizeof Elf64FileHeader: " << sizeof(Elf64FileHeader) << std::endl;
    std::cout << std::hex << "sizeof Elf64ProgramHeader: " << sizeof(Elf64ProgramHeader) << std::endl;
    std::cout << std::hex << "sizeof Elf64SectionHeader: " << sizeof(Elf64SectionHeader) << std::endl;

    // Open the file
    int fd = open(argv[1], O_RDONLY);

    // map the file into memory

    off_t end = lseek(fd, 0, SEEK_END);
    size_t lim = end;
    std::cout << "lim: " << lim << std::endl;
    void *fileBasePtr = mmap(NULL, lim, PROT_READ, MAP_SHARED, fd, 0);
    if (fileBasePtr == MAP_FAILED)
        std::cerr << "Error mmap'ing file\n";

    // Let's try populating these headers with info from the read-in file
    //const Elf64FileHeader* fileHeader = reinterpret_cast<const Elf64FileHeader*>(load(fileBasePtr, 0));
    const Elf64FileHeader* fileHeader = (const Elf64FileHeader*) fileBasePtr;
    std::cout << "Magic: " << fileHeader->ei_magic << std::endl;
    std::cout << "Class: " << (int)fileHeader->ei_class << std::endl;
    std::cout << "OSABI: " << (int)fileHeader->ei_osabi << std::endl;
    std::cout << "Section Num: " << std::dec << (int)(*fileHeader->e_shnum) << std::endl;
    std::cout << "Section Header Size: " << std::dec << (int)(*fileHeader->e_ehsize) << std::endl;

    munmap(fileBasePtr, lim);
    close(fd);
    return 0;
}
