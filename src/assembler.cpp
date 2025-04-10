//class that involves the two passes

#include "assembler.h"
#include "symtab.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

//Constructor: save the source file name for later use
Assembler::Assembler(const string& filename) {
    sourceFile = filename;
}

//Pass 1: Build symbol table and calculate addresses
void Assembler::passOne() {
    //Open the input .sic file
    ifstream infile(sourceFile.c_str());
    //Error message if the .sic file cant be found
    if (!infile) {
        cout << "Error: Could not open file! " << sourceFile << endl;
        return;
    }

    //Creating a symbol table and a line we will use to read the file
    SymbolTable SYMTAB;
    string currentLine;

    //Location counter starts at 0 --placeholder--
    int LOCCTR = 0;

    while (getline(infile, currentLine)) {
        string label = "";
        string opcode = "";
        string operand = "";

        //This function treats anything that starts with a '.' as a comment
        //and skips it, it skips it and doesnt try to parse it for labels, opcodes,
        //or operands.
        if (currentLine.length() > 0 && currentLine[0] == '.'){
            continue;
        }

        //This function splits up the line into three parts (label, opcode, and operand)
        stringstream ss(currentLine);
        ss >> label >> opcode >> operand;

        //This handles lines without a label
        if (opcode.empty() && !label.empty()) {
            opcode = label;
            label = "";
        }

        cout << "Label: " << label << " Opcode: " << opcode << " Operand: " << operand << endl;

        //Add label to the symbol table with its address(LOCCTR)
        if (!label.empty()) {
            SYMTAB.insert(label, LOCCTR);
        }

        //Increment location counter (placeholder: assume all instructions are 3 bytes)
        //So if a LOCCTR = 1000 then the next instruction will be 1003
        LOCCTR += 3;
    }
    //this will wite the SYMTAB to the file after all the lines have been read.
    //.st is the extension for SymbolTable
    SYMTAB.writeToFile(sourceFile + ".st");

    cout << sourceFile << " --Pass 1 Complete-- " << endl;
}

// Pass 2 will go here:
void Assembler::passTwo() {
    // TODO: Implement Pass 2 (translate to machine code, write .l file)
}