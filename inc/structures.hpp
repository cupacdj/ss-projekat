#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>
#include <vector>
#include <map>
#include <cstdint>

struct Symbol
{
    std::string name;
    uint32_t address;
    bool isGlobal;
    bool isDefined;
    std::string section;
    bool isWeak;

    Symbol() : address(0), isGlobal(false), isDefined(false), section("") {}
    Symbol(uint32_t address, bool isGlobal, bool isDefined, std::string section) : address(address), isGlobal(isGlobal), isDefined(isDefined), section(section) {}
};

struct Relocation
{
    uint32_t offset;
    std::string symbol;
    uint32_t addend;

    Relocation() : offset(0), symbol(""), addend(0) {}
    Relocation(uint32_t offset, std::string symbol, uint32_t addend) : offset(offset), symbol(symbol), addend(addend) {}
};

struct Section
{
    std::string name;
    std::vector<uint8_t> data;
    std::vector<Relocation> relocations;

    void addData(uint32_t data)
    {
        this->data.push_back(data & 0xFF);
        this->data.push_back((data >> 8) & 0xFF);
        this->data.push_back((data >> 16) & 0xFF);
        this->data.push_back((data >> 24) & 0xFF);
    }
};

enum Instructions
{
    HALT,
    INT,
    CALL,
    JUMP,
    XCHG,
    ARIT,
    LOGI,
    SH,
    ST,
    LD
};

enum JumpType
{
    UNCOND,
    EQ,
    NE,
    GT
};

enum ArithType
{
    ADD,
    SUB,
    MUL,
    DIV
};

enum LogiType
{
    AND,
    OR,
    XOR,
    NOT
};

enum ShiftType
{
    SHL,
    SHR
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
    uint32_t literal;
    std::string symbol;
    uint8_t reg;
    AdrType adrType;
};

#endif // STRUCTURES_HPP