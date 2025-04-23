//Kevin did this

#ifndef OPCODETAB_H
#define OPCODETAB_H

#include <unordered_map>
#include <vector>
#include <string>


using namespace std;

//Container to store the information for a specific opcode
struct OpcodeInfo{
    string opcodeHex;
    //The list of the valid instruction sizes
    vector<int> format;
};

//Helper class to store public functions that are used throughout the code
class OpcodeTab{ 
    public:
    OpcodeTab(); 
    OpcodeInfo getOpcodeInfo(const string& instruction);
    bool isInstruction(const string& instruction);

    //This is going to map each item to its opcode hex and supported fromats
    private:
    unordered_map<string, OpcodeInfo> optable;
};

#endif