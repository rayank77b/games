#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>
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
    void draw(SDL_Renderer* renderer, const double& sekunden);
    GameState handleLeftClick(const int& x, const int& y);
    GameState handleMouseClick(const int& mouseX, const int& mouseY, const bool& right);
    void drawNumbers(SDL_Renderer* renderer);
    void drawGameOver(SDL_Renderer* renderer, const bool&  lost, const double& sekunden);
    void drawMenuBox(SDL_Renderer* renderer, const double& sekunden);
    void restart();

    // Reveal the cell at (x,y). If neighborMines == 0, 
    // reveal neighbors recursively.
    void revealCell(const int& x, const int& y);

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

    int countCells_;
    bool cheated = false;  // for debug 

    SDL_Color textColorBlack_ = {0, 0, 0, 255};  // black

    bool debug_ = true;
    int menuHeight_ = 50;

    void placeMines(const int& mineCount);
    void computeAdjacency(); 
    int getMinenSum(const int& c, const int& r);
    // Helper to check bounds
    bool inBounds(const int& x, const int& y) const; 
    Cell& getMutableCell(const int& x, const int& y);
    void debugMe(const std::string& s);
    int getRemainMines() const;
};
