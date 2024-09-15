#include "emulator.hpp"
#include <iostream>

int main (int argc, char *argv[])
{
    Emulator &emulator = Emulator::getInstance();
    std::ifstream input_file;

    if (argc != 2)
    {
        std::cerr << "Error: Mora da bude tacno 2 argumenta!" << std::endl;
        return -1;
    }

    input_file.open(argv[1]);
    if (!input_file.is_open())
    {
        std::cerr << "Error: Neuspesno otvaranje fajla!" << std::endl;
        return -1;
    }

    emulator.loadFile(input_file);

    emulator.execute();

    return 0;
}