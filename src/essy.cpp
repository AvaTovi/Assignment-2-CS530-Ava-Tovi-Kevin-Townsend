// main function where it will have CLI handling

// This is the Pass 1 logic

#include <iostream>
#include <string>
#include "assembler.h"

using namespace std;

int main(int argc, char* argv[]) {
    //This function will check to make sure that the user atleast provided 
    //one .sic file as an arg
    if (argc < 2) {
        cout << "Usage: ./essy <file1.sic> <file2.sic> ..." << endl;
        return 1;
    }

    int index = 1;
    while (index < argc) {
        // This function will get the current filename and notify the user 
        //on which file is being retrieved
        string filename = argv[index];
        cout << "Assembling file: " << filename << endl;

        // This function is being used to create an assembler for the file
        Assembler assembler(filename);

        // Run Pass 1 (symbol table + address calculation)
        string intermfile = assembler.passOne();


        //So far this part will handle the output of the program ran, 
        //for example if we have 3 arg's then argc will equal 3, then 
        //argv[] 

        //Continue from here, --Do Pass 2 logic--
        //Make sure the Pass 2 line is assembler.passTwo like how I did my Pass 1

        string listingfile = assembler.passTwo(intermfile);

        index++;
    }

    //Program has been completed and now it is outputing message
    cout << "Process complete, all files have been assembled!" << endl;
    return 0;
}