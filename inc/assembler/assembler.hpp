#ifndef ASSEMBLER_HPP
#define ASSEMBLER_HPP

#include "../structures.hpp"
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


private:
    Assembler() {}
};

#endif // ASSEMBLER_HPP