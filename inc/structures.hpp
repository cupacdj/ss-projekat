#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>

enum class JumpType {
    UNCOND,
    EQ,
    NE,
    GT
};

enum class ArithType {
    ADD,
    SUB,
    MUL,
    DIV
};

enum class LogiType {
    AND,
    OR,
    XOR,
    NOT
};

enum AdrType {
    IMMED,
    SYMBOL,
    MDIR_LIT,
    MDIR_SYM,
    REG_DIR,
    REG_IND,
    REG_LIT,
    REG_SYM
};

struct Operand {
    int literal;
    std::string symbol;
    int reg;
    AdrType adrType;

};

#endif // STRUCTURES_HPP