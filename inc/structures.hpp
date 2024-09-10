#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdint>

struct Symbol
{
    uint32_t address;
    bool isGlobal;
    bool isDefined;
    std::string section;

    Symbol() : address(0), isGlobal(false), isDefined(false), section("") {}
    Symbol(uint32_t address, bool isGlobal, bool isDefined, std::string section) : address(address), isGlobal(isGlobal), isDefined(isDefined), section(section) {}
};

struct Relocation
{
    uint32_t offset;
    std::string symbol;
    int32_t addend;

    Relocation(uint32_t offset, std::string symbol, int32_t addend) : offset(offset), symbol(symbol), addend(addend) {}
};

struct Section
{
    std::vector<uint8_t> data;
    std::vector<Relocation> relocations;

    Section() : data(), relocations() {}

    void addData(uint32_t data)
    {
        this->data.push_back(data & 0xFF);
        this->data.push_back((data >> 8) & 0xFF);
        this->data.push_back((data >> 16) & 0xFF);
        this->data.push_back((data >> 24) & 0xFF);
    }
};

enum class JumpType
{
    UNCOND,
    EQ,
    NE,
    GT
};

enum class ArithType
{
    ADD,
    SUB,
    MUL,
    DIV
};

enum class LogiType
{
    AND,
    OR,
    XOR,
    NOT
};

enum AdrType
{
    IMMED,
    SYMB,
    MDIR_LIT,
    MDIR_SYM,
    REG_DIR,
    REG_IND,
    REG_LIT,
};

struct Operand
{
    int literal;
    std::string symbol;
    int reg;
    AdrType adrType;
};

#endif // STRUCTURES_HPP