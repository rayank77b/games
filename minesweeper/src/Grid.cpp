#include "Grid.hpp"
#include <iostream>
#include <string>
#include <random>
#include <algorithm> // for std::shuffle
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

Grid::Grid(int rows, int cols, int mines, int windowWidth, int windowHeight, TTF_Font* f, TTF_Font* fb)
    : rows_(rows), cols_(cols)
{
    minesCount_ = mines;
    font_ = f;
    fontBig_ = fb;
    cellWidth_  = windowWidth / cols_;
    cellHeight_ = (windowHeight-menuHeight_) / rows_;

    cells_.resize(cols_, std::vector<Cell>(rows_));

    placeMines(minesCount_);
    computeAdjacency();
    countCells_ = 0;
}

void Grid::restart() {
    debugMe("Restart the game...\n");
    // here we restart our game.
    for (int r = 0; r < rows_; ++r){ 
        for (int c = 0; c < cols_; ++c) {
            cells_[c][r].adjacentMines = 0;
            cells_[c][r].isFlagged = false;
            cells_[c][r].isRevealed = false;
            cells_[c][r].hasMine = false;
        }
    }
    placeMines(minesCount_);
    computeAdjacency();
    countCells_ = 0;
}

void Grid::draw(SDL_Renderer* renderer, const double& sekunden) {
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
                static_cast<float>(menuHeight_ + r * cellHeight_),
                static_cast<float>(cellWidth_),
                static_cast<float>(cellHeight_)
            };
            SDL_RenderFillRect(renderer, &rect);
        }
    }

    drawNumbers(renderer);

    drawMenuBox(renderer, sekunden);

    // draw grid on top
    SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
    // vertical lines
    for (int c = 0; c <= cols_; ++c) {
        int x = c * cellWidth_;
        SDL_RenderLine(renderer, x, menuHeight_, x, rows_ * cellHeight_+ menuHeight_);
    }

    // horizontal lines
    for (int r = 0; r <= rows_; ++r) {
        int y = r * cellHeight_;
        SDL_RenderLine(renderer, 0, menuHeight_+y, cols_ * cellWidth_, menuHeight_+y);
    }
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

    debugMe("Mines placed: " + std::to_string(std::min(mineCount, (int)positions.size())));
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
    int row = (y - menuHeight_) / cellHeight_;

    if (inBounds(col, row)) {
        Cell& cell = cells_[col][row];
        if (!cell.isRevealed && !cell.isFlagged) {
            debugMe("Revealed (" + std::to_string(col) + ", " + std::to_string(row)+ ")");
            if (cell.hasMine) {
                debugMe(" → 💣 BOOM");
                cell.isRevealed = true;
                countCells_++;
                return GameState::GAMEOVER;
            } else {
                debugMe(" → " + std::to_string(cell.adjacentMines) + " nearby mine(s)");
                revealCell(col,row);
            }
        }
    }
    return GameState::RUN;
}

GameState Grid::handleMouseClick(const int& mouseX, const int& mouseY, const bool& right) {
    GameState ret = GameState::RUN;

    if(mouseY<menuHeight_)   // clicked on text output
        return ret;

    int gridX = mouseX / cellWidth_;
    //int gridY = mouseY / cellHeight_;
    int gridY = (mouseY - menuHeight_) / cellHeight_;

    std::cout<<"clicked> "<<mouseX<<":"<<mouseY<<"\n";
    debugMe("handleMouseClick("+std::to_string(gridX)+"|"+std::to_string(gridY)+") ");
    if(right) {  // right was clicked
        debugMe("RIGHT was clicked");
        // Toggle flag
        if (inBounds(gridX, gridY)) {
            Cell& cell = getMutableCell(gridX, gridY);
            if (!cell.isRevealed) {
                debugMe("Flagged (" +  std::to_string(gridX) + ", " + std::to_string(gridY) + ")");
                cell.isFlagged = !cell.isFlagged;
                if(cell.isFlagged)
                    countCells_++;
                else
                    countCells_--;
            }
        }
    } else  {   // left or midle was clicked
        debugMe("LEFT was clicked");
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

                SDL_Surface* surface = TTF_RenderText_Solid(font_, text.c_str(), 
                        text.size(), textColorBlack_);
                if (!surface) continue;

                SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
                SDL_DestroySurface(surface);
                if (!texture) continue;

                SDL_GetTextureSize(texture, &texW, &texH);

                SDL_FRect dst {
                    static_cast<float>(c * cellWidth_ + cellWidth_ / 2 - texW / 2),
                    static_cast<float>(menuHeight_ + r * cellHeight_ + cellHeight_ / 2 - texH / 2),
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

void Grid::debugMe(const std::string& s) {
    if(debug_)
        std::cout << s << '\n';

}

int Grid::getRemainMines() const {
    int count = 0;
    for (int r = 0; r < rows_; ++r) 
        for (int c = 0; c < cols_; ++c)        
            if (cells_[c][r].isFlagged)
                count++;
    
    return minesCount_-count;
}

void Grid::drawMenuBox(SDL_Renderer* renderer, const double& sekunden) {
    SDL_Color textColorWhite = {255, 255, 255, 255};  // white

    std::string text = "Fields: " + std::to_string(cols_) + "x" + std::to_string(rows_)
        + " Mines: " + std::to_string(minesCount_)
        + " Remain: " + std::to_string(getRemainMines())
        + " Time: " + std::to_string(int(sekunden)) + " seconds";

    SDL_Surface* surface = TTF_RenderText_Blended(font_, text.c_str(), text.size(), textColorWhite);
    if (!surface) return;
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_DestroySurface(surface);
    if (!texture) return;
    float texW = 0, texH = 0;
    SDL_GetTextureSize(texture, &texW, &texH);

    SDL_FRect dst {
        5.0,
        5.0,
        static_cast<float>(texW),
        static_cast<float>(texH)
    };

    SDL_RenderTexture(renderer, texture, nullptr, &dst);
    SDL_DestroyTexture(texture);
}