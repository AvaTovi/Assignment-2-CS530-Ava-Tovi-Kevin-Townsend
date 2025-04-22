//class that involves the two passes

#include "assembler.h"
#include "symtab.h"
#include "opcodetab.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <algorithm>

using namespace std;

// Helper: remove leading/trailing whitespace
string trim(const string& str) {
    size_t first = str.find_first_not_of(" \t");
    size_t last  = str.find_last_not_of(" \t");
    return (first == std::string::npos) ? "" : str.substr(first, last - first + 1);
}
//Constructor: save the source file name for later use
Assembler::Assembler(const string& filename) {
    sourceFile = filename;
}

//Pass 1: Build symbol table and calculate addresses
string Assembler::passOne() {
    //Use opcode table to get correct instruction formats and determine byte length.
    OpcodeTab OPTAB;

    //Open the input .sic file
    ifstream infile(sourceFile.c_str());
    //Error message if the .sic file cant be found
    if (!infile) {
        cout << "Error: Could not open file! " << sourceFile << endl;
        return "";
    }

    //This creates the intermediate file by replacing the ".sic" with ".interm"
    string intermFileName = sourceFile.substr(0, sourceFile.length() - 4) + ".interm";
    ofstream intermfile(intermFileName);
    //Error message if can't do it
    if (!intermfile) {
        cout << "⚠️  Could not create intermediate file: " << intermFileName << endl;
        return "";
    }

    //Creating a symbol table and a line we will use to read the file
    SYMTAB = SymbolTable();
    string currentLine;

    //Location counter starts at 0 --placeholder--
    int LOCCTR = 0;

    while (getline(infile, currentLine)) {
        string location = "";

        //This function treats anything that starts with a '.' as a comment
        //and skips it, it skips it and doesnt try to parse it for labels, opcodes,
        //or operands.
        if (currentLine.length() > 0 && currentLine[0] == '.'){
            continue;
        }

        //This function splits up the line into three parts (label, opcode, and operand)
        stringstream ss(currentLine);
        string p1, p2, p3;
        ss >> p1 >> p2 >> p3;

        string label = "", opcode = "", operand = "";

        //This handles lines without a label
        // If the code has 3 parts then it will use the label opcode operand
        if (!p3.empty()) {
            label = p1;
            opcode = p2;
            operand = p3;
        }
        // If the code has 2 parts then it will use the opcode operand
        else if (!p2.empty()) {
            opcode = p1;
            operand = p2;
        }
        // If 1 it is 1 part then it's just an opcode
        else {
            opcode = p1;
        }

        cout << "Location: " << location << "Label: " << label << " Opcode: " << opcode << " Operand: " << operand << endl;

        //This function skips START,END,BASE directives
        if (!label.empty() &&
        opcode != "START" &&
        opcode != "END"   &&
        opcode != "BASE") {
        SYMTAB.insert(label, LOCCTR);
    }

        intermfile << setw(4) << setfill('0') << hex << uppercase << LOCCTR
           << "  " << label << " " << opcode << " " << operand << endl;

        //Increment location counter 
        //So if a LOCCTR = 1000 then the next instruction will be 1003
        if (opcode == "START") {
            LOCCTR = stoi(operand, nullptr, 16);
        }
        else if (opcode == "BYTE")
            LOCCTR += (operand[0] == 'X') ? (operand.length() - 3) / 2 : operand.length() - 3;
        else if (opcode == "WORD")
            LOCCTR += 3;
        else if (opcode == "RESB")
            LOCCTR += stoi(operand);
        else if (opcode == "RESW")
            LOCCTR += 3 * stoi(operand);
        else if (!opcode.empty() && opcode[0] == '+')
            LOCCTR += 4;
        else if (OPTAB.isInstruction(opcode))
            LOCCTR += OPTAB.getOpcodeInfo(opcode).format[0];
    }
    //This closes the file when we are done writing to it
    intermfile.close();

    //this will wite the SYMTAB to the file after all the lines have been read.
    //.st is the extension for SymbolTable
    string SYMTABFileName = sourceFile.substr(0, sourceFile.length() - 4) + ".st";
    SYMTAB.writeToFile(SYMTABFileName);
    // Debug: print symbol table to console
    cout << "-- SYMBOL TABLE DUMP --" << endl;
    SYMTAB.printAll(); 
    cout << "-----------------------" << endl;
    
    cout << sourceFile << " --Pass 1 Complete-- " << endl;

    return intermFileName;
}

// Pass 2 will go here:
//Pass two takes the intermediate file from pass one as a parameter
string Assembler::passTwo(const string& intermfile){
    // TODO: Implement Pass 2 (translate to machine code, write .l file)

    //Use Opcode table to assist in generating object code
    OpcodeTab OPTAB;

    //Open the intermediate file recieved from pass one
    ifstream infile(intermfile.c_str());

    //Create and open the listing file
    string listingFileName = sourceFile.substr(0, sourceFile.length() - 4) + ".l";
    ofstream listingFile(listingFileName);

    //currentline is an iterator, that will go through the file line by line
    string currentLine;
    int base = 0;

    while(getline(infile, currentLine)){
        string location = "";
        string label = "";
        string opcode = "";
        string operand = "";
        string objcode = "";

        //Any line that starts with a '.' is a comment (ignore it)
        if (currentLine.length() > 0 && currentLine[0] == '.'){
            continue;
        }

        // --- Split by tokens to handle missing labels cleanly -------
        vector<string> parts;
        string field;
        stringstream ss(currentLine);
        while (ss >> field) {
            parts.push_back(field);
        }

        if (parts.size() == 4) {
            location = parts[0];
            label    = parts[1];
            opcode   = parts[2];
            operand  = parts[3];
        }
        else if (parts.size() == 3) {
            location = parts[0];
            label    = "";
            opcode   = parts[1];
            operand  = parts[2];
        }
        else if (parts.size() == 2) {
            location = parts[0];
            label    = ""; 
            opcode   = parts[1];
            operand  = "";  
        }
        else {
            continue;             
        }

        // trim whitespace on each field
        label   = trim(label);
        opcode  = trim(opcode);
        operand = trim(operand);


        //This handles BASE directive
        if (opcode == "BASE") {
            base = SYMTAB.getAddress(operand);   // store base register value
            if (base == -1)
                cout << "⚠️  BASE label not found: " << operand << endl;
            continue;          // skip further processing of this line
        }

        // Strip leading '+' for extended format, then check if it's really an instruction
        string cleanOp = (opcode[0] == '+') ? opcode.substr(1): opcode;

        //Temporary variable for the opcode that will be used to get the opcode info from the Opcode Table
        string cleanOpcode = opcode;
        //Temporary variable for the operand that will be used to get the label's address from the Symbol Table
        string cleanOperand = operand;

        //Addressing modes
        int n = 0;
        int i = 0;
        int x = 0;
        int b = 0;
        int p = 0;
        int e = 0;
        
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

        //Skip assembler directives because they don't generate opcode


        //Assign the correct instruction format
        int format = 0;
        if(OPTAB.isInstruction(cleanOpcode)){ 
            OpcodeInfo info = OPTAB.getOpcodeInfo(cleanOpcode);

            //Extended format
            if(isExtended){
                format = info.format[1];
            }
            //Default instruction format
            else{
                format = info.format[0];
            }
        } 
        else {
            //If instruction not in Opcode Table, write line to listing file and go to next line (no object code)
            listingFile << setw(6) << location << "  "
            << setw(8) << label
            << setw(8) << opcode
            << setw(10) << operand
            << objcode << endl;
            continue;
        }

        // Special‑case format 3, no‑operand (Ex: RSUB)
        if (format == 3 && operand.empty()) {
            // build the two‑nibble opcode + n/i bits (n=1,i=1)
            int opInt = stoi(OPTAB.getOpcodeInfo(cleanOpcode).opcodeHex, nullptr, 16);
            opInt = (opInt & 0xFC) | (1<<1) | 1;
            // flags nibble = 0 (no xbpe bits)
            int flags = 0;
            // zero displacement
            int obj = (opInt << 16) | (flags << 12) | 0;
            stringstream ss;
            ss << hex << uppercase << setfill('0') << setw(6) << obj;
            objcode = ss.str();

            // write it out and skip the generic logic
            listingFile << location << "    "
                        << label    << "    "
                        << opcode   << "    "
                        << operand  << "    "
                        << objcode  << endl;
            continue;
        }

        //Check if base relative or pc relative and calculate displacement
        int disp;
        int ta; //Target address
        int pc; //Program counter

        //Check to see if the operand is a label or a constant then adjust target address accordingly
        if(!cleanOperand.empty() && isdigit(cleanOperand[0])){
            ta = stoi(cleanOperand);
        }
        else{
            ta = SYMTAB.getAddress(cleanOperand); 
            if (ta == -1) {
                cerr << "Error: undefined symbol " << cleanOperand << " at address " 
                          << location << endl;
            //when we set ta=0 the assembler will continue
            ta=0;
            }
        }
        pc = stoi(location, nullptr, 16) + format; 
        int diff = ta - pc;
        //Pc relative
        if(diff >= -2048 && diff <= 2047){
            p = 1;
            b = 0;
            disp = diff;
        }
        //Base relative
        else {
            p = 0;
            b = 1;
        
            if (SYMTAB.getAddress("BASE") == -1) {
                cout << "⚠️  BASE label not found. Cannot apply base-relative addressing." << endl;
                continue;
            }
        
            int base = SYMTAB.getAddress("BASE");
            disp = ta - base;
        }

        //Construct Object code
        OpcodeInfo info = OPTAB.getOpcodeInfo(cleanOpcode); 
        int opcodeInt = stoi(info.opcodeHex, nullptr, 16); //Get the opcode hex value from the Opcode Table
                                                           //And convert it to a hex integer
        opcodeInt = (opcodeInt & 0xFC) | (n<<1) | i; //first two nibbles, made up by opcode & n/i flags
                                                    
        int flags = (x << 3) | (b << 2) | (p << 1) | e; //third nibble made up by the xbpe flags

        //Check if format 3 or format 4
        if(format == 1){
            objcode = info.opcodeHex;
        }
        else if(format == 2){
            stringstream ss(operand);
            string reg1 = "";
            string reg2 = "";
            getline(ss, reg1, ',');
            getline(ss, reg2);

            int r1 = getRegisterNum(reg1);
            int r2 = 0;
            if(!reg2.empty()){
                r2 = getRegisterNum(reg2);
            }

            //Error handling, in the case of invalid register
            if (r1 == -1 || (!reg2.empty() && r2 == -1)) {
                continue;
            }

            opcodeInt = stoi(info.opcodeHex, nullptr, 16);
            int objCode = (opcodeInt << 8) | (r1 << 4) | r2;
            stringstream objCodeStream;
            objCodeStream << hex << uppercase << setfill('0') << setw(4) << objCode;
            objcode = objCodeStream.str();
        }

        else if(format == 3){ //format 3 (last 3 nibbles)
            int objCode = (opcodeInt << 16) | (flags << 12) | (disp & 0xFFF);
            stringstream ss;
            ss << hex << uppercase << setfill('0') << setw(6) << objCode;
            objcode = ss.str();
        }
        else if(format == 4){ //format 4 (last 5 nibbles)
            int objCode = (opcodeInt << 24) | (flags << 20) | (ta & 0xFFFFF);
            stringstream ss;
            ss << hex << uppercase << setfill('0') << setw(8) << objCode;
            objcode = ss.str();
        }
        
        //Write to listing file
        listingFile << location << "    " << label    << "    " << opcode   << "    " << operand  << "    " << objcode  << "\n";        
        
    }
    listingFile.close();

    return listingFileName; 
}