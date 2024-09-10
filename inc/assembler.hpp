#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "structures.hpp"
#include <cstdint>
class Assembler
{
public:
    static Assembler &getInstance()
    {
        static Assembler instance;
        return instance;
    }
    Assembler(Assembler const &) = delete;
    void operator=(Assembler const &) = delete;

    void label(std::string label);

    void parseHalt();
    void parseInt();
    void parseIret();
    void parseCall(Operand* op);
    void parseRet();
    void parseJump(uint8_t gpr1, uint8_t gpr2, Operand* op, JumpType type);
    void parsePush(uint8_t gpr);
    void parsePop(uint8_t gpr);
    void parseXchg(uint8_t gprS, uint8_t gprD);
    void parseArith(uint8_t gprS, uint8_t gprD, ArithType type);
    void parseLogi(uint8_t gprS, uint8_t gprD, LogiType type);
    void parseShl(uint8_t gprS, uint8_t gprD);
    void parseShr(uint8_t gprS, uint8_t gprD);
    void parseLd(Operand* op, uint8_t gpr);
    void parseSt(uint8_t gpr, Operand* op);
    void parseCsrrd(uint8_t csr, uint8_t gpr);
    void parseCsrwr(uint8_t gpr, uint8_t csr);
    
    void parseGlobal(std::vector <std::string> symbols);
    void parseSection(std::string section);
    void parseWord(std::vector <std::string> symbols);
    void parseSkip(uint8_t num);
    void parseEnd();

private:
    Assembler() {}
};

#endif // ASSEMBLER_HPP