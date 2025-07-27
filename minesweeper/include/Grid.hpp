#pragma once
#include <SDL3/SDL.h>
#include <vector>

constexpr int  DEFAULT_MINE_COUNT = 40;

struct Cell {
    bool isRevealed = false;
    bool isFlagged = false;
    bool hasMine = false;
    // int adjacentMines = 0;
};

class Grid {
public:
    Grid(int rows, int cols, int windowWidth, int windowHeight);
    void draw(SDL_Renderer* renderer);
    void handleClick(int x, int y);
    void placeMines(int mineCount);

private:
    int rows_;
    int cols_;
    int cellWidth_;
    int cellHeight_;
    std::vector<std::vector<Cell>> cells_;
};
