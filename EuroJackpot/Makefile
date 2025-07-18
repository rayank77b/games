CXX = g++-14
CXXFLAGS = -std=c++14 -Wall -Wextra -O2

SRC = main.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = eurojackpot

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)
	@echo "+ Build complete."

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
