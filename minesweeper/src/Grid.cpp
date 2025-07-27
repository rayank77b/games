#include "Grid.hpp"
#include <iostream>
#include <random>
#include <algorithm> // for std::shuffle

Grid::Grid(int rows, int cols, int windowWidth, int windowHeight)
    : rows_(rows), cols_(cols)
{
    cellWidth_  = windowWidth / cols_;
    cellHeight_ = windowHeight / rows_;

    cells_.resize(rows_, std::vector<Cell>(cols_));

    placeMines(DEFAULT_MINE_COUNT);
}

void Grid::draw(SDL_Renderer* renderer) {
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& cell = cells_[r][c];

            if (cell.isRevealed) {
                if (cell.hasMine) {
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);  // red
                } else {
                    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); // light gray
                }
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

void Grid::placeMines(int mineCount) {
    std::vector<std::pair<int, int>> positions;

    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c < cols_; ++c)
            positions.emplace_back(r, c);

    // Shuffle positions randomly
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(positions.begin(), positions.end(), gen);

    for (int i = 0; i < mineCount && i < static_cast<int>(positions.size()); ++i) {
        auto [r, c] = positions[i];
        cells_[r][c].hasMine = true;
    }

    std::cout << "Mines placed: " << std::min(mineCount, (int)positions.size()) << '\n';
}
