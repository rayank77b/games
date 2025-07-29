CXX = g++
CXXFLAGS = -std=c++17 -O0 -Wall -Wextra -Iinclude $(shell pkg-config --cflags sdl3)
CXXFLAGS += -I/usr/include/SDL3 -I/usr/include/SDL3_ttf
LDFLAGS  = -lSDL3 -lSDL3_ttf

SRC = $(wildcard src/*.cpp)
OBJ = $(SRC:.cpp=.o)
TARGET = minesweeper

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f src/*.o $(TARGET)

.PHONY: all clean

