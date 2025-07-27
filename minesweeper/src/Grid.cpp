#include "Grid.hpp"
#include <iostream>
#include <random>
#include <algorithm> // for std::shuffle
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

Grid::Grid(int rows, int cols, int windowWidth, int windowHeight, TTF_Font* f)
    : rows_(rows), cols_(cols)
{
    font = f;
    cellWidth_  = windowWidth / cols_;
    cellHeight_ = windowHeight / rows_;

    cells_.resize(rows_, std::vector<Cell>(cols_));

    placeMines(DEFAULT_MINE_COUNT);
    computeAdjacency();
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

    drawNumbers(renderer);

    // draw grid on top
    // vertical lines
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
            std::cout << "Revealed (" << row << ", " << col << ")";
            if (cell.hasMine) {
                std::cout << " → 💣 BOOM";
            } else {
                std::cout << " → " << cell.adjacentMines << " nearby mine(s)";
            }
            std::cout << '\n';
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

int Grid::getMinenSum(int r, int c  ) {
    int count = 0;
    for(int x=-1; x<=1; x++) {
        for(int y=-1; y<=1; y++) {
            if(x+r>-1 && x+r<cols_) {
                if(y+c>-1 && y+c<rows_) {
                    if (cells_[x+r][y+c].hasMine) {
                        ++count;
                    }
                }
            }
        }
    }
    return count;
}

void Grid::computeAdjacency() {
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            if (cells_[r][c].hasMine) 
                continue;
            cells_[r][c].adjacentMines = getMinenSum(r,c);
        }
    }
}

void Grid::drawNumbers(SDL_Renderer* renderer) {
    SDL_Color textColor = {0, 0, 0, 255};  // black

    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& cell = cells_[r][c];
            
            //if (cell.isRevealed && !cell.hasMine && cell.adjacentMines > 0) {
            if (!cell.hasMine && cell.adjacentMines >= 0) {
                
                std::string text = std::to_string(cell.adjacentMines);

                SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), text.size(), textColor);
                if (!surface) {
                    //SDL_Log("Failed to load surface: %s", SDL_GetError());
                    continue;
                }
                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_free(surface);
                if (!texture) continue;

                float texW = 0, texH = 0;
                SDL_GetTextureSize(texture, &texW, &texH);


                SDL_FRect dst {
                    static_cast<float>(c * cellWidth_ + cellWidth_ / 2 - texW / 2),
                    static_cast<float>(r * cellHeight_ + cellHeight_ / 2 - texH / 2),
                    static_cast<float>(texW),
                    static_cast<float>(texH)
                };

                SDL_RenderTexture(renderer, texture, nullptr, &dst);
                SDL_DestroyTexture(texture);
            }
        }
    }
}