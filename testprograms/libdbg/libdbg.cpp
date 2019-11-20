#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>

#include "Breakpoint.h"
#include "Debugger.h"

#include "ElfFile.h"

void testFunction(int argc, char* argv[]);
int main(int argc, char *argv[])
{

    if (argc < 2)
    {
        std::cerr << "Program name not specified" << std::endl;
        return -1;
    }

    testFunction(argc, argv);

    pid_t pid = fork();
    if(pid == 0)
    {
        // we're in the child process
        ptrace(PTRACE_TRACEME, 0, NULL, NULL);
        //return execv(argv[1], &argv[1]);
        execv(argv[1], &argv[1]);
    }
    else if( pid >= 1)
    {
        // We're in the parent process
        // Run the debugger

        std::cout << "Debugger attaching to process pid: " << pid << std::endl;
        std::string progName(argv[1]);
        Debugger dbg(progName, pid);
        dbg.run();
    }


    return 0;
}

void testFunction(int argc, char* argv[])
{
	Elf64File efile(argv[1]);

    std::cout << "Magic: " << efile.fileHeader->ei_magic << std::endl;
    std::cout << "Class: " << efile.fileHeader->ei_class << std::endl;
    std::cout << "OSABI: " << efile.fileHeader->ei_osabi << std::endl;
    std::cout << "Section Num: " << std::dec << efile.fileHeader->e_shnum << std::endl;
    std::cout << "Section Header Size: " << std::dec << efile.fileHeader->e_ehsize << std::endl;

	std::cout << "Testing Elf file logic" << std::endl;
	for(unsigned int i = 0; i < efile.sections.size(); ++i)
	{
		std::cout << (char*)efile.retrieve(1 + efile.sections[i].sectionHeader->sh_name + efile.fileHeader->e_shstrndx, 10) << std::endl;
	}

}
