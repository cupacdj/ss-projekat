#include "linker.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>

void Linker::printTables(std::map<std::string, Symbol> &symbolTable, std::map<std::string, Section> &sectionTable)
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

void Linker::mergeTables(std::map<std::string, Symbol> &symbolTable, std::map<std::string, Section> &sectionTable)
{
    for (const auto &[name, symbol] : symbolTable)
    {
        if (globalSymbolTable.find(name) == globalSymbolTable.end())
        {
            globalSymbolTable[name] = symbol;
            globalSymbolTable[name].address += globalSectionTable[symbol.section].data.size();
        }
        else
        {
            if (globalSymbolTable[name].isDefined && symbol.isDefined)
            {
                std::cerr << "Error: Simbol " << name << " je definisan vise puta." << std::endl;
                exit(-1);
            }
            else if (!globalSymbolTable[name].isDefined)
            {
                globalSymbolTable[name] = symbol;
                globalSymbolTable[name].address += globalSectionTable[symbol.section].data.size();
            }
        }
    }

    // ako je ostao neki simbol u globalnoj tabeli koji nije definisan
    for (const auto &[name, symbol] : symbolTable)
    {
        if (!globalSymbolTable[name].isDefined)
        {
            std::cerr << "Error: Simbol " << name << " nije definisan." << std::endl;
            exit(-1);
        }
    }

    // for relocations in sectionTable
    for (const auto &[name, section] : sectionTable)
    {
        for (const auto &rel : section.relocations)
        {
            Relocation newRel = rel;
            newRel.offset += globalSectionTable[section.name].data.size();
            
            if(rel.symbol == name){
                newRel.addend += globalSectionTable[section.name].data.size();
            }
            globalSectionTable[section.name].relocations.push_back(newRel);
        }
        globalSectionTable[section.name].data.insert(globalSectionTable[section.name].data.end(), section.data.begin(), section.data.end());
    }

    

}

void Linker::readFromFile(std::ifstream &input_file)
{
    std::cout << "Reading from file..." << std::endl;

    std::map<std::string, Section> localSectionTable;
    std::map<std::string, Symbol> localSymbolTable;

    // number of sections
    uint32_t numSections;
    input_file.read(reinterpret_cast<char *>(&numSections), sizeof(numSections));

    for (int i = 0; i < numSections; i++)
    {
        Section section;

        // section name and name size
        uint32_t nameSize;
        input_file.read(reinterpret_cast<char *>(&nameSize), sizeof(nameSize));
        section.name.resize(nameSize);
        input_file.read(&section.name[0], nameSize);

        // number of relocations
        uint32_t numRelocations;
        input_file.read(reinterpret_cast<char *>(&numRelocations), sizeof(numRelocations));

        // relocations
        for (int j = 0; j < numRelocations; j++)
        {
            Relocation rel;

            // offset
            input_file.read(reinterpret_cast<char *>(&rel.offset), sizeof(rel.offset));

            // symbol name and name size
            uint32_t symbolSize;
            input_file.read(reinterpret_cast<char *>(&symbolSize), sizeof(symbolSize));
            rel.symbol.resize(symbolSize);
            input_file.read(&rel.symbol[0], symbolSize);

            // addend
            input_file.read(reinterpret_cast<char *>(&rel.addend), sizeof(rel.addend));

            section.relocations.push_back(rel);
        }

        // data size
        uint32_t dataSize;
        input_file.read(reinterpret_cast<char *>(&dataSize), sizeof(dataSize));
        section.data.resize(dataSize);
        input_file.read(reinterpret_cast<char *>(section.data.data()), dataSize);

        localSectionTable[section.name] = section;
        ;
    }

    // number of symbols
    size_t numSymbols;
    input_file.read(reinterpret_cast<char *>(&numSymbols), sizeof(numSymbols));

    for (int i = 0; i < numSymbols; i++)
    {
        Symbol symbol;

        // symbol name and name size
        uint32_t nameSize;
        input_file.read(reinterpret_cast<char *>(&nameSize), sizeof(nameSize));
        symbol.name.resize(nameSize);
        input_file.read(&symbol.name[0], nameSize);

        // symbol address
        input_file.read(reinterpret_cast<char *>(&symbol.address), sizeof(symbol.address));

        // section name and name size
        uint32_t sectionSize;
        input_file.read(reinterpret_cast<char *>(&sectionSize), sizeof(sectionSize));
        symbol.section.resize(sectionSize);
        input_file.read(&symbol.section[0], sectionSize);

        input_file.read(reinterpret_cast<char *>(&symbol.isDefined), sizeof(symbol.isDefined));

        localSymbolTable[symbol.name] = symbol;
    }

    std::cout << "Local tables: \n"
              << std::endl;
    Linker::printTables(localSymbolTable, localSectionTable);

    // Linker::mergeTables(localSymbolTable, localSectionTable);
}

void Linker::readFiles(std::vector<std::string> files)
{
    for (auto file : files)
    {
        std::ifstream input_file(file, std::ios::binary);
        if (!input_file.is_open())
        {
            std::cerr << "Error opening file: " << file << std::endl;
            exit(-1);
        }

        Linker::readFromFile(input_file);
    }
}
