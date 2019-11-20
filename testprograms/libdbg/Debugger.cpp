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
      breakpointMap(),
      registerDescriptors()
{
    // The DWARF register numbers are taken from the System V x86_64 ABI document
    registerDescriptors["r15"] = CPURegisterDescriptor(CPURegisterEnum::r15, 15, "r15");
    registerDescriptors["r14"] = CPURegisterDescriptor(CPURegisterEnum::r14, 14, "r14");
    registerDescriptors["r13"] = CPURegisterDescriptor(CPURegisterEnum::r13, 13, "r13");
    registerDescriptors["r12"] = CPURegisterDescriptor(CPURegisterEnum::r12, 12, "r12");
    registerDescriptors["rbp"] = CPURegisterDescriptor(CPURegisterEnum::rbp, 6, "rbp");
    registerDescriptors["rbx"] = CPURegisterDescriptor(CPURegisterEnum::rbx, 3, "rbx");
    registerDescriptors["r11"] = CPURegisterDescriptor(CPURegisterEnum::r11, 11, "r11");
    registerDescriptors["r10"] = CPURegisterDescriptor(CPURegisterEnum::r10, 10, "r10");
    registerDescriptors["r9"] = CPURegisterDescriptor(CPURegisterEnum::r9, 9, "r9");
    registerDescriptors["r8"] = CPURegisterDescriptor(CPURegisterEnum::r8, 8, "r8");
    registerDescriptors["rax"] = CPURegisterDescriptor(CPURegisterEnum::rax, 0, "rax");
    registerDescriptors["rcx"] = CPURegisterDescriptor(CPURegisterEnum::rcx, 2, "rcx");
    registerDescriptors["rdx"] = CPURegisterDescriptor(CPURegisterEnum::rdx, 1, "rdx");
    registerDescriptors["rsi"] = CPURegisterDescriptor(CPURegisterEnum::rsi, 4, "rsi");
    registerDescriptors["rdi"] = CPURegisterDescriptor(CPURegisterEnum::rdi, 5, "rdi");
    registerDescriptors["orig_rax"] = CPURegisterDescriptor(CPURegisterEnum::orig_rax, -1, "orig_rax");
    registerDescriptors["rip"] = CPURegisterDescriptor(CPURegisterEnum::rip, -1, "rip");
    registerDescriptors["cs"] = CPURegisterDescriptor(CPURegisterEnum::cs, 51, "cs");
    registerDescriptors["eflags"] = CPURegisterDescriptor(CPURegisterEnum::eflags, 49, "eflags");
    registerDescriptors["rsp"] = CPURegisterDescriptor(CPURegisterEnum::rsp, 7, "rsp");
    registerDescriptors["ss"] = CPURegisterDescriptor(CPURegisterEnum::ss, 52, "ss");
    registerDescriptors["fs_base"] = CPURegisterDescriptor(CPURegisterEnum::fs_base, 58, "fs_base");
    registerDescriptors["gs_base"] = CPURegisterDescriptor(CPURegisterEnum::gs_base, 59, "gs_base");
    registerDescriptors["ds"] = CPURegisterDescriptor(CPURegisterEnum::ds, 53, "ds");
    registerDescriptors["es"] = CPURegisterDescriptor(CPURegisterEnum::es, 50, "es");
    registerDescriptors["fs"] = CPURegisterDescriptor(CPURegisterEnum::fs, 54, "fs");
    registerDescriptors["gs"] = CPURegisterDescriptor(CPURegisterEnum::gs, 55, "gs");
}

void Debugger::run()
{
    waitForSignal();

    // Read command file
    std::vector<std::string> commandLines = readCommandFile("commandFile.txt");

    // handle commands
    for(std::vector<std::string>::iterator i = commandLines.begin(); i != commandLines.end(); ++i)
    {
        handleCommand(*i);
    }

    // (re)start the debuggee and open a command prompt for interactive use
    ptrace(PTRACE_CONT, pid, 0, 0);

    std::string input;
    std::cout << "ldbg> ";
    std::getline(std::cin, input);
    while(handleCommand(input) != -1)
    {
        std::cout << "ldbg> ";
        std::getline(std::cin, input);
    }

}

void Debugger::continueExecution()
{
    stepOverBreakpoint();
    ptrace(PTRACE_CONT, pid, 0, 0);
    waitForSignal();
}

int Debugger::handleCommand(const std::string& line)
{
    std::cout << "Received command: " << line << std::endl;
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
    else if(is_prefix(command, "register"))
    {
        if(args[1] == "read")
        {
            uint64_t val = getRegisterValue(registerDescriptors[args[1]].regEnum);
            std::cout << val << std::endl;
        }
        if(args[1] == "write")
        {
            char* end;
            std::string valToWrite(args[3], 2, std::string::npos);
            setRegisterValue(registerDescriptors[args[2]].regEnum, std::strtol(valToWrite.c_str(), &end, 16));
        }
        if(args[1] == "dump")
        {
            dumpRegisters();
        }
    }
    else
    {
        std::cerr << "Unknown command: " << command << std::endl;
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

uint64_t Debugger::getRegisterValue(const CPURegisterEnum::CPURegister reg)
{
    // See <sys/user.h> for the definition of this struct
    user_regs_struct registers;
    ptrace(PTRACE_GETREGS, pid, NULL, &registers);

    switch(reg)
    {
        case CPURegisterEnum::r15:
            return registers.r15;

        case CPURegisterEnum::r14:
            return registers.r14;

        case CPURegisterEnum::r13:
            return registers.r13;

        case CPURegisterEnum::r12:
            return registers.r12;

        case CPURegisterEnum::rbp:
            return registers.rbp;

        case CPURegisterEnum::rbx:
            return registers.rbx;

        case CPURegisterEnum::r11:
            return registers.r11;

        case CPURegisterEnum::r10:
            return registers.r10;

        case CPURegisterEnum::r9:
            return registers.r9;

        case CPURegisterEnum::r8:
            return registers.r8;

        case CPURegisterEnum::rax:
            return registers.rax;

        case CPURegisterEnum::rcx:
            return registers.rcx;

        case CPURegisterEnum::rdx:
            return registers.rdx;

        case CPURegisterEnum::rsi:
            return registers.rsi;

        case CPURegisterEnum::rdi:
            return registers.rdi;

        case CPURegisterEnum::orig_rax:
            return registers.orig_rax;

        case CPURegisterEnum::rip:
            return registers.rip;

        case CPURegisterEnum::cs:
            return registers.cs;

        case CPURegisterEnum::eflags:
            return registers.eflags;

        case CPURegisterEnum::rsp:
            return registers.rsp;

        case CPURegisterEnum::ss:
            return registers.ss;

        case CPURegisterEnum::fs_base:
            return registers.fs_base;

        case CPURegisterEnum::gs_base:
            return registers.gs_base;

        case CPURegisterEnum::ds:
            return registers.ds;

        case CPURegisterEnum::es:
            return registers.es;

        case CPURegisterEnum::fs:
            return registers.fs;

        case CPURegisterEnum::gs:
            return registers.gs;

        default:
            std::cout << "Unknown Register" << std::endl;
            return 0;
    }
}

void Debugger::setRegisterValue(const CPURegisterEnum::CPURegister reg, uint64_t value)
{
    // See <sys/user.h> for the definition of this struct
    user_regs_struct registers;
    ptrace(PTRACE_GETREGS, pid, NULL, &registers);

    switch(reg)
    {
        case CPURegisterEnum::r15:
            registers.r15 = value;
            break;

        case CPURegisterEnum::r14:
            registers.r14 = value;
            break;

        case CPURegisterEnum::r13:
            registers.r13 = value;
            break;

        case CPURegisterEnum::r12:
            registers.r12 = value;
            break;

        case CPURegisterEnum::rbp:
            registers.rbp = value;
            break;

        case CPURegisterEnum::rbx:
            registers.rbx = value;
            break;

        case CPURegisterEnum::r11:
            registers.r11 = value;
            break;

        case CPURegisterEnum::r10:
            registers.r10 = value;
            break;

        case CPURegisterEnum::r9:
            registers.r9 = value;
            break;

        case CPURegisterEnum::r8:
            registers.r8 = value;
            break;

        case CPURegisterEnum::rax:
            registers.rax = value;
            break;

        case CPURegisterEnum::rcx:
            registers.rcx = value;
            break;

        case CPURegisterEnum::rdx:
            registers.rdx = value;
            break;

        case CPURegisterEnum::rsi:
            registers.rsi = value;
            break;

        case CPURegisterEnum::rdi:
            registers.rdi = value;
            break;

        case CPURegisterEnum::orig_rax:
            registers.orig_rax = value;
            break;

        case CPURegisterEnum::rip:
            registers.rip = value;
            break;

        case CPURegisterEnum::cs:
            registers.cs = value;
            break;

        case CPURegisterEnum::eflags:
            registers.eflags = value;
            break;

        case CPURegisterEnum::rsp:
            registers.rsp = value;
            break;

        case CPURegisterEnum::ss:
            registers.ss = value;
            break;

        case CPURegisterEnum::fs_base:
            registers.fs_base = value;
            break;

        case CPURegisterEnum::gs_base:
            registers.gs_base = value;
            break;

        case CPURegisterEnum::ds:
            registers.ds = value;
            break;

        case CPURegisterEnum::es:
            registers.es = value;
            break;

        case CPURegisterEnum::fs:
            registers.fs = value;
            break;

        case CPURegisterEnum::gs:
            registers.gs = value;
            break;

        default:
            break;
    }
    ptrace(PTRACE_SETREGS, pid, NULL, &registers);
}

void Debugger::dumpRegisters()
{

    for(std::map<std::string, CPURegisterDescriptor>::iterator i = registerDescriptors.begin();
        i != registerDescriptors.end();
        ++i)
    {
        std::cout << i->second.name << ": 0x" << getRegisterValue(i->second.regEnum) << std::endl;
    }
}

void Debugger::waitForSignal()
{
    int waitStatus(0);
    int options(0);
    waitpid(pid, &waitStatus, options);
}

void Debugger::stepOverBreakpoint()
{
    uint64_t bpLoc = getRegisterValue(CPURegisterEnum::rip) - 1;

    if(breakpointMap.find(bpLoc) != breakpointMap.end())
    {
        Breakpoint& bp = breakpointMap[bpLoc];
        if(bp.isEnabled())
        {
            setRegisterValue(CPURegisterEnum::rip, bpLoc);
            bp.disable();
            ptrace(PTRACE_SINGLESTEP, pid, NULL, NULL);
            waitForSignal();
            bp.enable();
        }
    }
}

std::vector<std::string> Debugger::readCommandFile(const std::string& inFileName)
{

    std::ifstream inFile("commandfile.txt", std::fstream::in);
    std::vector<std::string> commandLines;

    std::string line;

    // get all command lines
    while(std::getline(inFile, line))
    {
        commandLines.push_back(line);
    }
    inFile.close();

    std::cout << "Read " << commandLines.size() << " lines\n";
    return commandLines;
}
