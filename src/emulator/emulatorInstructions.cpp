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

    switch (instr.mode)
    {
    case 0x00:
        // pc<=gpr[A]+gpr[B]+D;
        pc = gpr[instr.A] + gpr[instr.B] + instr.D;
        break;
    case 0x01:
    {
        // pc<=mem32[gpr[A]+gpr[B]+D];
        uint32_t tmp = 0;
        for (int i = 0; i < 4; i++)
        {
            tmp = tmp | (memory[gpr[instr.A] + gpr[instr.B] + instr.D + i] << (8 * i));
        }
        pc = tmp;
        break;
    }
    }
}

void Emulator::executeJump(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        // pc<=gpr[A]+gpr[B]+D;
        pc = gpr[instr.A] + gpr[instr.B] + instr.D;
        break;
    case 0x01:
        if (gpr[instr.B] == gpr[instr.B])
        {
            pc = gpr[instr.A] + instr.D;
        }
        break;
    case 0x02:
        if (gpr[instr.B] != gpr[instr.C])
        {
            pc = gpr[instr.A] + instr.D;
        }
        break;
    case 0x03:
        if (static_cast<int32_t>(gpr[instr.B]) > static_cast<int32_t>(gpr[instr.C]))
        {
            pc = gpr[instr.A] + instr.D;
        }
        break;
    case 0x08:
    {
        // pc<=mem32[gpr[A]+D];
        uint32_t tmp = 0;
        for (int i = 0; i < 4; i++)
        {
            tmp = tmp | (memory[gpr[instr.A] + instr.D + i] << (8 * i));
        }
        pc = tmp;
        break;
    }
    case 0x09:
    {
        if (gpr[instr.B] == gpr[instr.C])
        {
            uint32_t tmp2 = 0;
            for (int i = 0; i < 4; i++)
            {
                tmp2 = tmp2 | (memory[gpr[instr.A] + instr.D + i] << (8 * i));
            }
            pc = tmp2;
        }
        break;
    }
    case 0x0A:
    {
        if (gpr[instr.B] != gpr[instr.C])
        {
            uint32_t tmp3 = 0;
            for (int i = 0; i < 4; i++)
            {
                tmp3 = tmp3 | (memory[gpr[instr.A] + instr.D + i] << (8 * i));
            }
            pc = tmp3;
        }
        break;
    }
    case 0x0B:
    {
        if (static_cast<int32_t>(gpr[instr.B]) > static_cast<int32_t>(gpr[instr.C]))
        {
            uint32_t tmp4 = 0;
            for (int i = 0; i < 4; i++)
            {
                tmp4 = tmp4 | (memory[gpr[instr.A] + instr.D + i] << (8 * i));
            }
            pc = tmp4;
        }
        break;
    }
    default:
        std::cerr << "Nepoznat kod skoka" << std::endl;
        exit(-1);
        break;
    }
}

void Emulator::executeXchg(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    uint32_t tmp = gpr[instr.B];
    gpr[instr.B] = gpr[instr.C];
    gpr[instr.C] = tmp;
}

void Emulator::executeArit(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        gpr[instr.A] = gpr[instr.B] + gpr[instr.C];
        break;
    case 0x01:
        gpr[instr.A] = gpr[instr.B] - gpr[instr.C];
        break;
    case 0x02:
        gpr[instr.A] = gpr[instr.B] * gpr[instr.C];
        break;
    case 0x03:
        gpr[instr.A] = gpr[instr.B] / gpr[instr.C];
        break;
    }
}

void Emulator::executeLogi(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        gpr[instr.A] = ~gpr[instr.B];

        break;
    case 0x01:
        gpr[instr.A] = gpr[instr.B] & gpr[instr.C];

        break;
    case 0x02:
        gpr[instr.A] = gpr[instr.B] | gpr[instr.C];

        break;
    case 0x03:
        gpr[instr.A] = gpr[instr.B] ^ gpr[instr.C];
        break;
    }
}

void Emulator::executeSh(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        gpr[instr.A] = gpr[instr.B] << gpr[instr.C];
        break;
    case 0x01:
        gpr[instr.A] = gpr[instr.B] >> gpr[instr.C];
        break;
    }
}

void Emulator::executeSt(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        for (int i = 0; i < 4; i++)
        {
            memory[gpr[instr.A] + gpr[instr.B] + instr.D + i] = (gpr[instr.C] >> (8 * i)) & 0xFF;
        }
        break;
    case 0x01:
    {
        uint32_t address = 0;
        for (int i = 0; i < 4; i++)
        {
            address = address | (memory[gpr[instr.A] + gpr[instr.B] + instr.D + i] << (8 * i));
        }

        for (int i = 0; i < 4; i++)
        {
            memory[address + i] = (gpr[instr.C] >> (8 * i)) & 0xFF;
        }
        break;
    }
    case 0x02:
        gpr[instr.A] = gpr[instr.A] + instr.D;
        for (int i = 0; i < 4; i++)
        {
            memory[gpr[instr.A] + i] = (gpr[instr.C] >> (8 * i)) & 0xFF;
        }
        break;
    }
}

void Emulator::executeLd(std::vector<uint8_t> &instruction)
{
    InstrEmu instr = extractRegisters(instruction);

    switch (instr.mode)
    {
    case 0x00:
        gpr[instr.A] = csr[instr.B];
        break;
    case 0x01:
        gpr[instr.A] = gpr[instr.B] + instr.D;
        break;
    case 0x02:
    {
        uint32_t tmp = 0;
        for (int i = 0; i < 4; i++)
        {
            tmp = tmp | (memory[gpr[instr.B] + gpr[instr.C] + instr.D + i] << (8 * i));
        }
        gpr[instr.A] = tmp;
        break;
    }
    case 0x03:
    {
        uint32_t tmp2 = 0;
        for (int i = 0; i < 4; i++)
        {
            tmp2 = tmp2 | (memory[gpr[instr.B] + i] << (8 * i));
        }
        gpr[instr.A] = tmp2;
        gpr[instr.B] = gpr[instr.B] + instr.D;
        break;
    }
    case 0x04:
        csr[instr.A] = gpr[instr.B];
        break;
    case 0x05:
        csr[instr.A] = csr[instr.B] | instr.D;
        break;
    case 0x06:
    {
        uint32_t tmp3 = 0;
        for (int i = 0; i < 4; i++)
        {
            tmp3 = tmp3 | (memory[gpr[instr.B] + gpr[instr.C] + instr.D + i] << (8 * i));
        }
        csr[instr.A] = tmp3;
        break;
    }
    case 0x07:
    {
        uint32_t tmp4 = 0;
        for (int i = 0; i < 4; i++)
        {
            tmp4 = tmp4 | (memory[gpr[instr.B] + i] << (8 * i));
        }
        csr[instr.A] = tmp4;
        gpr[instr.B] = gpr[instr.B] + instr.D;
        break;
    }
    }
}