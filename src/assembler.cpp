//class that involves the two passes

#include "assembler.h"
#include "symtab.h"
#include "opcodetab.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace std;

//Constructor: save the source file name for later use
Assembler::Assembler(const string & filename) {
    sourceFile = filename;
}

//Pass 1: Build symbol table and calculate addresses
// --YOU FORGOT THE INTERMFILE, INITIALIZE THE INTERMFILE I PASSED IN PASSTWO--
void Assembler::passOne() {
    //Open the input .sic file
    ifstream infile(sourceFile.c_str());

    //Error message if the .sic file cant be found
    if (!infile) {
        cout << "Error: Could not open file! " << sourceFile << endl;
        return;
    }

    //This creates the intermediate file by replacing the ".sic" with ".interm"
    string intermFileName = sourceFile.substr(0, sourceFile.length() - 4) + ".interm";
    ofstream intermfile(intermFileName);
    //Error message if can't do it
    if (!intermfile) {
        cout << "⚠️  Could not create intermediate file: " << intermFileName << endl;
        return;
    }

    //Creating a symbol table and a line we will use to read the file
    SymbolTable SYMTAB;
    string currentLine;
    //Location counter starts at 0 --placeholder--
    int LOCCTR = 0;

    while (getline(infile, currentLine)) {
        string location = "";
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

        cout << " Location: " << location << " Label: " << label << " Opcode: " << opcode << " Operand: " << operand << endl;

        //Add label to the symbol table with its address(LOCCTR)
        if (!label.empty()) {
            SYMTAB.insert(label, LOCCTR);
        }

        intermfile << hex << uppercase << setw(4) << setfill('0') <<
        LOCCTR << " " << label << " " << opcode << " " << operand << endl;


        //Increment location counter (placeholder: assume all instructions are 3 bytes)
        //So if a LOCCTR = 1000 then the next instruction will be 1003
        LOCCTR += 3;
    }
    //This closes the file when we are done writing to it
    intermfile.close();
    //this will wite the SYMTAB to the file after all the lines have been read.
    //.st is the extension for SymbolTable
    SYMTAB.writeToFile(sourceFile + ".st");

    cout << sourceFile << " --Pass 1 Complete-- " << endl;
}

// Pass 2 will go here:
//Pass two takes the intermediate file and symbol table from pass one as parameters
void Assembler::passTwo(const string& intermfile, const SymbolTable& SYMTAB){
    // TODO: Implement Pass 2 (translate to machine code, write .l file)
    OpcodeTab OPTAB;

    //Open the intermediate file recieved from pass one
    ifstream infile(intermfile.c_str());

    //currentline is an iterator, that will go through the file line by line
    string currentLine;

    while(getline(infile, currentLine)){
        string location = "";
        string label = "";
        string opcode = "";
        string operand = "";
        string objcode = "";

        //Any line that starts with a '.' is a comment
        if (currentLine.length() > 0 && currentLine[0] == '.'){
            continue;
        }

        //This function splits up the line into four parts (location, label, opcode, and operand)
        stringstream ss(currentLine);
        ss >> location >> label >> opcode >> operand;

        //Checks if there is a label for the currentline
        //If there is no label then set the label as empty and make
        //sure the opcode is properly assigned
        if (opcode.empty() && !label.empty()) { 
            opcode = label;
            label = "";
        }

        //Temporary variable for the opcode that will be used to get the opcode info from the Opcode Table
        string cleanOpcode = opcode;
        //Temporary variable for the operand that will be used to get the label's address from the Symbol Table
        string cleanOperand = operand;
        //Instruction format
        int format;
        //Addressing modes
        int n = 0;
        int i = 0;
        int x = 0;
        int b = 0;
        int p = 0;
        int e = 0;
        //displacement
        int disp; 
        
        // n and i flags
        //Check for indirect addressing
        if(operand[0] == '@'){
            n = 1;
            i = 0;
            cleanOperand = operand.substr(1);
        }
        //Check for immediate addressing
        else if(operand[0] == '#'){
            n = 0;
            i = 1;
            cleanOperand = operand.substr(1);
        }
        //If the n and i flags are both still 0, then the addressing mode is simple addressing
        else{
            n = 1;
            i = 1;
        }
        //Check for indexed addressing
        if(cleanOperand.find(",X") != string::npos){
            x = 1;
            cleanOperand = cleanOperand.substr(0, cleanOperand.find(",X"));
        }
        //Checks if the opcode starts with a '+' which indicates extended format
        //if so create a substring that starts after the '+' and assign it to cleanOpcode
        bool isExtended = false;
        if(opcode[0] == '+'){
            isExtended = true;
            cleanOpcode = opcode.substr(1);
            e = 1;
        }

        //Checks if the opcode instruction exists in the Opcode Table, if so
        //Add the two digit opcodeHex to the start of the objectcode
        if(OPTAB.isInstruction(opcode)){ 
            OpcodeInfo info = OPTAB.getOpcodeInfo(opcode);
            objcode = info.opcodeHex;

            //Extended format
            if(isExtended){
                format = info.format[1];
            }
            //Default instruction format
            else{
                format = info.format[0];
            }
        }

        //Check if base relative or pc relative
        int ta = stoi(SYMTAB.getAddress(cleanOperand), nullptr, 16); //Target address
        int pc = stoi(location, nullptr, 16) + format; //Program counter
        int diff = ta - pc;
        if(diff >= -2048 && diff <= 2047){
            p = 1;
            b = 0;
            disp = diff;
        }
        else{
            p = 0;
            b = 1;
        }
        
        cout << "Location: " << location <<  " Label: " << label << " Opcode: " << opcode << " Operand: " << operand << "Object Code: " << objcode << endl;
    }
  
}