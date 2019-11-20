#ifndef BREAKPOINT_H
#define BREAKPOINT_H

#include <stdint.h>
#include <unistd.h>
//#include <sys/types.h>

class Breakpoint
{
public:
    Breakpoint(pid_t inPid, uint64_t inAddr);
    Breakpoint();
    void enable();
    void disable();

    bool isEnabled() const { return enabled; }
    uint64_t getAddress() const { return addr; }

private:
    pid_t pid;
    uint64_t addr;
    bool enabled;
    uint8_t savedByte;
};

#endif // BREAKPOINT_H
