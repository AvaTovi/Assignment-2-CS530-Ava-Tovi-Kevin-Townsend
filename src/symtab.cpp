// manages the SYMTAB
//Ava Tovi did the SYMTAB file

#include "symtab.h"
#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;

//This prints every symbol and the addresses of the symbols in uppercase hex
void SymbolTable::printAll() const {
    for (auto const & entry : table) {
        cout << entry.first << " " << hex << uppercase << entry.second << dec << "\n";
    }
}
//This function will add the label and the address to the symbol table
void SymbolTable::insert(const string & label, int address) {
    if (table.find(label) == table.end()) {
        table[label] = address;
    } else {
        cout << "Warning: This label already exist: " << label << endl;
    }
}

//This handles the address part for the label it is on. This will be used in the 
//Pass 2, returns negative 1 to signal there is no address and label available.
int SymbolTable::getAddress(const string & label){
    if (table.find(label) != table.end()) {
        return table[label];
    } else {
        cout << "Error: Label not found in symbol table: " << label << endl;
        return -1;
    }
}

//This will be the last process and it will simply write the contents of 
//the symtab to a file
void SymbolTable::writeToFile(const string& filename) {
    ofstream symFile(filename);

    //checks to see if the opens and if it does it will continue without
    //triggering error message
    if (symFile) {
        for (const auto & entry : table) {
            symFile
            << entry.first << " "
            << hex << uppercase
            << setw(4) << setfill('0') << entry.second
            << dec << "\n";
        }
        cout << "✅ Symbol table export complete! File saved as: " << filename << endl;
    } 
    else {
        cout << "⚠️  Unable to create symbol table file: " << filename << endl;
    }
}