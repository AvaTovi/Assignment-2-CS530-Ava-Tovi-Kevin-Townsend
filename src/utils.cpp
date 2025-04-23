// utils file for helper functions
//Kevin did the utils file
#include "utils.h"

#include <iostream>
#include <unordered_map>
#include <unordered_set>

//This function looks up a register name and returns its numeric code
int getRegisterNum(const string& mem){
    static unordered_map<string, int> registers = {
        {"A", 0}/* Accumulator */, {"X", 1}/* index register */, {"L", 2}/* link register */,
        {"B", 3}/* base register */, {"S", 4}/* S register */, {"T", 5}/* T register */, 
        {"F", 6}/* Floating-point register */, {"PC", 8}/* program counter */, {"SW", 9}/* status word */

    };

    //Checks that the string passed is a valid register
    if(registers.find(mem) == registers.end()){
        cerr << "Invalid Register: '" << mem << "'" << endl;
        return -1;
    }
    return registers.at(mem);   
}