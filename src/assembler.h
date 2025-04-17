// declarations
#include "symtab.h"

//Assembler decleration
#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <string>

using namespace std;

class Assembler {
public:
    Assembler(const string& filename);
    string passOne();
    string passTwo(const string& interimfile); // Don't need to touch this was incorporated for you

private:
    string sourceFile;
    SymbolTable SYMTAB;
};

#endif