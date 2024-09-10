#include <iostream>
#include "assembler.hpp"



using namespace std;



void Assembler::parseHalt()
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'00);
}

void Assembler::parseInt()
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'10);
}

void Assembler::parseIret()
{
    cout << "iret" << endl;
}

void Assembler::parseCall(Operand *op)
{
    cout << "call " << op << endl;
}

void Assembler::parseRet()
{
    cout << "ret" << endl;
}

void Assembler::parseJump(uint8_t gpr1, uint8_t gpr2, Operand *op, JumpType type)
{
    cout << "jmp " << "r" << (int)gpr1 << ", r" << (int)gpr2 << ", " << op << endl;
}

void Assembler::parsePush(uint8_t gpr)
{
    cout << "push " << "r" << (int)gpr << endl;
}

void Assembler::parsePop(uint8_t gpr)
{
    cout << "pop " << "r" << (int)gpr << endl;
}

void Assembler::parseXchg(uint8_t gprS, uint8_t gprD)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'40 | (gprD << 8) | (gprS << 20));
}

void Assembler::parseArith(uint8_t gprS, uint8_t gprD, ArithType type)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    if(type == ArithType::ADD)
    {
        currentSection->addData(0x00'00'00'50 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if(type == ArithType::SUB)
    {
        currentSection->addData(0x00'00'00'51 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if(type == ArithType::MUL)
    {
        currentSection->addData(0x00'00'00'52 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if(type == ArithType::DIV)
    {
        currentSection->addData(0x00'00'00'53 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
}

void Assembler::parseLogi(uint8_t gprS, uint8_t gprD, LogiType type)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    if(type == LogiType::NOT)
    {
        currentSection->addData(0x00'00'00'60 | (gprD << 8) | (gprD << 12));
    }
    else if(type == LogiType::AND)
    {
        currentSection->addData(0x00'00'00'61 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if(type == LogiType::OR)
    {
        currentSection->addData(0x00'00'00'62 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if(type == LogiType::XOR)
    {
        currentSection->addData(0x00'00'00'63 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    
}

void Assembler::parseShl(uint8_t gprS, uint8_t gprD)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'70 | (gprD << 8) | (gprD << 12) | (gprS << 20));
}

void Assembler::parseShr(uint8_t gprS, uint8_t gprD)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'71 | (gprD << 8) | (gprD << 12) | (gprS << 20));
}

void Assembler::parseLd(Operand *op, uint8_t gpr)
{
    cout << "ld " << op << ", r" << (int)gpr << endl;
}

void Assembler::parseSt(uint8_t gpr, Operand *op)
{
    cout << "st " << "r" << (int)gpr << ", " << op << endl;
}

void Assembler::parseCsrrd(uint8_t csr, uint8_t gpr)
{
    cout << "csrrd " << "r" << (int)csr << ", r" << (int)gpr << endl;
}

void Assembler::parseCsrwr(uint8_t gpr, uint8_t csr)
{
    cout << "csrwr " << "r" << (int)gpr << ", r" << (int)csr << endl;
}
