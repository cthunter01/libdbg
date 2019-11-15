#ifndef DEBUGGER_H
#define DEBUGGER_H

#include <string>
#include <ext/hash_map>
#include <sys/types.h>

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

private:
    std::string progName;
    pid_t pid;
    hash_map<uint64_t, Breakpoint> breakpointMap;
    hash_map<CPURegisterEnum::CPURegister, CPURegisterDescriptor> registerDescriptors;
};

#endif // DEBUGGER_H
