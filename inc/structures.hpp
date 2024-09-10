#ifndef STRUCTURES_HPP
#define STRUCTURES_HPP

#include <string>
#include <vector>

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
    SYMB,
    MDIR_LIT,
    MDIR_SYM,
    REG_DIR,
    REG_IND,
    REG_LIT,
};

struct Operand {
    int literal;
    std::string symbol;
    int reg;
    AdrType adrType;
};



#endif // STRUCTURES_HPP