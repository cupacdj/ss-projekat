#include "linker.hpp"
#include <iostream>
#include <fstream>


int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Invalid number of arguments" << std::endl;
        exit(1);
    }

    std::ifstream input_file(argv[1]);
    if (!input_file.is_open())
    {
        std::cerr << "Unable to open file" << std::endl;
        exit(1);
    }

    Linker &linker = Linker::getInstance();
    linker.readFromFile(input_file);

    return 0;
}