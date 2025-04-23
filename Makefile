# Compiler & Flags
CXX      = g++
CXXFLAGS = -g -Wall -std=c++11 -arch x86_64

# Where gdb lives (Homebrew path or auto‑detect)
GDB      = $(shell which gdb)

# Project Files
TRG      = essy
SRC      = src
SRCS     = $(wildcard $(SRC)/*.cpp)
OBJS     = $(SRCS:.cpp=.o)

# Default target
all: $(TRG)

# Build executable
$(TRG): $(OBJS)
	@echo " Linking object files..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files
$(SRC)/%.o: $(SRC)/%.cpp
	@echo " Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo " Cleaning up..."
	rm -f $(SRC)/*.o $(TRG) *.interm *.st *.l

# Run the assembler
run: all
	@echo " Running test1.sic..."
	./$(TRG) test1.sic

# Debug target using Homebrew’s gdb
.PHONY: debug
debug: all
	@echo " Launching gdb ($(GDB))…"
	$(GDB) --args ./$(TRG)
