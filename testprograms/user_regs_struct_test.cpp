#include <iostream>
#include <sys/user.h>
#include "CPURegisterEnum.h"

typedef struct user_regs_struct user_regs_struct;

uint64_t getVal(user_regs_struct* regs, CPURegisterEnum::CPURegister reg)
{

    // Scary hack. But because the Enum is laid out in the same order as the members of the regs struct, this works
    uint64_t mask(-1);
    mask <<= reg;
    return (regs & mask) >> reg;
}

int main()
{
    user_regs_struct regs;

    CPURegisterEnum::CPURegister reg;

    std::cout << "result: " << getVal(reinterpret_cast<uint64_t*>(&regs), CPURegisterEnum::r15) << std::endl;

    // Scary hack. But because the Enum is laid out in the same order as the members of the regs struct, this works
    
    return 0;
}


