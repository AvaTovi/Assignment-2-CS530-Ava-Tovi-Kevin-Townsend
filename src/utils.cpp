// utils file for helper functions
//Kevin did the utils file
#include "utils.h"

#include <unordered_map>

//This function takes a mnemonic string as a parameter and returns
//the corresponding register number.
int getRegisterNum(const string& mem){
    static unordered_map<string, int> registers = {
        {"A", 0}, {"X", 1}, {"L", 2}, {"B", 3}, 
        {"S", 4}, {"T", 5}, {"F", 6}, {"PC", 8},
        {"SW", 9}

    };
    
    return registers.at(mem);
    
}