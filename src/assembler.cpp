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

<<<<<<< Updated upstream
        cout << " Location: " << location << " Label: " << label << " Opcode: " << opcode << " Operand: " << operand << endl;
=======
        //*cout << "Location: " << location << "Label: " << label << " Opcode: " << opcode << " Operand: " << operand << endl;
        int currentLoc = LOCCTR;
        //Check for "START" assembler directive (Save starting location and go to next line)
        if(opcode == "START"){
            LOCCTR = stoi(operand, nullptr, 16);
            intermfile << hex << uppercase << setw(4) << setfill('0') << LOCCTR << "    "
                << setfill(' ') << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand << endl;
            continue;
        }

        //Check for "END" or "BASE" assembler directive
        else if (opcode == "END" || opcode == "BASE"){ //These opcodes don't use any space, no need to update Location
            intermfile << setw(8) << " " 
                << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand << endl;
            continue;
        }
>>>>>>> Stashed changes

        //Add label to the symbol table with its address(LOCCTR)
        if (!label.empty()) {
            SYMTAB.insert(label, LOCCTR);
        }

<<<<<<< Updated upstream
        intermfile << hex << uppercase << setw(4) << setfill('0') <<
        LOCCTR << " " << label << " " << opcode << " " << operand << endl;


        //Increment location counter (placeholder: assume all instructions are 3 bytes)
        //So if a LOCCTR = 1000 then the next instruction will be 1003
        LOCCTR += 3;
=======
        //Increment location counter 
        //So if a LOCCTR = 1000 then the next instruction will be 1003
        else if (opcode == "BYTE"){
            LOCCTR += (operand[0] == 'X') ? (operand.length() - 3) / 2 : operand.length() - 3;
        }
        else if (opcode == "WORD"){
            LOCCTR += 3;
        }   
        else if (opcode == "RESB"){
            LOCCTR += stoi(operand);
        }
            
        else if (opcode == "RESW"){
            LOCCTR += 3 * stoi(operand);
        }
        else if (!opcode.empty() && opcode[0] == '+'){
            LOCCTR += 4;
        }
        else if (OPTAB.isInstruction(opcode)){
            LOCCTR += OPTAB.getOpcodeInfo(opcode).format[0];
        }
        else 
            cout << "⚠️ Unknown opcode: " << opcode << endl;

        intermfile << setw(4) << setfill('0') << hex << uppercase << currentLoc << "    ";
        intermfile << setfill(' ') << setw(8) << left << label
            << setw(8) << left << opcode
            << setw(10) << left << operand << endl;
        
>>>>>>> Stashed changes
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

    //initalize base register to be used for base relative addressing
    int base;
    while(getline(infile, currentLine)){
        string location = currentLine.substr(0, 4);
        string label = currentLine.substr(8, 8);
        string opcode = currentLine.substr(16, 8);
        string operand = currentLine.substr(24, 10);
        string objcode = "";

<<<<<<< Updated upstream
        //Any line that starts with a '.' is a comment
        if (currentLine.length() > 0 && currentLine[0] == '.'){
=======
        int disp; //Displacement
        int ta; //Target address
        int pc; //Program counter

        //nixbpe flags
        int n = 0;
        int i = 0;
        int x = 0;
        int b = 0;
        int p = 1; //always assume pc-relative
        int e = 0;

        //instruction format
        int format = 0;
        

        //Any line that starts with a '.' is a comment (ignore it)
        if(currentLine.length() > 0 && currentLine[0] == '.'){
>>>>>>> Stashed changes
            continue;
        }

        //This function splits up the line into four parts (location, label, opcode, and operand)
        stringstream ss(currentLine);
        ss >> location >> label >> opcode >> operand;

        //Checks if there is a label for the currentline
        //If there is no label then set the label as empty and make
        //sure the opcode is properly assigned
        if(opcode.empty() && !label.empty()) { 
            opcode = label;
            label = "";
        }

<<<<<<< Updated upstream
=======
        //Skip assembler directives because they don't generate object code
        if(opcode == "START" || opcode == "END" || opcode == "RESW" ||
            opcode == "RESB" || opcode == "WORD" || opcode == "BYTE") {   
            listingFile << setfill(' ') << setw(8) << left << location
                << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand
                << setw(8) << left << objcode << endl;
            continue;
        }

        if(opcode == "BASE"){
            if(!operand.empty()){
                base = SYMTAB.getAddress(operand); 
            }
            listingFile << setfill(' ') << setw(8) << left << location
                << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand
                << setw(8) << left << objcode << endl;
            continue;
        }
        else if(opcode == "WORD") {
            int value = stoi(operand);
            stringstream ss;
            ss << hex << uppercase << setfill('0') << setw(6) << value;
            objcode = ss.str();
        }
        else if(opcode == "BYTE"){
            if(operand[0] == 'C') { //Character constant
                string chars = operand.substr(2, operand.length() - 3);
                for (int i = 0; i < chars.length(); i++) {
                    stringstream ss;
                    ss << hex << uppercase << (int)chars[i];
                    objcode += ss.str();
                }
            }
            else if(operand[0] == 'X'){ //Hexadecimal constant
                objcode = operand.substr(2, operand.length() - 3);
            }
        }

>>>>>>> Stashed changes
        //Temporary variable for the opcode that will be used to get the opcode info from the Opcode Table
        string cleanOpcode = opcode;
        //Temporary variable for the operand that will be used to get the label's address from the Symbol Table
        string cleanOperand = operand;
<<<<<<< Updated upstream
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
=======
>>>>>>> Stashed changes
        
        // n and i flags
        //Check for indirect addressing
        if(!operand.empty()){
            if(operand[0] == '@'){
                n = 1;
                i = 0;
                cleanOperand = operand.substr(1);
            }
            //Check for immediate addressing
            else if(operand[0] == '#'){
                //check if operand is a constant value
                if(isdigit(operand[1])){
                    disp = disp = stoi(operand.substr(1));
                    b = 0; 
                    p = 0;
                }
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
        }
<<<<<<< Updated upstream
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
=======
        
>>>>>>> Stashed changes
        //Checks if the opcode starts with a '+' which indicates extended format
        //if so create a substring that starts after the '+' and assign it to cleanOpcode
        bool isExtended = false;
        if(opcode[0] == '+'){
            isExtended = true;
            cleanOpcode = opcode.substr(1);
            e = 1;
        }

<<<<<<< Updated upstream
        //Checks if the opcode instruction exists in the Opcode Table, if so
        //Add the two digit opcodeHex to the start of the objectcode
        if(OPTAB.isInstruction(opcode)){ 
            OpcodeInfo info = OPTAB.getOpcodeInfo(opcode);
            objcode = info.opcodeHex;
=======
    
        //Assign the correct instruction forma
        if(OPTAB.isInstruction(cleanOpcode)){ 
            OpcodeInfo info = OPTAB.getOpcodeInfo(cleanOpcode);
>>>>>>> Stashed changes

            //Extended format
            if(isExtended){
                format = info.format[1];
            }
            //Default instruction format
            else{
                format = info.format[0];
            }
        }
<<<<<<< Updated upstream

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
=======
        else{
            //If instruction not in Opcode Table, write line to listing file and go to next line (no object code)
            listingFile << setfill(' ') << setw(8) << left << location
                << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand
                << setw(8) << left << objcode << endl;
            continue;
        }

        //Check if base relative or pc relative and calculate displacement
        /*
        int disp;
        int ta; //Target address
        int pc; //Program counter
        */
        //Check to see if the operand is a label or a constant then adjust target address accordingly
        if(p != 0 || b != 0){
            if(!cleanOperand.empty() && isdigit(cleanOperand[0])){
                ta = stoi(cleanOperand);
            }
            else{
                ta = SYMTAB.getAddress(cleanOperand);
                if(ta == -1){ //Label not in symbol table
                    listingFile << setfill(' ') << setw(8) << left << location
                        << setw(8) << left << label
                        << setw(8) << left << opcode
                        << setw(10) << left << operand 
                        << setw(8) << left << objcode << endl;
                    continue; 
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
            else{
                p = 0;
                b = 1;
                disp = ta - base;
            }
        }
        
        //Construct Object code
        OpcodeInfo info = OPTAB.getOpcodeInfo(cleanOpcode); 
        int opcodeInt = stoi(info.opcodeHex, nullptr, 16); //Get the opcode hex value from the Opcode Table
        //Check which format
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
                //Take this out in final code
                listingFile << setfill(' ') << setw(8) << left << location
                    << setw(8) << left << label
                    << setw(8) << left << opcode
                    << setw(10) << left << operand
                    << setw(8) << left << objcode << endl;
                continue;
            }

            opcodeInt = stoi(info.opcodeHex, nullptr, 16);
            int objCode = (opcodeInt << 8) | (r1 << 4) | r2;
            stringstream objCodeStream;
            objCodeStream << hex << uppercase << setfill('0') << setw(4) << objCode;
            objcode = objCodeStream.str();
        }

        else if(format == 3 || format == 4){                                                
            opcodeInt = (opcodeInt & 0xFC) | (n<<1) | i; //first two nibbles, made up by opcode & n/i flags                                         
            int flags = (x << 3) | (b << 2) | (p << 1) | e; //third nibble made up by the xbpe flags

            if(format == 3){
                int objCode = (opcodeInt << 16) | (flags << 12) | (disp & 0xFFF);
                stringstream ss;
                ss << hex << uppercase << setfill('0') << setw(6) << objCode;
                objcode = ss.str();
            }
            else{ //Format 4
                int objCode = (opcodeInt << 24) | (flags << 20) | (ta & 0xFFFFF);
                stringstream ss;
                ss << hex << uppercase << setfill('0') << setw(8) << objCode;
                objcode = ss.str();
            }
        }
        
        //Write to listing file
        listingFile << setfill(' ') << setw(8) << left << location
            << setw(8) << left << label
            << setw(8) << left << opcode
            << setw(10) << left << operand 
            << setw(8) << left << objcode << endl;   
>>>>>>> Stashed changes
    }
  
}