CXX       := g++
CXXFLAGS  := -std=c++14 -Wall `sdl2-config --cflags` -I/usr/include/SDL2
LDFLAGS   := `sdl2-config --libs` -lSDL2_ttf

SRC_DIR   := src
SHAPES    := $(wildcard $(SRC_DIR)/shapes/*.h)
SRCS      := $(SRC_DIR)/main.cpp \
             $(SRC_DIR)/Game.cpp \
             $(SRC_DIR)/Board.cpp \
             $(SRC_DIR)/Tetromino.cpp \
             $(SRC_DIR)/Renderer.cpp \
             $(SRC_DIR)/InputHandler.cpp
OBJS      := $(SRCS:.cpp=.o)

TARGET    := tetris

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)