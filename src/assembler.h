// declarations

//Assembler decleration
#ifndef ASSEMBLER_H
#define ASSEMBLER_H
#include <string>

using namespace std;

class Assembler {
public:
    Assembler(const string& filename);
    string passOne();
    void passTwo(const string& interimfile, SymbolTable& SYMTAB); // Don't need to touch this was incorporated for you

private:
    string sourceFile;
};

#endif