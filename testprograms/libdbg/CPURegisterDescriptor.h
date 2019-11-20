#ifndef CPUREGISTERDESCRIPTOR_H
#define CPUREGISTERDESCRIPTOR_H

#include "CPURegisterEnum.h"
#include <string>

class CPURegisterDescriptor
{
public:
    CPURegisterDescriptor() : regEnum(), dwarfNum(0), name("") { }
    CPURegisterDescriptor(const CPURegisterEnum::CPURegister r, int d, const std::string& n) : regEnum(r), dwarfNum(d), name(n) { }
    ~CPURegisterDescriptor() { }

    CPURegisterEnum::CPURegister regEnum;
    int dwarfNum;
    std::string name;

};


#endif // CPUREGISTERDESCRIPTOR_H
