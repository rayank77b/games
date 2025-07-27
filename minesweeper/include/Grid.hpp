#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>

constexpr int  DEFAULT_MINE_COUNT = 40; 

struct Cell {
    bool isRevealed = false;
    bool isFlagged = false;
    bool hasMine = false;
    int adjacentMines = 0;
};

class Grid {
public:
    Grid(int rows, int cols, int windowWidth, int windowHeight, TTF_Font* f);
    void draw(SDL_Renderer* renderer);
    void handleClick(int x, int y);
    void drawNumbers(SDL_Renderer* renderer);

private:
    int rows_;
    int cols_;
    int cellWidth_;
    int cellHeight_;
    TTF_Font* font;
    std::vector<std::vector<Cell>> cells_;

    void placeMines(int mineCount);
    void computeAdjacency(); 
};
