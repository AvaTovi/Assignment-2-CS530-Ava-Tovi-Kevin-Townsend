// symtab.h
//Ava did this
#ifndef SYMTAB_H
#define SYMTAB_H

#include <map>
#include <string>

using namespace std;

//Helper class to store public functions that are used throughout the code
class SymbolTable {
public:
    void insert(const string& label, int address);
    int getAddress(const string& label);
    void writeToFile(const string& filename);
    void printAll() const;

    //This will store each of the symbol and there addresses associated with them
private:
    map<string, int> table;
};

#endif