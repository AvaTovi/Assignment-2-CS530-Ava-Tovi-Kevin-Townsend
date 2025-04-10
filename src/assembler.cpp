// class that involves the two passes

#include "assembler.h"
#include "symbol_table.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// Constructor: save the source file name for later use
Assembler::Assembler(const string& filename) {
    sourceFile = filename;
}

// Pass 1: Build symbol table and calculate addresses
void Assembler::passOne() {
    // Open the input .sic file
    ifstream infile(sourceFile.c_str());
    // Error message if the .sic file cant be found
    if (!infile) {
        cout << "Error: Could not open file " << sourceFile << endl;
        return;
    }

    //Creating a symbol table and a line we will use to read the file
    SymbolTable SYMTAB;
    string line;

    //Location counter starts at 0 --placeholder--
    int LOCCTR = 0;

    while (getline(infile, line)) {
        string label = "";
        string opcode = "";
        string operand = "";

        if (line.length() > 0 && line[0] == '.'){
            contine;
        }
    }
}