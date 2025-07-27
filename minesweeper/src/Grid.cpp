#include "Grid.hpp"
#include <iostream>

Grid::Grid(int rows, int cols, int windowWidth, int windowHeight)
    : rows_(rows), cols_(cols)
{
    cellWidth_  = windowWidth / cols_;
    cellHeight_ = windowHeight / rows_;
}

void Grid::draw(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);

    // vertical lines
    for (int c = 0; c <= cols_; ++c) {
        int x = c * cellWidth_;
        SDL_RenderLine(renderer, x, 0, x, rows_ * cellHeight_);
    }

    // horizontal lines
    for (int r = 0; r <= rows_; ++r) {
        int y = r * cellHeight_;
        SDL_RenderLine(renderer, 0, y, cols_ * cellWidth_, y);
    }
}

void Grid::handleClick(int x, int y) {
    int col = x / cellWidth_;
    int row = y / cellHeight_;
    if (col >= 0 && col < cols_ && row >= 0 && row < rows_) {
        std::cout << "Clicked cell: (" << row << ", " << col << ")\n";
    }
}
