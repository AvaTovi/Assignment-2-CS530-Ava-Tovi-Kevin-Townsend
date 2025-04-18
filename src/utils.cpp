// utils file for helper functions
//Kevin did the utils file
#include "utils.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

//This function takes a mnemonic string as a parameter and returns
//the corresponding register number.
int getRegisterNum(const string& mem){
    static unordered_map<string, int> registers = {
        {"A", 0}, {"X", 1}, {"L", 2}, {"B", 3}, 
        {"S", 4}, {"T", 5}, {"F", 6}, {"PC", 8},
        {"SW", 9}

    };

    //Checks that the string passed is a valid register
    if(registers.find(mem) == registers.end()){
        cerr << "Invalid Register: '" << mem << "'" << endl;
        return -1;
    }
    return registers.at(mem);   
}
