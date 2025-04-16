#ifndef OPCODETAB_H
#define OPCODETAB_H

#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct OpcodeInfo{ //Container to store the information for a specific opcode
    string opcodeHex;   //Two digit opcode
    vector<int> format; //The intruction format or formats if multiple are possible e.g 3 and 4
};

class OpcodeTab{ 
    public:
    OpcodeTab(); 
    OpcodeInfo getOpcodeInfo(const string& instruction);
    bool isInstruction(const string& instruction);

    private:
    unordered_map<string, OpcodeInfo> optable;
};

#endif