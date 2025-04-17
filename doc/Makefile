# ========================
#   MAKEFILE FOR ESSY
# ========================

# Compiler & Flags
CXX         = g++
CXXFLAGS    = -Wall -std=c++11

# Project Files
TRG      = essy
SRC      = src
SRCS     = $(wildcard $(SRC)/*.cpp)
OBJS     = $(SRCS:.cpp=.o)

# Default target
all: $(TRG)

# Build executable
$(TRG): $(OBJS)
	@echo "🔧 Linking object files..."
	$(CXX) $(CXXFLAGS) -o $@ $(OBJS)

# Compile source files
$(SRC)/%.o: $(SRC)/%.cpp
	@echo "🔨 Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "🧹 Cleaning up..."
	rm -f $(SRC)/*.o $(TRG) *.interm *.st *.l

# Custom message
run: all
	@echo "🚀 Running test1.sic..."
	./$(TRG) test1.sic
