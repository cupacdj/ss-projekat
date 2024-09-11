#include "assembler.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

void Assembler::fixRelocations()
{
    for (auto &[name, section] : sectionTable)
    {
        for (auto &reloc : section.relocations)
        {
            auto it = symbolTable.find(reloc.symbol);
            if (!it->second.isGlobal)
            {
                reloc.symbol = section.name;
                reloc.addend = it->second.address;
            }
        }
    }
}

void Assembler::printTables()
{

    std::cout << "Symbol Table:" << std::endl;
    std::cout << std::setw(20) << "Name"
              << std::setw(10) << "Address"
              << std::setw(10) << "Global"
              << std::setw(10) << "Defined"
              << std::setw(15) << "Section" << std::endl;

    for (const auto &[name, symbol] : symbolTable)
    {
        std::cout << std::setw(20) << name
                  << std::setw(10) << symbol.address
                  << std::setw(10) << (symbol.isGlobal ? "Yes" : "No")
                  << std::setw(10) << (symbol.isDefined ? "Yes" : "No")
                  << std::setw(15) << symbol.section << std::endl;
    }
    std::cout << std::endl;

    std::cout << "Section Table:" << std::endl;
    for (const auto &[name, section] : sectionTable)
    {
        std::cout << "Section: " << name << std::endl;
        std::cout << "Data: \n";

        // for (auto byte : section.data)
        // {
        //     std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte) << " ";
        // }

        for (int i = 0; i < section.data.size(); i++)
        {
            std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(section.data[i]) << " ";

            if (i % 16 == 15)
            {
                std::cout << std::endl;
            }
            else if (i % 4 == 3)
            {
                std::cout << " ";
            }
        }
        std::cout << std::endl;

        std::cout << "Relocations:" << std::endl;
        std::cout << std::setfill(' ')
                  << std::setw(10) << "Offset"
                  << std::setw(20) << "Symbol"
                  << std::setw(10) << "Addend" << std::endl;
        for (const auto &rel : section.relocations)
        {
            std::cout << std::setw(10) << rel.offset
                      << std::setw(20) << rel.symbol
                      << std::setw(10) << rel.addend << std::endl;
        }
        std::cout << std::endl;
    }
}

void Assembler::writeToFile(std::ofstream &output_file)
{

    for (const auto &sects : sectionTable)
    {
        const Section &section = sects.second;

        if (!section.data.empty())
        {
            output_file.write(reinterpret_cast<const char *>(section.data.data()), section.data.size());
        }
    }
}




