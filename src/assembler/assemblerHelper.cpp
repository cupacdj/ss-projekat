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
            if(it == symbolTable.end())
            {
                std::cout << "Error: Simbol: (" << reloc.symbol << ") nije pronadjen!" << std::endl;
                exit(-1);
            }
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
    // write number of sections
    uint32_t numSections = sectionTable.size();
    output_file.write(reinterpret_cast<const char *>(&numSections), sizeof(numSections));

    for (const auto &sects : sectionTable)
    {
        const Section &section = sects.second;

        // section name and name size
        uint32_t nameSize = section.name.size();
        output_file.write(reinterpret_cast<const char *>(&nameSize), sizeof(nameSize));
        output_file.write(section.name.c_str(), section.name.size());


        // number of relocations
        uint32_t numRelocations = section.relocations.size();
        output_file.write(reinterpret_cast<const char *>(&numRelocations), sizeof(numRelocations));

        // relocations
        for (const auto &rel : section.relocations)
        {
            // offset
            output_file.write(reinterpret_cast<const char *>(&rel.offset), sizeof(rel.offset));

            // symbol name and name size
            uint32_t symbolSize = rel.symbol.size();
            output_file.write(reinterpret_cast<const char *>(&symbolSize), sizeof(symbolSize));
            output_file.write(rel.symbol.c_str(), rel.symbol.size());

            // addend
            output_file.write(reinterpret_cast<const char *>(&rel.addend), sizeof(rel.addend));
        }

        // section data size
        uint32_t dataSize = section.data.size();
        output_file.write(reinterpret_cast<const char *>(&dataSize), sizeof(dataSize));

        // section data
        if (!section.data.empty())
        {
            output_file.write(reinterpret_cast<const char *>(section.data.data()), section.data.size());
        }
    }

    // number of symbols
    size_t numSymbols = 0;
    for (const auto &symbol : symbolTable)
    {
        if (symbol.second.isGlobal)
        {
            numSymbols++;
        }
    }

    output_file.write(reinterpret_cast<const char *>(&numSymbols), sizeof(numSymbols));

    for (const auto &symbol : symbolTable)
    {
        if (symbol.second.isGlobal ) 
        {
            // symbol name and name size
            uint32_t nameSize = symbol.first.size();
            output_file.write(reinterpret_cast<const char *>(&nameSize), sizeof(nameSize));
            output_file.write(symbol.first.c_str(), symbol.first.size());

            // symbol address
            output_file.write(reinterpret_cast<const char *>(&symbol.second.address), sizeof(symbol.second.address));

            // section name and name size
            uint32_t sectionSize = symbol.second.section.size();
            output_file.write(reinterpret_cast<const char *>(&sectionSize), sizeof(sectionSize));
            output_file.write(symbol.second.section.c_str(), symbol.second.section.size());

            // is defined
            output_file.write(reinterpret_cast<const char *>(&symbol.second.isDefined), sizeof(symbol.second.isDefined));
        }
    }
}

