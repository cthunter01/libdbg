#include <stdint.h>
#include <unistd.h>
#include <sys/ptrace.h>
//#include <sys/types.h>

#include "Breakpoint.h"

Breakpoint::Breakpoint(pid_t inPid, uint64_t inAddr)
    : pid(inPid),
      addr(inAddr),
      enabled(false),
      savedByte()
{ }

Breakpoint::Breakpoint()
    : pid(),
      addr(),
      enabled(false),
      savedByte()
{ }

void Breakpoint::enable()
{
    long data = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
    savedByte = static_cast<uint8_t>(data & 0xff);
    uint64_t int3 = 0xcc;
    uint64_t dataWithInt3 = ((data & ~0xff) | int3);
    ptrace(PTRACE_POKEDATA, pid, addr, dataWithInt3);
    enabled = true;
}

void Breakpoint::disable()
{
    long data = ptrace(PTRACE_PEEKDATA, pid, addr, NULL);
    uint64_t restoredData = ((data & ~0xff) | savedByte);
    ptrace(PTRACE_POKEDATA, pid, addr, restoredData);

    enabled=false;
}

