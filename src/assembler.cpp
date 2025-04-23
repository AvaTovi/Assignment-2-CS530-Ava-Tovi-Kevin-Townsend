//class that involves the two passes
//Ava did passOne and Kevin did passTwo
//And for debugging we both helped on both passOne and passTwo

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

        if (opcode == "START") {
            LOCCTR = stoi(operand, nullptr, 16);
        }

        intermfile << setw(4) << setfill('0') << hex << uppercase << LOCCTR
           << "  " << label << " " << opcode << " " << operand << endl;

        //This will adjust the LOCCTR based on which ever directive or instruction it 
        //is out of these; which ever format it uses
        if (opcode == "BYTE")
            // If it is 'X' then that means the it is 1 byte
            LOCCTR += (operand[0] == 'X') ? (operand.length() - 3) / 2 : operand.length() - 3;
        else if (opcode == "WORD")
            // WORD is 3 bytes
            LOCCTR += 3;
        else if (opcode == "RESB")
            //This means that RESB is going to reserve the specified number of bytes
            LOCCTR += stoi(operand);
        else if (opcode == "RESW")
            // This means RESW will multiply the 3 bytes by the integer number and use the 
            //outcome
            LOCCTR += 3 * stoi(operand);
        else if (!opcode.empty() && opcode[0] == '+')
            // If it is a '+' that means it is 4 bytes.
            LOCCTR += 4;
        else if (OPTAB.isInstruction(opcode))
            //All of the other SIC/XE instructions will use their default format size
            //v This is what will give the byte length at the end
            LOCCTR += OPTAB.getOpcodeInfo(opcode).format[0];
    }
    //This closes the file when we are done writing to it
    intermfile.close();

    //this will write the SYMTAB to the file after all the lines have been read.
    //.st is the extension for SymbolTable
    string SYMTABFileName = sourceFile.substr(0, sourceFile.length() - 4) + ".st";
    SYMTAB.writeToFile(SYMTABFileName);
    // Debug: print symbol table to console
    cout << "-- INFO PRINTED TO SYMTAB --" << endl;
    SYMTAB.printAll(); 
    
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

    //this will read each line and it will reset all the parts
    //so no old data is being used when it is getting parsed
    while(getline(infile, currentLine)){
        string location = "";
        string label = "";
        string opcode = "";
        string operand = "";
        string objcode = "";

        //nixbpe flags
        //indriect addressing
        int n = 0;
        //immediate addressing 
        int i = 0;
        //indexed addressing
        int x = 0;
        //base-relative
        int b = 0;
        //PC relative
        int p = 1;
        //extended format
        int e = 0;

        //Target Address
        int ta;
        //Program Counter
        int pc;
        //displacement
        int disp;

        //insruction format
        int format = 0; 

        //Any line that starts with a '.' is a comment (ignore it)
        if (currentLine.length() > 0 && currentLine[0] == '.'){
            continue;
        }

        //this will split the current line into individual words
        vector<string> parts;
        string field;
        stringstream ss(currentLine);

        //this grabs each word from the line and alligns it with the correct parts
        while (ss >> field) {
            parts.push_back(field);
        }

        //This assigns the right amount of fields in the code based on how many instructions
        //the line is using
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
            //When not a valid line it will skip it
            continue;             
        }

        // trim whitespace on each field
        label   = trim(label);
        opcode  = trim(opcode);
        operand = trim(operand);

        //This will handle just printing these out in the columns and skipping the rest
        if (opcode == "START" || opcode == "END" || opcode == "RESW" ||
            opcode == "RESB") {   
                listingFile << setfill(' ') << setw(8) << left << location
                    << setw(8) << left << label
                    << setw(8) << left << opcode
                    << setw(10) << left << operand 
                    << setw(8) << left << objcode << endl;
            // go on to the next line without doing object‑code work
            continue;
        }

        //This handles BASE directive
        if (opcode == "BASE") {
            //this will store the base register value
            base = SYMTAB.getAddress(operand);
            if (base == -1)
                cout << "⚠️  BASE label not found: " << operand << endl;
            listingFile << setfill(' ') << setw(8) << left << location
                << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand
                << setw(8) << left << objcode << endl;
             // skip further processing of this line
            continue;         
        }
        //If this is a 'WORD' directive, then it will turn the decimal value into a 
        //3-byte hex string
        else if(opcode == "WORD") {
            int value = stoi(operand);
            stringstream ss;
            ss << hex << uppercase << setfill('0') << setw(6) << value;
            objcode = ss.str();
        }
        //This will handle 'BYTE' directives with character constants'C'
        //It will initially convrt each character into its two-digit hex code
        else if(opcode == "BYTE"){
            //Character constant
            if(operand[0] == 'C') { 
                string chars = operand.substr(2, operand.length() - 3);
                for (int i = 0; i < chars.length(); i++) {
                    stringstream ss;
                    ss << hex << uppercase << (int)chars[i];
                    objcode += ss.str();
                }
            }
            //Hexadecimal constant
            else if(operand[0] == 'X'){ 
                objcode = operand.substr(2, operand.length() - 3);
            }
        }

        ////Temporary variable for the opcode that will be used to get the opcode info from the Opcode Table
        string cleanOpcode = opcode;
        
        //Temporary variable for the operand that will be used to get the label's address from the Symbol Table
        string cleanOperand = operand;

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
                    disp = stoi(operand.substr(1));
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
        
        //Checks if the opcode starts with a '+' which indicates extended format
        //if so create a substring that starts after the '+' and assign it to cleanOpcode
        bool isExtended = false;
        if(opcode[0] == '+'){
            isExtended = true;
            cleanOpcode = opcode.substr(1);
            e = 1;
        }

    
        //Assign the correct instruction format
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
        else{
            //If instruction not in Opcode Table, write line to listing file and go to next line (no object code)
            listingFile << setfill(' ') << setw(8) << left << location
                << setw(8) << left << label
                << setw(8) << left << opcode
                << setw(10) << left << operand
                << setw(8) << left << objcode << endl;
            continue;
        }

        //Check to see if the operand is a label or a constant then adjust target address if needed
        if(p != 0 || b != 0){
            if(!cleanOperand.empty() && isdigit(cleanOperand[0])){
                ta = stoi(cleanOperand);
            }
            else{
                ta = SYMTAB.getAddress(cleanOperand);
                //Label not in symbol table
                if(ta == -1){ 
                    listingFile << setfill(' ') << setw(8) << left << location
                        << setw(8) << left << label
                        << setw(8) << left << opcode
                        << setw(10) << left << operand 
                        << setw(8) << left << objcode << endl;
                    continue; 
                } 
            }
            //This will compute the next program counter
            //It turns the hex location string into an int and adds the 
            //instructions byte length
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
        //Get the opcode hex value from the Opcode Table
        int opcodeInt = stoi(info.opcodeHex, nullptr, 16); 
        //Check which format it is
        if(format == 1){
            objcode = info.opcodeHex;
        }
        //This is format 2 and it will use two registers
        //R1 and R2 from the operand
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
                //This is for the printing, then it will continue onto the next line
                listingFile << setfill(' ') << setw(8) << left << location
                    << setw(8) << left << label
                    << setw(8) << left << opcode
                    << setw(10) << left << operand
                    << setw(8) << left << objcode << endl;
                continue;
            }
            //this will turn format 2 object code into 2 bytes, then it will turn it into a
            //4 digit hex string
            opcodeInt = stoi(info.opcodeHex, nullptr, 16);
            int objCode = (opcodeInt << 8) | (r1 << 4) | r2;
            stringstream objCodeStream;
            objCodeStream << hex << uppercase << setfill('0') << setw(4) << objCode;
            objcode = objCodeStream.str();
        }

        else if(format == 3 || format == 4){      
            //first two nibbles, made up by opcode & n/i flags                                            
            opcodeInt = (opcodeInt & 0xFC) | (n<<1) | i;    
            //third nibble made up by the xbpe flags                                   
            int flags = (x << 3) | (b << 2) | (p << 1) | e;

            //format 3
            if(format == 3){
                int objCode = (opcodeInt << 16) | (flags << 12) | (disp & 0xFFF);
                stringstream ss;
                ss << hex << uppercase << setfill('0') << setw(6) << objCode;
                objcode = ss.str();
            }
            //Format 4
            else{ 
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
        
    }
    //stop reading to the listingFile
    listingFile.close();

    return listingFileName; 
}