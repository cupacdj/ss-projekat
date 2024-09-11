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
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    uint8_t sp = 14;
    uint8_t disp4 = 4;
    uint8_t disp8 = 8;
    uint8_t pc = 15;
    uint8_t csr = 0;

    // pop status: csr = (m(sp+4))
    currentSection->addData(0x00'00'00'96 | (sp << 8) | (csr << 12) | (disp4 << 16));

    // pop pc: pc = m(sp), sp = sp + 8)
    currentSection->addData(0x00'00'00'93 | (sp << 8) | (pc << 12) | (disp8 << 16));
}

void Assembler::parseCall(Operand *op)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    if (op->adrType != AdrType::MDIR_SYM && op->adrType != AdrType::MDIR_LIT)
    {
        throw "Adresa nije u ispravnom obliku";
    }
    uint8_t pc = 15;
    std::string symbol = op->symbol;
    uint8_t disp = 4;

    // call mem(pc+4)
    currentSection->addData(0x00'00'00'21 | (pc << 12) | (disp << 16));
    // jmp pc+4
    currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));

    if (op->adrType == AdrType::MDIR_LIT)
    {
        size_t lit = op->literal;

        if (lit > 0xFFFFFFFF)
        {
            throw "Prevelik broj!";
        }

        currentSection->addData(lit);
    }
    else
    {
        std::string symbol = op->symbol;
        currentSection->relocations.push_back(Relocation{static_cast<uint32_t>(currentSection->data.size()), symbol, 0});
        currentSection->addData(0);
    }
}

void Assembler::parseJump(uint8_t gpr1, uint8_t gpr2, Operand *op, JumpType type)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    if (op->adrType != AdrType::MDIR_SYM && op->adrType != AdrType::MDIR_LIT)
    {
        throw "Adresa nije u ispravnom obliku";
    }

    uint8_t pc = 15;
    uint8_t disp = 4;

    if (type == JumpType::UNCOND)
    {
        // jmp mem(pc+4)
        currentSection->addData(0x00'00'00'38 | (pc << 12));
    }
    else if (type == JumpType::EQ)
    {
        // jmp mem(pc+4)
        currentSection->addData(0x00'00'00'39 | (gpr1 << 8) | (pc << 12) | (disp << 16) | (gpr2 << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));
    }
    else if (type == JumpType::NE)
    {
        // jmp mem(pc+4)
        currentSection->addData(0x00'00'00'3A | (pc << 12) | (disp << 16) | (disp << 16) | (gpr2 << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));
    }
    else if (type == JumpType::GT)
    {
        // jmp mem(pc+4)
        currentSection->addData(0x00'00'00'3B | (pc << 12) | (disp << 16) | (disp << 16) | (gpr2 << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));
    }

    if (op->adrType == AdrType::MDIR_LIT)
    {
        size_t lit = op->literal;

        if (lit > 0xFFFFFFFF)
        {
            throw "Prevelik broj!";
        }

        currentSection->addData(lit);
    }
    else
    {
        std::string symbol = op->symbol;
        currentSection->relocations.push_back(Relocation{static_cast<uint32_t>(currentSection->data.size()), symbol, 0});
        currentSection->addData(0);
    }
}

void Assembler::parseRet()
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    uint8_t sp = 14;
    uint8_t disp = 4;
    uint8_t pc = 15;
    currentSection->addData(0x00'00'00'93 | (sp << 8) | (pc << 12) | (disp << 16));
}

void Assembler::parsePush(uint8_t gpr)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    uint8_t sp = 14;
    uint16_t disp = 0xffc;
    currentSection->addData(0x00'00'00'81 | (sp << 12) | ((disp & 0xF) << 16) | (gpr << 20) | ((disp & 0xFF0) << 20));
}

void Assembler::parsePop(uint8_t gpr)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    uint8_t sp = 14;
    uint8_t disp = 4;
    currentSection->addData(0x00'00'00'93 | (sp << 8) | (gpr << 12) | (disp << 16));
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
    if (type == ArithType::ADD)
    {
        currentSection->addData(0x00'00'00'50 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if (type == ArithType::SUB)
    {
        currentSection->addData(0x00'00'00'51 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if (type == ArithType::MUL)
    {
        currentSection->addData(0x00'00'00'52 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if (type == ArithType::DIV)
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
    if (type == LogiType::NOT)
    {
        currentSection->addData(0x00'00'00'60 | (gprD << 8) | (gprD << 12));
    }
    else if (type == LogiType::AND)
    {
        currentSection->addData(0x00'00'00'61 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if (type == LogiType::OR)
    {
        currentSection->addData(0x00'00'00'62 | (gprD << 8) | (gprD << 12) | (gprS << 20));
    }
    else if (type == LogiType::XOR)
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
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }

    uint8_t pc = 15;
    uint8_t disp = 4;

    if (op->adrType == AdrType::IMMED || op->adrType == AdrType::MDIR_LIT)
    {
        size_t lit = op->literal;

        if (lit > 0xFFFFFFFF)
        {
            throw "Prevelik broj!";
        }

        // ld mem(pc+4)
        currentSection->addData(0x00'00'00'92 | (gpr << 12) | (disp << 16) | (pc << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));

        currentSection->addData(lit);

        // ld 0xf00, %r5 <=> ld $0xf00, %r5, ld [%r5], %r5
        if (op->adrType == AdrType::MDIR_LIT)
        {
            currentSection->addData(0x00'00'00'92 | (gpr << 8) | (gpr << 12));
        }
    }
    else if (op->adrType == AdrType::SYMB || op->adrType == AdrType::MDIR_SYM)
    {
        // ld mem(pc+4)
        currentSection->addData(0x00'00'00'92 | (gpr << 12) | (disp << 16) | (pc << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));

        std::string symbol = op->symbol;
        currentSection->relocations.push_back(Relocation{static_cast<uint32_t>(currentSection->data.size()), symbol, 0});
        currentSection->addData(0);

        // ld sym, %r5 <=> ld $sym, %r5, ld [%r5], %r5
        if (op->adrType == AdrType::MDIR_SYM)
        {
            currentSection->addData(0x00'00'00'92 | (gpr << 8) | (gpr << 12));
        }
    }
    else if (op->adrType == AdrType::REG_DIR)
    {
        uint8_t reg = op->reg;
        // ld reg
        currentSection->addData(0x00'00'00'91 | (reg << 8) | (gpr << 12));
    }
    else if (op->adrType == AdrType::REG_IND || op->adrType == AdrType::REG_LIT)
    {
        size_t lit = op->literal;
        uint8_t reg = op->reg;

        if (lit > 0xFFF)
        {
            throw "Prevelik broj!";
        }

        // ld (reg + lit)
        currentSection->addData(0x00'00'00'92 | (reg << 8) | ((lit & 0xF) << 16) | (gpr << 12) | ((lit & 0xFF0) << 20));
    }
    else
    {
        throw "Neispravno adresiranje";
    }
}

void Assembler::parseSt(uint8_t gpr, Operand *op)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    if (op->adrType == AdrType::IMMED || op->adrType == AdrType::SYMB || op->adrType == AdrType::REG_DIR)
    {
        throw "Neispravno adresiranje";
    }

    uint8_t pc = 15;
    uint8_t disp = 4;

    if (op->adrType == AdrType::MDIR_LIT)
    {
        size_t lit = op->literal;

        if (lit > 0xFFFFFFFF)
        {
            throw "Prevelik broj!";
        }

        // st mem(pc+4)
        currentSection->addData(0x00'00'00'82 | (pc << 12) | (disp << 16) | (gpr << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));

        currentSection->addData(lit);
    }
    else if (op->adrType == AdrType::MDIR_SYM)
    {
        // st mem(pc+4)
        currentSection->addData(0x00'00'00'82 | (pc << 12) | (disp << 16) | (gpr << 20));
        // jmp pc+4
        currentSection->addData(0x00'00'00'30 | (pc << 12) | (disp << 16));

        std::string symbol = op->symbol;
        currentSection->relocations.push_back(Relocation{static_cast<uint32_t>(currentSection->data.size()), symbol, 0});
        currentSection->addData(0);
    }
    else if (op->adrType == AdrType::REG_IND || op->adrType == AdrType::REG_LIT)
    {
        size_t lit = op->literal;
        uint8_t reg = op->reg;

        if (lit > 0xFFF)
        {
            throw "Prevelik broj!";
        }

        // st mem(reg + lit)
        currentSection->addData(0x00'00'00'80 | (reg << 12) | ((lit & 0xF) << 16) | (gpr << 20) | ((lit & 0xFF0) << 20));
    }
    else
    {
        throw "Neispravno adresiranje";
    }
}

void Assembler::parseCsrrd(uint8_t csr, uint8_t gpr)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'90 | (csr << 8) | (gpr << 12));
}

void Assembler::parseCsrwr(uint8_t gpr, uint8_t csr)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    currentSection->addData(0x00'00'00'94 | (gpr << 8) | (csr << 12));
}
