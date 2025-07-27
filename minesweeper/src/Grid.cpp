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

    cells_.resize(cols_, std::vector<Cell>(rows_));

    placeMines(DEFAULT_MINE_COUNT);
    computeAdjacency();
}

void Grid::draw(SDL_Renderer* renderer) {
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& cell = cells_[c][r];

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

void Grid::placeMines(int mineCount) {
    std::vector<std::pair<int, int>> positions;

    for (int r = 0; r < rows_; ++r)
        for (int c = 0; c < cols_; ++c)
            positions.emplace_back(c, r);

    // Shuffle positions randomly
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(positions.begin(), positions.end(), gen);

    for (int i = 0; i < mineCount && i < static_cast<int>(positions.size()); ++i) {
        auto [c, r] = positions[i];
        cells_[c][r].hasMine = true;
    }

    std::cout << "Mines placed: " << std::min(mineCount, (int)positions.size()) << '\n';
}

void Grid::handleClick(int x, int y) {
    int col = x / cellWidth_;
    int row = y / cellHeight_;
    if (inBounds(col, row)) {
        Cell& cell = cells_[col][row];
        if (!cell.isRevealed) {
            std::cout << "Revealed (" << col << ", " << row << ")";
            if (cell.hasMine) {
                std::cout << " → 💣 BOOM";
                cell.isRevealed = true;
            } else {
                std::cout << " → " << cell.adjacentMines << " nearby mine(s)";
                revealCell(col,row);
            }
            std::cout << '\n';
        }
    }
}

void Grid::revealCell(int x, int y) {
    // 1) Bounds check
    if (!inBounds(x, y)) return;

    Cell& cell = cells_[x][y];

    // 2) If already revealed or flagged, do nothing
    if (cell.isRevealed || cell.isFlagged) return;

    // 3) Reveal this cell
    cell.isRevealed = true;

    // 4) If it has zero neighbor mines, recurse on all 8 neighbors
    if (cell.adjacentMines == 0 && !cell.hasMine) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                // skip the center cell
                if (dx == 0 && dy == 0) continue;
                revealCell(x + dx, y + dy);
            }
        }
    }
}

int Grid::getMinenSum(int c, int r  ) {
    int count = 0;
    for(int dx=-1; dx<=1; dx++) {
        for(int dy=-1; dy<=1; dy++) {
            if ( inBounds(dx+c, dy+r) )  {
                if (cells_[dx+c][dy+r].hasMine) {
                    ++count;
                }
            }
        }
    }
    return count;
}

void Grid::computeAdjacency() {
    
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            if (cells_[c][r].hasMine) 
                continue;
            cells_[c][r].adjacentMines = getMinenSum(c,r);
        }
    }
}


void Grid::drawNumbers(SDL_Renderer* renderer) {
    SDL_Color textColor = {0, 0, 0, 255};  // black

    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& cell = cells_[c][r];
            
            if (cell.isRevealed && !cell.hasMine && cell.adjacentMines > 0) {
            //if (!cell.hasMine && cell.adjacentMines >= 0) {
                
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