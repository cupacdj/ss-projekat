#include "linker.hpp"
#include <iostream>
#include <fstream>
#include <cstring>

int main(int argc, char *argv[])
{
    if (std::strcmp(argv[1], "-hex") != 0)
    {
        std::cout << "Invalid argument!" << std::endl;
        exit(-1);
    }
    
    // if (argc >= 3)
    // {
    //     std::cout << "Potrebno je 3 ili vise argumenta a data su: " << argc - 1 << "!" << std::endl;
    //     return -1;
    // }

    Linker &linker = Linker::getInstance();

    for (int i = 2; i < argc; i++)
    {
        std::ifstream input_file(argv[i], std::ios::binary);
        if (!input_file.is_open())
        {
            std::cout << "Neuspesno otvaranje fajla " << argv[3] << std::endl;
            return -1;
        }

        linker.readFromFile(input_file);
    }

    return 0;
}