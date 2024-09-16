#ifndef LINKER_HPP
#define LINKER_HPP

#include "structures.hpp"

class Linker
{
public:
    static Linker &getInstance()
    {
        static Linker instance;
        return instance;
    }
    Linker(Linker const &) = delete;
    void operator=(Linker const &) = delete;

    void readFiles(std::vector<std::string> files);
    void readFromFile(std::ifstream &input_file);
    void writeToFile(std::ofstream &output_file, std::map<std::string, uint32_t> &sectionAddresses);

    void mergeTables(std::map<std::string, Symbol> &symbolTable, std::map<std::string, Section> &sectionTable);
    void placeSections(std::map<std::string, uint32_t> &sectionAddresses, std::vector<std::string> &files);
    void relocation(std::map<std::string, uint32_t> &sectionAddresses);
    
    void printTables(std::map<std::string, Symbol> &symbolTable, std::map<std::string, Section> &sectionTable);
    void makeTextFile(std::string file);
    
private:
    Linker() {}
    ~Linker() {}

    std::map<std::string, Symbol> globalSymbolTable;
    std::map<std::string, Section> globalSectionTable;
};

#endif // LINKER_HPP
