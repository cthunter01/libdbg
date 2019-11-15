// Class Declaration
#include "Debugger.h"

// System Includes
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <fstream>
#include <utility>



Debugger::Debugger(const std::string& inProgName, const pid_t& inPid)
    : progName(inProgName),
      pid(inPid),
      breakpointMap()
{ }

void Debugger::run()
{
    int wait_status(0);
    int options(0);
    waitpid(pid, &wait_status, options);

    // Read command file
    std::ifstream inFile("commandfile.txt", std::fstream::in);

    typedef std::pair<std::string, std::string> CommandPairType;
    CommandPairType commandPair;
    std::vector<CommandPairType> commands;
    std::vector<std::string> commandLines;

    std::string line;

    // get all command lines
    while(std::getline(inFile, line))
    {
        commandLines.push_back(line);
    }
    inFile.close();

    std::cout << "Read " << commandLines.size() << " lines\n";

    for(std::vector<std::string>::iterator i = commandLines.begin(); i != commandLines.end(); ++i)
    {
        std::stringstream ls(*i);
        std::string token;
        std::vector<std::string> tokens;
        while(std::getline(ls, token, ' '))
        {
            tokens.push_back(token);
        }
        if(2 == tokens.size())
        {
            CommandPairType command = std::make_pair(tokens[0], tokens[1]);
            commands.push_back(command);
        }
    }

    // handle commands
    for(std::vector<std::string>::iterator i = commandLines.begin(); i != commandLines.end(); ++i)
    {
        handleCommand(*i);
    }

    std::string input;
    while(handleCommand(input) != -1)
    {
        std::cout << "ldbg> ";
        std::getline(std::cin, input);
    }

}

void Debugger::continueExecution()
{
    ptrace(PTRACE_CONT, pid, 0, 0);
    int waitStatus(0);
    int options(0);
    waitpid(pid, &waitStatus, options);
}

int Debugger::handleCommand(const std::string& line)
{
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> args;
    while(std::getline(ss, item, ' '))
    {
        args.push_back(item);
    }
    if(!args.empty())
    {
    std::string command = args[0];
    
    struct
    {
        bool operator()(const std::string& s, const std::string& of)
        {
            if(s.size() > of.size())
                return false;
            return std::equal(s.begin(), s.end(), of.begin());
        }
    } is_prefix;

    if(is_prefix(command, "continue"))
    {
        continueExecution();
    }
    else if(is_prefix(command, "quit"))
    {
        return -1;
    }
    else if(is_prefix(command, "break"))
    {
        std::string addr(args[1], 2, std::string::npos);
        char* end;
        setBreakpointAt(std::strtol(addr.c_str(), &end, 16));
    }
    else
    {
        std::cerr << "Unknown command" << std::endl;
    }
    }
    return 0;
}

void Debugger::setBreakpointAt(uint64_t address)
{
    std::cout << "Set breakpoint at address 0x" << std::hex << address << std::endl;
    Breakpoint bp(pid, address);
    bp.enable();
    breakpointMap[address] = bp;
}
