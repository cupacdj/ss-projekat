#include "assembler.hpp"
#include <iostream>
#include <fstream>
#include "parser.tab.hpp"

extern int yyparse();
extern FILE *yyin;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Potrebno je 3 argumenta a data su: " << argc - 1 << "!" << std::endl;
        return -1;
    }

    std::ofstream output_file(argv[2], std::ios::binary);

    if (!output_file.is_open())
    {
        std::cout << "Neuspesno otvaranje fajla " << argv[2] << std::endl;
        return -1;
    }

    FILE *input_file = fopen(argv[3], "r");
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

    asmblr.printTables();

    output_file.close();


    return 0;
}
