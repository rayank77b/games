#pragma once
#include <SDL3/SDL.h>

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
};
