#include "emulator.hpp"
#include <iostream>

void Emulator::executeInt()
{
    // push status
    sp -= 4;
    for (int i = 0; i < 4; i++)
    {
        memory[sp + i] = (status >> (8 * i)) & 0xFF;
    }
    // push pc
    sp -= 4;
    for (int i = 0; i < 4; i++)
    {
        memory[sp + i] = (pc >> (8 * i)) & 0xFF;
    }

    cause = 4;
    status = status | 0x4;
    pc = handle;
}

void Emulator::executeCall(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    // push pc
    sp -= 4;
    for (int i = 0; i < 4; i++)
    {
        memory[sp + i] = (pc >> (8 * i)) & 0xFF;
    }

    // pc<=mem32[gpr[A]+gpr[B]+D];
    uint32_t tmp = 0;
    for (int i = 0; i < 4; i++)
    {
        tmp = tmp | (memory[gpr[instr.gprA] + gpr[instr.gprB] + instr.D + i] << (8 * i));
    }
    pc = tmp;
}

void Emulator::executeJump(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        // pc<=gpr[A]+gpr[B]+D;
        pc = gpr[instr.gprA] + gpr[instr.gprB] + instr.D;
        break;
    case 0x01:
        if (gpr[instr.gprB] == gpr[instr.gprB])
        {
            pc = gpr[instr.gprA] + instr.D;
        }
        break;
    case 0x02:
        if (gpr[instr.gprB] != gpr[instr.gprC])
        {
            pc = gpr[instr.gprA] + instr.D;
        }
        break;
    case 0x03:
        if (static_cast<int32_t>(gpr[instr.gprB]) > static_cast<int32_t>(gpr[instr.gprC]))
        {
            pc = gpr[instr.gprA] + instr.D;
        }
        break;
    case 0x08:
        
        break;
    case 0x09:

        break;
    case 0x0A:
        break;
    case 0x0B:
        break;
    default:
        std::cerr << "Nepoznat tip skoka" << std::endl;
        exit(1);
        break;
    }
}