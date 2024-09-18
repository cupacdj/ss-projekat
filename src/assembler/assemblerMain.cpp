#include "assembler.hpp"
#include <iostream>
#include <fstream>
#include "parser.tab.hpp"

extern int yyparse();
extern FILE *yyin;

int main(int argc, char *argv[])
{
    std::string input_filename, output_filename;

    for (int i = 1; i < argc; i++)
    {
        if (std::string(argv[i]) == "-o")
        {
            if (i + 1 < argc)
            {
                output_filename = argv[i + 1];
                i++;
            }
            else
            {
                std::cout << "Neispravno koriscenje opcije -o" << std::endl;
                return -1;
            }
        }
        else
        {
            input_filename = argv[i];
        }
    }

    if (input_filename.empty())
    {
        std::cout << "Nedostaje ulazni fajl" << std::endl;
        return -1;
    }

    if (output_filename.empty())
    {
        std::cout << "Nedostaje izlazni fajl" << std::endl;
        return -1;
    }

    std::ofstream output_file(output_filename, std::ios::binary);

    if (!output_file.is_open())
    {
        std::cout << "Neuspesno otvaranje fajla " << argv[2] << std::endl;
        return -1;
    }

    FILE *input_file = fopen(input_filename.c_str(), "r");
    if (!input_file)
    {
        std::cout << "Neuspesno otvaranje fajla " << argv[3] << std::endl;
        return -1;
    }

    yyin = input_file;
    try
    {
        yyparse();
    }
    catch (const char *msg)
    {
        std::cout << msg << std::endl;
        fclose(input_file);
        output_file.close();
        return -1;
    }

    fclose(input_file);

    Assembler &asmblr = Assembler::getInstance();

    asmblr.fixRelocations();

    asmblr.writeToFile(output_file);

    output_file.close();

    // asmblr.printTables();

    asmblr.makeTextFile(argv[2]);

    return 0;
}
