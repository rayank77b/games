# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++14 `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Source and object files
SRC = PlanetConquer.cpp Planet.cpp
OBJ = $(SRC:.cpp=.o)
EXEC = PlanetConquer

# Default target
all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build files
clean:
	rm -f $(OBJ) $(EXEC)