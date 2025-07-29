#include "Grid.hpp"
#include <iostream>
#include <random>
#include <algorithm> // for std::shuffle
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

Grid::Grid(int rows, int cols, int windowWidth, int windowHeight, TTF_Font* f, TTF_Font* fb)
    : rows_(rows), cols_(cols)
{
    font_ = f;
    fontBig_ = fb;
    cellWidth_  = windowWidth / cols_;
    cellHeight_ = windowHeight / rows_;

    cells_.resize(cols_, std::vector<Cell>(rows_));

    placeMines(DEFAULT_MINE_COUNT);
    computeAdjacency();
    countCells_ = 0;
}

void Grid::restart() {
    std::cout<<"Restart the game...\n";
    // here we restart our game.
    for (int r = 0; r < rows_; ++r){ 
        for (int c = 0; c < cols_; ++c) {
            cells_[c][r].adjacentMines = 0;
            cells_[c][r].isFlagged = false;
            cells_[c][r].isRevealed = false;
            cells_[c][r].hasMine = false;
        }
    }
    placeMines(DEFAULT_MINE_COUNT);
    computeAdjacency();
    countCells_ = 0;
}

void Grid::draw(SDL_Renderer* renderer) {
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            const Cell& cell = cells_[c][r];

            if (cell.isRevealed) {
                if (cell.hasMine)
                    SDL_SetRenderDrawColor(renderer, 200, 0, 0, 255);  // red
                else
                    SDL_SetRenderDrawColor(renderer, 180, 180, 180, 255); // light gray
            } else {
                if(cell.isFlagged) 
                    SDL_SetRenderDrawColor(renderer, 125, 125, 0, 255); // yellow
                else if (cell.hasMine && cheated)
                    SDL_SetRenderDrawColor(renderer, 20, 0, 0, 255);  // debug
                else 
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
    std::cout<<"\ropened cells: "<<countCells_<<std::flush;
}

void Grid::placeMines(const int& mineCount) {
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

void Grid::revealCell(const int& x, const int& y) {
    // 1) Bounds check
    if (!inBounds(x, y)) return;

    // 2) If already revealed or flagged, do nothing
    if (cells_[x][y].isRevealed || cells_[x][y].isFlagged) return;

    // 3) Reveal this cell
    cells_[x][y].isRevealed = true;
    countCells_++;

    // 4) If it has zero neighbor mines, recurse on all 8 neighbors
    if (cells_[x][y].adjacentMines == 0 && !cells_[x][y].hasMine) {
        for (int dy = -1; dy <= 1; ++dy) {
            for (int dx = -1; dx <= 1; ++dx) {
                // skip the center cell
                if (dx == 0 && dy == 0) continue;
                revealCell(x + dx, y + dy);
            }
        }
    }
}

int Grid::getMinenSum(const int& c, const int& r) {
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

Cell& Grid::getMutableCell(const int& x, const int& y) {
    return cells_[x][y];
}

bool Grid::inBounds(const int& x, const int& y) const {
    return x >= 0 && x < cols_ && y >= 0 && y < rows_;
}


GameState Grid::handleLeftClick(const int& x, const int& y) {
    int col = x / cellWidth_;
    int row = y / cellHeight_;
    if (inBounds(col, row)) {
        Cell& cell = cells_[col][row];
        if (!cell.isRevealed && !cell.isFlagged) {
            std::cout << "Revealed (" << col << ", " << row << ")";
            if (cell.hasMine) {
                std::cout << " → 💣 BOOM";
                cell.isRevealed = true;
                countCells_++;
                std::cout << '\n';
                return GameState::GAMEOVER;
            } else {
                std::cout << " → " << cell.adjacentMines << " nearby mine(s)";
                revealCell(col,row);
            }
            std::cout << '\n';
        }
    }
    return GameState::RUN;
}

GameState Grid::handleMouseClick(const int& mouseX, const int& mouseY, const bool& right) {
    int gridX = mouseX / cellWidth_;
    int gridY = mouseY / cellHeight_;
    GameState ret = GameState::RUN;

    std::cout<<"handleMouseClick("<<gridX<<"|"<<gridY<<") ";
    if(right) {  // right was clicked
        std::cout<<"RIGHT was clicked \n";
        // Toggle flag
        if (inBounds(gridX, gridY)) {
            Cell& cell = getMutableCell(gridX, gridY);
            if (!cell.isRevealed) {
                std::cout << "Flagged (" << gridX << ", " << gridY << ")";
                cell.isFlagged = !cell.isFlagged;
                if(cell.isFlagged)
                    countCells_++;
                else
                    countCells_--;
            }
        }
    } else  {   // left or midle was clicked
        std::cout<<"LEFT was clicked \n";
        ret = handleLeftClick(mouseX, mouseY);
    }
    if(countCells_==cols_*rows_)
        return GameState::YOUWON;
    return ret;
}

void Grid::drawNumbers(SDL_Renderer* renderer) {
    float texW = 0, texH = 0;
    
    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {           
            if (cells_[c][r].isRevealed && !cells_[c][r].hasMine 
                && cells_[c][r].adjacentMines > 0) {
                std::string text = std::to_string(cells_[c][r].adjacentMines);

                SDL_Surface* surface = TTF_RenderText_Solid(font_, text.c_str(), text.size(), textColorBlack);
                if (!surface) continue;

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                //SDL_free(surface);
                SDL_DestroySurface(surface);
                if (!texture) continue;

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

void Grid::drawGameOver(SDL_Renderer* renderer, const bool& lost, const double& sekunden) {
    SDL_Color textColor = {0, 200, 0, 255};  // green
    std::string text = "YOU WON ";
    if(lost){
        textColor = {155, 0, 0, 255};  // red
        text = "GAME OVER ";
    } 
    text = text + "                    "+std::to_string(int(sekunden))+" Sekunden Continue press R";
    SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(fontBig_, text.c_str(), text.size(), textColor,700);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    //SDL_free(surface);
    SDL_DestroySurface(surface);
    if (!texture) return;
    float texW = 0, texH = 0;
    SDL_GetTextureSize(texture, &texW, &texH);

    SDL_FRect dst {
        100.0,
        100.0,
        static_cast<float>(texW),
        static_cast<float>(texH)
    };

    SDL_RenderTexture(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
    
}