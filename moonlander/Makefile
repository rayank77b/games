# Makefile für SDL2 Mondlander
CXX = g++
CXXFLAGS = -Wall -std=c++14
SRC = main.cpp
OUT = moonlander
PKG = sdl2 SDL2_ttf

all:
	$(CXX) $(CXXFLAGS) -o $(OUT) $(SRC) $(shell pkg-config --cflags --libs $(PKG))

clean:
	rm -f $(OUT)
