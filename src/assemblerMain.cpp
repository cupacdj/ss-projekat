#include <iostream>
#include <fstream>
#include "parser.tab.hpp"

extern int yyparse();
extern FILE *yyin;
std::ofstream output_file;

using namespace std;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        cout << "Potrebno je 3 argumenta a data su: " << argc - 1 << "!" << endl;
        return -1;
    }

    output_file = ofstream(argv[2]);

    FILE *input_file = fopen(argv[3], "r");
    if (!input_file)
    {
        cout << "Neuspesno otvaranje fajla " << argv[3] << endl;
        return -1;
    }

    yyin = input_file;
    try {
    yyparse();
    } catch (const char* msg) {
        cout << msg << endl;
        fclose(input_file);
        output_file.close();
        return -1;
    }

    fclose(input_file);

    // fix relocations of local symbols
    // Assembler::getInstance().writeToFile(output_file);

    output_file.close();
    return 0;
}