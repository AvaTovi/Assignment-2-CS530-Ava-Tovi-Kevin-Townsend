#include "opcodetab.h"
#include "iostream"

OpcodeTab::OpcodeTab(){ //Constructor that fills in the opcode table with all of the opcodes
                        // and the respective opcodeinfo
      optable["ADD"] = {"18", {3, 4}};
      optable["ADDF"] = {"58", {3, 4}};
      optable["CLEAR"] = {"B4", {2}};
};

OpcodeInfo OpcodeTab::getOpcodeInfo(const string& instruction){ 
    if(isInstruction(instruction)){ //If the instruction exists, return the opcodeinfo
        return optable.at(instruction);
    }
    else{ //If the instuction is not a SIC/XE instrucion throw an error, stating that it's invalid
        throw runtime_error("Invalid instruction: " + instruction);
    }
};

bool OpcodeTab::isInstruction(const string& instruction){ //Checks that the instruction exists in
                                                          //the opcode table
    if(optable.find(instruction) != optable.end()){
        return true;
    }
    else{
        return false;
    }
};

    
