#include "linker.hpp"
#include <iostream>
#include <fstream>
#include <string>

int main(int argc, char *argv[])
{
    bool hex = false;
    bool place = false;
    bool out = false;
    std::string output;
    std::string argument;
    Linker &linker = Linker::getInstance();
    std::vector<std::string> files;
    std::map<std::string, uint32_t> sectionAddresses;
    

    if (argc < 3)
    {
        std::cout << "Potrebno je 3 ili vise argumenta a data su: " << argc - 1 << "!" << std::endl;
        return -1;
    }

    for (int i = 1; i < argc; i++)
    {
        argument = std::string(argv[i]);
        if (argument == "-hex")
        {
            hex = true;
            continue;
        }

        if (argument == "-o")
        {
            output = std::string(argv[i + 1]);
            out = true;
            i++;
            continue;
        }
        
        if (argument.find("-place=") == 0)
        {
            std::string place = argument.substr(7);
            size_t monkey = place.find('@');
            if (monkey != std::string::npos)
            {
                std::string section = place.substr(0, monkey);
                std::string addressStr = place.substr(monkey + 1);
                uint32_t address = std::stoul(addressStr, nullptr, 16); 
                
                if (sectionAddresses.find(section) != sectionAddresses.end())
                {
                    std::cerr << "Greška: Sekcija " << section << " je navedena više puta." << std::endl;
                    exit(-1);
                }

                sectionAddresses[section] = address;
            }
            else
            {
                std::cout << "Greška u -place argumentu: " << argument << std::endl;
                exit(-1);
            }
            continue;
        }

        files.push_back(argument);
    }

    if (!hex)
    {
        std::cout << "Nije pronadjena -hex" << std::endl;
        exit(-1);
    }

    if (!out)
    {
        std::cout << "Nije pronadjen -o" << std::endl;
        exit(-1);
    }

    linker.readFiles(files);
    
    linker.placeSections(sectionAddresses, files);

    linker.relocation(sectionAddresses);
    
    std::ofstream outfile(output, std::ios::binary);
    linker.writeToFile(outfile, sectionAddresses);

    linker.makeTextFile(output);

    return 0;
}