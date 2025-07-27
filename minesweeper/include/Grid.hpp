#pragma once
#include <SDL3/SDL.h>
#include <vector>

struct Cell {
    bool isRevealed = false;
    bool isFlagged = false;
    // Future:
    // bool hasMine = false;
    // int adjacentMines = 0;
};

class Grid {
public:
    Grid(int rows, int cols, int windowWidth, int windowHeight);
    void draw(SDL_Renderer* renderer);
    void handleClick(int x, int y);

private:
    int rows_;
    int cols_;
    int cellWidth_;
    int cellHeight_;
    std::vector<std::vector<Cell>> cells_;
};
