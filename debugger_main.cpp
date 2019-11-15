#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>

#include "Breakpoint.h"
#include "Debugger.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Program name not specified" << std::endl;
        return -1;
    }

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

