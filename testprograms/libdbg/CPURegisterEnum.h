#ifndef CPUREGISTERENUM_H_
#define CPUREGISTERENUM_H_

#include <sys/user.h>

class CPURegisterEnum
{
public:
    CPURegisterEnum() { }

    ~CPURegisterEnum() { }

    // Copied from <sys/user.h>
    enum CPURegister
    {
        r15,
        r14,
        r13,
        r12,
        rbp,
        rbx,
        r11,
        r10,
        r9,
        r8,
        rax,
        rcx,
        rdx,
        rsi,
        rdi,
        orig_rax,
        rip,
        cs,
        eflags,
        rsp,
        ss,
        fs_base,
        gs_base,
        ds,
        es,
        fs,
        gs,
        NUMREGS
    };

};



#endif //CPUREGISTERENUM_H
