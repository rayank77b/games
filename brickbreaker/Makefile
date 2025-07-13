CXXFLAGS = -std=c++14 -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_ttf

SRC = main.cpp Brick.cpp Bar.cpp Game.cpp Ball.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = brickbreaker

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)
	@echo "+ Build complete."

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)