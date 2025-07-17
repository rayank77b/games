#include "Board.h"

Board::Board() {
    reset();
}

void Board::reset() {
    SDL_Color bg = {0,0,0,255};
    for (auto& row : grid_)
        for (auto& c : row)
            c = bg;
}

bool Board::isInside(int x, int y) const {
    return x>=0 && x<WIDTH && y>=0 && y<HEIGHT;
}

bool Board::isOccupied(int x, int y) const {
    if (!isInside(x,y)) return true;
    SDL_Color bg = {0,0,0,255};
    return !(grid_[y][x].r==bg.r && grid_[y][x].g==bg.g && grid_[y][x].b==bg.b);
}

bool Board::canPlace(const Tetromino& t) const {
    for (auto b : t.getBlocks()) {
        int x = t.getPosition().x + b.x;
        int y = t.getPosition().y + b.y;
        if (isOccupied(x,y)) return false;
    }
    return true;
}

bool Board::moveIfPossible(Tetromino& t, int dx, int dy) const {
    t.move(dx, dy);
    if (!canPlace(t)) {
        t.move(-dx, -dy);
        return false;
    }
    return true;
}

void Board::place(const Tetromino& t) {
    for (auto b : t.getBlocks()) {
        int x = t.getPosition().x + b.x;
        int y = t.getPosition().y + b.y;
        if (isInside(x,y))
            grid_[y][x] = t.getColor();
    }
}

int Board::clearLines() {
    int cleared = 0;
    for (int y = HEIGHT-1; y>=0; --y) {
        bool full = true;
        for (int x=0; x<WIDTH; ++x)
            if (!isOccupied(x,y)) { full=false; break; }
        if (full) {
            ++cleared;
            // nach oben verschieben
            for (int yy=y; yy>0; --yy)
                grid_[yy] = grid_[yy-1];
            // oberste Zeile clearen
            SDL_Color bg={0,0,0,255};
            grid_[0].fill(bg);
            ++y; // nochmal dieselbe Zeile prüfen
        }
    }
    return cleared;
}

const SDL_Color& Board::cell(int x, int y) const {
    return grid_[y][x];
}