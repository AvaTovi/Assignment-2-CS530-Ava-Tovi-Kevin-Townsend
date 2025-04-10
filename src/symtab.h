// symtab.h
#ifndef SYMTAB_H
#define SYMTAB_H

#include <map>
#include <string>

using namespace std;

class SymbolTable {
public:
    void insert(const string& label, int address);
    int getAddress(const string& label);
    void writeToFile(const string& filename);

private:
    map<string, int> table;
};

#endif
