#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include "App.hpp"

constexpr int  DEFAULT_MINE_COUNT = 40; 

struct Cell {
    bool isRevealed = false;
    bool isFlagged = false;
    bool hasMine = false;
    int adjacentMines = 0;
};

class Grid {
public:
    Grid(int rows, int cols, int windowWidth, int windowHeight, TTF_Font* f, TTF_Font* fb);
    void draw(SDL_Renderer* renderer);
    GameState handleClick(int x, int y);
    GameState handleMouseClick(int mouseX, int mouseY, bool right);
    void drawNumbers(SDL_Renderer* renderer);

    void drawGameOver(SDL_Renderer* renderer);

    void restart();

    // Reveal the cell at (x,y). If neighborMines == 0, 
    // reveal neighbors recursively.
    void revealCell(int x, int y);

    int getCW() {return cellWidth_;};
    int getCH() {return cellHeight_;};
    
private:
    int rows_;
    int cols_;
    int cellWidth_;
    int cellHeight_;
    TTF_Font* font_;
    TTF_Font* fontBig_;
    std::vector<std::vector<Cell>> cells_;

    void placeMines(int mineCount);
    void computeAdjacency(); 
    int getMinenSum(int c, int r);
    // Helper to check bounds
    bool inBounds(int x, int y) const; 
    Cell& getMutableCell(int x, int y);
};
