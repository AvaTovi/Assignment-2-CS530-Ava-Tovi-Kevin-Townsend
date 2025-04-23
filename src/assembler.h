// declarations

//Assembler decleration
#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <string>
#include "symtab.h"

using namespace std;

//Helper class to store public functions that are used throughout the code
class Assembler {
public:
    Assembler(const string& filename);
    string passOne();
    string passTwo(const string& interimfile); // Don't need to touch this I incorporated it for you

//these are private functions used only in certain areas
private:
    string sourceFile;
    SymbolTable SYMTAB;
};

#endif