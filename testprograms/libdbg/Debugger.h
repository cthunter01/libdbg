#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <ext/hash_map>
#include <sys/types.h>
#include <map>

#include "Breakpoint.h"
#include "CPURegisterEnum.h"
#include "CPURegisterDescriptor.h"

using __gnu_cxx::hash_map;

class Debugger {
public:
    Debugger(const std::string& inProgName, const pid_t& inPid);
    void run();
    int handleCommand(const std::string& line);
    void setBreakpointAt(uint64_t address);
    void continueExecution();

    typedef std::pair<std::string, std::string> CommandPairType;

    uint64_t getRegisterValue(const CPURegisterEnum::CPURegister reg);
    void setRegisterValue(const CPURegisterEnum::CPURegister reg, uint64_t value);
    void dumpRegisters();

private:
    std::string progName;
    pid_t pid;
    hash_map<uint64_t, Breakpoint> breakpointMap;
    std::map<std::string, CPURegisterDescriptor> registerDescriptors;

    void waitForSignal();
    void stepOverBreakpoint();
    std::vector<std::string> readCommandFile(const std::string& inFileName = "commandfile.txt");
};

#endif // DEBUGGER_H
