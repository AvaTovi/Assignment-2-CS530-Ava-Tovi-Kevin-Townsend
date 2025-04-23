//Kevin did this

#include "opcodetab.h"
#include "iostream"

OpcodeTab::OpcodeTab(){ 
    //This is opcode table and it provides each instructions hex code 
    //and the format it uses
    optable["STCH"] = {"54", {3, 4}};
    optable["SUBR"] = {"94", {2}};
    optable["JSUB"] = {"48", {3, 4}};
    optable["FLOAT"] = {"C0", {1}};
    optable["LDT"] = {"74", {3, 4}};
    optable["WD"] = {"DC", {3, 4}};
    optable["SHIFTL"] = {"A4", {2}};
    optable["COMP"] = {"28", {3, 4}};
    optable["SSK"] = {"EC", {3, 4}};
    optable["STX"] = {"10", {3, 4}};
    optable["TIO"] = {"F8", {1}};
    optable["AND"] = {"40", {3, 4}};
    optable["STF"] = {"80", {3, 4}};
    optable["TD"] = {"E0", {3, 4}};
    optable["DIVR"] = {"9C", {2}};
    optable["STL"] = {"14", {3, 4}};
    optable["SHIFTR"] = {"A8", {2}};
    optable["ADDR"] = {"90", {2}};
    optable["JEQ"] = {"30", {3, 4}};
    optable["LPS"] = {"D0", {3, 4}};
    optable["FIX"] = {"C4", {1}};
    optable["LDX"] = {"04", {3, 4}};
    optable["SUBF"] = {"5C", {3, 4}};
    optable["CLEAR"] = {"B4", {2}};
    optable["MULF"] = {"60", {3, 4}};
    optable["RMO"] = {"AC", {2}};
    optable["NORM"] = {"C8", {1}};
    optable["TD"] = {"E0", {3, 4}};
    optable["DIVF"] = {"64", {3, 4}};
    optable["STSW"] = {"E8", {3, 4}};
    optable["JEQ"] = {"30", {3, 4}};
    optable["RD"] = {"D8", {3, 4}};
    optable["MULR"] = {"98", {2}};
    optable["COMPR"] = {"A0", {2}};
    optable["STI"] = {"D4", {3, 4}};
    optable["LDF"] = {"70", {3, 4}};
    optable["JLT"] = {"38", {3, 4}};
    optable["OR"] = {"44", {3, 4}};
    optable["TIX"] = {"2C", {3, 4}};
    optable["LDB"] = {"68", {3, 4}};
    optable["SUB"] = {"1C", {3, 4}};
    optable["LDL"] = {"08", {3, 4}};
    optable["STB"] = {"78", {3, 4}};
    optable["STT"] = {"84", {3, 4}};
    optable["LDA"] = {"00", {3, 4}};
    optable["JEQ"] = {"30", {3, 4}};
    optable["DIV"] = {"24", {3, 4}};
    optable["ADDF"] = {"58", {3, 4}};
    optable["FLOAT"] = {"C0", {1}};
    optable["TIXR"] = {"B8", {2}};
    optable["WD"] = {"DC", {3, 4}};
    optable["STCH"] = {"54", {3, 4}};
    optable["HIO"] = {"F4", {1}};
    optable["COMPF"] = {"88", {3, 4}};
    optable["STL"] = {"14", {3, 4}};
    optable["STSW"] = {"E8", {3, 4}};
    optable["J"] = {"3C", {3, 4}};
    optable["JEQ"] = {"30", {3, 4}};
    optable["JSUB"] = {"48", {3, 4}};
    optable["TIO"] = {"F8", {1}};
    optable["WD"] = {"DC", {3, 4}};
    optable["STX"] = {"10", {3, 4}};
            
};

OpcodeInfo OpcodeTab::getOpcodeInfo(const string& instruction){ 
    //If the instruction exists, return the opcodeinfo
    if(isInstruction(instruction)){ 
        return optable.at(instruction);
    }
    else{
         //If the instuction is not a SIC/XE instrucion throw an error
        throw runtime_error("Invalid instruction: " + instruction);
    }
};

//Checks that the instruction exists in
//the opcode table
bool OpcodeTab::isInstruction(const string& instruction){
    if(optable.find(instruction) != optable.end()){
        return true;
    }
    else{
        return false;
    }
};

    