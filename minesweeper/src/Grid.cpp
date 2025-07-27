#include "Grid.hpp"
#include <iostream>

Grid::Grid(int rows, int cols, int windowWidth, int windowHeight)
    : rows_(rows), cols_(cols)
{
    cellWidth_  = windowWidth / cols_;
    cellHeight_ = windowHeight / rows_;

    cells_.resize(rows_, std::vector<Cell>(cols_));
}

void Grid::draw(SDL_Renderer* renderer) {
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& cell = cells_[r][c];

            if (cell.isRevealed) {
                SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255);  // light gray
            } else {
                SDL_SetRenderDrawColor(renderer, 50, 50, 50, 255);     // dark cell
            }

            SDL_FRect rect = {
                static_cast<float>(c * cellWidth_),
                static_cast<float>(r * cellHeight_),
                static_cast<float>(cellWidth_),
                static_cast<float>(cellHeight_)
            };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    // draw grid on top
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
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
        Cell& cell = cells_[row][col];
        if (!cell.isRevealed) {
            cell.isRevealed = true;
            std::cout << "Revealed cell: (" << row << ", " << col << ")\n";
        }
    }
}
