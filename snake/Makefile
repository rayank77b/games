CXX = g++-14
CXXFLAGS = -std=c++14 -Wall -Wextra -O2
LDFLAGS = -lSDL2 -lSDL2_ttf

SRC = main.cpp Game.cpp Snake.cpp Food.cpp TextRenderer.cpp
OBJ = $(SRC:.cpp=.o)
TARGET = snake_game
FONT = FreeSans.ttf

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)
	@echo "+ Build complete."

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)
