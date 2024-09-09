#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>

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

struct Operand {
    int adressing_mode;
    int literal;
    std::string symbol;
    int reg;
};

#endif // STRUCTURES_HPP