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

    void readFromFile(std::ofstream &output_file);

private:
    Linker() {}
    ~Linker() {}

    std::map<std::string, Symbol> globalSymbolTable;
    std::map<std::string, Section> globalSectionTable;
};

#endif // LINKER_HPP
