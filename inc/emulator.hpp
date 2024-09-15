#ifndef EMULATOR_HPP
#define EMULATOR_HPP
#include <vector>
#include <cstdint>
#include <fstream>

#include "structures.hpp"

struct InstrEmu
{
    uint8_t mode;
    uint8_t A;
    uint8_t B;
    uint8_t C;
    int16_t D;
};

class Emulator
{
public:
    static Emulator &getInstance()
    {
        static Emulator instance;
        return instance;
    }
    Emulator(Emulator const &) = delete;
    void operator=(Emulator const &) = delete;

    void loadFile(std::ifstream &input_file);
    void printMemory();

    void execute();
    void getInstruction();
    void printEmulator();
    InstrEmu extractRegisters(std::vector<uint8_t> &instruction);

    void executeInt();
    void executeCall(std::vector<uint8_t> &instruction);
    void executeJump(std::vector<uint8_t> &instruction);
    void executeXchg(std::vector<uint8_t> &instruction);
    void executeArit(std::vector<uint8_t> &instruction);
    void executeLogi(std::vector<uint8_t> &instruction);
    void executeSh(std::vector<uint8_t> &instruction);
    void executeSt(std::vector<uint8_t> &instruction);
    void executeLd(std::vector<uint8_t> &instruction);
    

private:
    Emulator() {}
    ~Emulator() {}

    std::vector<uint8_t> memory;
    uint32_t gpr[16];
    uint32_t csr[3];
    uint32_t &pc = gpr[15];
    uint32_t &sp = gpr[14];
    uint32_t &status = csr[0];
    uint32_t &handle = csr[1];
    uint32_t &cause = csr[2];
};

#endif // EMULATOR_HPP