#include "emulator.hpp"
#include <iostream>
#include <iomanip>

void Emulator::loadFile(std::ifstream &input_file)
{
    std::string line;
    while (std::getline(input_file, line))
    {
        if (line.empty())
            continue;

        size_t colon_pos = line.find(':');
        if (colon_pos == std::string::npos)
        {
            std::cerr << "Nije nadjen karakter ':' " << line << std::endl;
            continue;
        }

        std::string addressStr = line.substr(0, colon_pos);

        uint32_t address = std::stoul(addressStr, nullptr, 16);

        for (size_t i = 10; i < line.size(); i += 3)
        {
            std::string byteStr = line.substr(i, 2);
            uint8_t byte = std::stoul(byteStr, nullptr, 16);

            if (address >= memory.size())
            {
                memory.resize(address + 1);
            }
            memory[address] = byte;
            // print Memory
            // std::cout << std::hex << std::setw(2) << std::setfill('0') << (int)memory[address] << " ";
            address++;
        }
        std::cout << std::endl;
    }
}

void Emulator::execute()
{
    for (int i = 0; i < 16; i++)
    {
        gpr[i] = 0;
    }
    for (int i = 0; i < 3; i++)
    {
        csr[i] = 0;
    }
    pc = 0x40000000;

    while (true)
    {
        std::cout << "PC: " << std::hex << pc << std::endl;
        getInstruction();
    }
}

void Emulator::printEmulator()
{
    std::cout << "-----------------------------------------------------------------" << std::endl;
    std::cout << "Emulated processor executed halt instruction." << std::endl;
    std::cout << "Emulated processor state:";
    for (int i = 0; i < 16; ++i)
    {
        if (i % 4 == 0)
        {
            std::cout << std::endl;
        }
        std::cout << (i < 10 ? " " : "") << "r" << std::dec << i << "=0x" << std::hex
                  << std::setw(8) << std::setfill('0') << gpr[i] << "\t";
    }
    std::cout << std::endl;
}

void Emulator::getInstruction()
{
    std::vector<uint8_t> instr = {memory[pc], memory[pc + 1], memory[pc + 2], memory[pc + 3]};
    pc += 4;
    uint8_t opcode = instr[0];
    opcode = (opcode >> 4) & 0x0F;
    std::cout << "Opcode: " << std::hex << (int)opcode << std::endl;
    switch (opcode)
    {
    case Instructions::HALT:
        std::cout << "HALT" << std::endl;
        printEmulator();
        exit(0);
        break;
    case Instructions::INT:
        std::cout << "INT" << std::endl;
        executeInt();
        break;
    case Instructions::CALL:
        std::cout << "CALL" << std::endl;
        executeCall(instr);
        break;
    case Instructions::JUMP:
        std::cout << "JUMP" << std::endl;
        executeJump(instr);
        break;
    case Instructions::XCHG:
        std::cout << "XCHG" << std::endl;
        executeXchg(instr);
        break;
    case Instructions::ARIT:
        std::cout << "ARIT" << std::endl;
        executeArit(instr);
        break;
    case Instructions::LOGI:
        std::cout << "LOGI" << std::endl;
        executeLogi(instr);

        break;
    case Instructions::SH:
        std::cout << "SH" << std::endl;
        executeSh(instr);
        break;
    case Instructions::ST:
        std::cout << "ST" << std::endl;
        executeSt(instr);
        break;
    case Instructions::LD:
        std::cout << "LD" << std::endl;
        executeLd(instr);
        break;
    default:
        std::cerr << "Nepoznata instrukcija" << std::endl;
        break;
    }
}

InstrEmu Emulator::extractRegisters(std::vector<uint8_t> &instruction)
{
    InstrEmu instr;
    instr.mode = instruction[0] & 0x0F;
    instr.A = (instruction[1] >> 4) & 0x0F;
    instr.B = instruction[1] & 0x0F;
    instr.C = (instruction[2] >> 4) & 0x0F;
    uint8_t D1 = instruction[2] & 0x0F;
    //int16_t D = (D1 << 8) | instruction[3];
    int16_t D = (instruction[3] << 4) | D1;
    if (D & 0x800)
    {
        D = D | 0xF000;
    }

    instr.D = D;
    return instr;
}
