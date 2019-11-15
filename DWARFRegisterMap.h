#ifndef DWARFREGISTERENUM_H
#define DWARFREGISTERENUM_H

#include "CPURegisterEnum.h"

class DWARFRegisterMap
{
public:
    DWARFRegisterEnum() { }

    ~DWARFRegisterEnum()

    // Copied from <sys/user.h>
    static std::map<CPURegisterEnum::CPURegister, int> dwarfRegisterMap;
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
        gs

        // Floating point registers
        cwd,
        swd,
        ftw,
        fop,
        rip,
        rdp,
        mxcsr,
        mxcr_mask,
        st_space,  /* 8*16 bytes for each FP-reg = 128 bytes */
        xmm_space  /* 16*16 bytes for each XMM-reg = 256 bytes */
    
};


#endif // DWARFREGISTERENUM_H
#ifndef CPUREGISTERENUM_H
#define CPUREGISTERENUM_H

class CPURegisterEnum
{
public:
    CPURegisterEnum() { }

    ~CPURegisterEnum()

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
        gs

        // Floating point registers
        cwd,
        swd,
        ftw,
        fop,
        rip,
        rdp,
        mxcsr,
        mxcr_mask,
        st_space,  /* 8*16 bytes for each FP-reg = 128 bytes */
        xmm_space  /* 16*16 bytes for each XMM-reg = 256 bytes */
    };
};


#endif //CPUREGISTERENUM_H
