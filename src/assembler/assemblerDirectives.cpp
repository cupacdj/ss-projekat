#include "assembler.hpp"


void Assembler::label(std::string label)
{
    symbolTable[label].address = currentSection->data.size();
    symbolTable[label].isDefined = true;
    symbolTable[label].section = currentSection->name;
}


void Assembler::parseGlobal(std::vector<std::string> symbols)
{
    for (const std::string &sym : symbols)
    {
        symbolTable[sym].isGlobal = true;
    }
}

void Assembler::parseSection(const std::string sectionName)
{
    currentSection = &sectionTable[sectionName];
    currentSection->name = sectionName;
}


void Assembler::parseWord(std::vector<std::string> symbols)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    for (const std::string &sym : symbols)
    {
        try
        {
            size_t val = std::stoull(sym, nullptr, 0);

            if (val > 0xFFFFFFFF)
            {
                throw "Vrednost je veca od 32 bita!";
            }

            currentSection->addData(val);
        }
        catch (...)
        {
            currentSection->relocations.push_back({static_cast<uint32_t>(currentSection->data.size()), sym, 0});
            currentSection->addData(0);
        }
    }
}

void Assembler::parseSkip(uint8_t num)
{
    if (currentSection == nullptr)
    {
        throw "Nije definisana sekcija!";
    }
    while(num--){
        currentSection->data.push_back(0);
    }
}

void Assembler::parseType(std::string symb, std::string type)
{
    if (type == "FUNC")
    {
        symbolTable[symb].type = DataType::FUNC;
    }
    else if (type == "DATA")
    {
        symbolTable[symb].type = DataType::DATA;
    }
    else
    {
        symbolTable[symb].type = DataType::NOTYPE;
    }
}
