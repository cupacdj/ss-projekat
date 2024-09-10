#include <cstdio>
#include <fstream> 
#include <iomanip>
#include <iostream>
#include "assembler.hpp"
#include "parser.tab.hpp"

extern int yyparse();
extern FILE *yyin;
std::ofstream output_file; 

using namespace std;

int main(int argc, char *argv[]) {
    if (argc != 4) {
        cout << "Expected 3 arguments, got " << argc - 1 << "!" << endl;
        return -1;
    }

    output_file = ofstream(argv[2]); 

    FILE *input_file = fopen(argv[3], "r");
    if (!input_file) {
        cout << "Failed to open file " << argv[3] << endl;
        return -1;
    }

    yyin = input_file;
    yyparse();
    fclose(input_file);

    //Assembler::getInstance().writeToFile(output_file);

    output_file.close();
    return 0;
}

void Assembler::label(std::string label) {
    cout << label << ":" << endl;
}

void Assembler::parseHalt() {
    cout << "halt" << endl;
}

void Assembler::parseInt() {
    cout << "int" << endl;
}

void Assembler::parseIret() {
    cout << "iret" << endl;
}

void Assembler::parseCall(Operand* op) {
    cout << "call " << op << endl;
}

void Assembler::parseRet() {
    cout << "ret" << endl;
}

void Assembler::parseJump(uint8_t gpr1, uint8_t gpr2, Operand* op, JumpType type) {
    cout << "jmp " << "r" << (int)gpr1 << ", r" << (int)gpr2 << ", " << op << endl;
}

void Assembler::parsePush(uint8_t gpr) {
    cout << "push " << "r" << (int)gpr << endl;
}

void Assembler::parsePop(uint8_t gpr) {
    cout << "pop " << "r" << (int)gpr << endl;
}

void Assembler::parseXchg(uint8_t gprS, uint8_t gprD) {
    cout << "xchg " << "r" << (int)gprS << ", r" << (int)gprD << endl;
}

void Assembler::parseArith(uint8_t gprS, uint8_t gprD, ArithType type) {
    cout << "arith " << "r" << (int)gprS << ", r" << (int)gprD << ", " << endl;
}

void Assembler::parseLogi(uint8_t gprS, uint8_t gprD, LogiType type) {
    cout << "logi " << "r" << (int)gprS << ", r" << (int)gprD << ", "  << endl;
}

void Assembler::parseShl(uint8_t gprS, uint8_t gprD) {
    cout << "shl " << "r" << (int)gprS << ", r" << (int)gprD << endl;
}

void Assembler::parseShr(uint8_t gprS, uint8_t gprD) {
    cout << "shr " << "r" << (int)gprS << ", r" << (int)gprD << endl;
}

void Assembler::parseLd(Operand* op, uint8_t gpr) {
    cout << "ld " << op << ", r" << (int)gpr << endl;
}

void Assembler::parseSt(uint8_t gpr, Operand* op) {
    cout << "st " << "r" << (int)gpr << ", " << op << endl;
}

void Assembler::parseCsrrd(uint8_t csr, uint8_t gpr) {
    cout << "csrrd " << "r" << (int)csr << ", r" << (int)gpr << endl;
}

void Assembler::parseCsrwr(uint8_t gpr, uint8_t csr) {
    cout << "csrwr " << "r" << (int)gpr << ", r" << (int)csr << endl;
}

void Assembler::parseGlobal(std::vector <std::string> symbols) {
    cout << ".global ";
    for (int i = 0; i < symbols.size(); i++) {
        cout << symbols[i];
        if (i != symbols.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

void Assembler::parseSection(std::string section) {
    cout << ".section " << section << endl;
}

void Assembler::parseWord(std::vector <std::string> symbols) {
    cout << ".word ";
    for (int i = 0; i < symbols.size(); i++) {
        cout << symbols[i];
        if (i != symbols.size() - 1) {
            cout << ", ";
        }
    }
    cout << endl;
}

void Assembler::parseSkip(uint8_t num) {
    cout << ".skip " << (int)num << endl;
}

void Assembler::parseEnd() {
    cout << ".end" << endl;
}

