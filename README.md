# CS 530 Assignment 2 - SIC/XE Two-Pass Assembler
# CS530-05 Systems Programming - Spring 2025
# Professor Guy Leonard

# Team Members
- Kevin Townsend, cssc0825, Red ID: 825581231
- Ava Tovi,  cssc0836, Red ID: 131423718

# Project Overview:
This project implements a two-pass assembler for the SIC/XE machine architecture. The assembler is named `essy` and reads SIC/XE `.sic` assembly files and outputs:
- A listing file `.l` showing the assembled output
- A symbol table `.st` showing label addresses
- An intermediate file `.interm` used internally between passes

# Team Member Contributions:
At the top of every file, we wrote down who worked on what. The only exceptions were `assembler.cpp` and `essy.cpp`, which we worked on together. Ava focused on `passOne`, and Kevin handled `passTwo`. Ava also did `symtab.cpp`, `symtab.h`, `assembler.h`, and this `README.md`, plus he helped plan how we’d split up the work. Kevin worked on `opcodetab.cpp`, `opcodetab.h`, `utils.cpp`, and `utils.h`. All the files have clear comments, and we used those comments not just to explain the code, but to help each other out too. We met up in person a few times to figure things out, then worked from home and left notes in the comments so the other person would know what still needed to be done. It helped us stay on the same page. We also created a repository in Github where we pushed our codes everytime we made changes to keep eachother updated. We also met again toward the end to finish up the project and start testing everything. Kevin was the one who submitted the final version.

# Tools Used
- Compiler: g++(make)
- Language: C++
- IDE: Visual Studio Code, Edoras
- Version COntrol: Git & Github

# Testing
The sample files in the Assignment#2 were not opening for us so what we did instead was we created our own  
.sic file named `test1.sic`to test the assembler. the file includes directives like START, END, BASE, RESW, WORD. It handles all the formats and incorporates the  n i x b p e flags.

# Testing Files Produced
.st: contains the labels with correct addresses
.interm: uses the correct LOCCTR per line
.l: this file contains the assembled instructions and the object code

# How to Compile and Run
- Open terminal
- type: make clean -- To clean the entire file and start fresh before make and running.
- type: make -- This makes all the files pre-testing.
- type: make run -- This runs the program and makes all the files post-testing.

