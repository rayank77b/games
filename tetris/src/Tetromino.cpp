#include "Tetromino.h"

Tetromino::Tetromino(SDL_Color col)
 : color_(col), position_{0,0} {}

const std::vector<SDL_Point>& Tetromino::getBlocks() const {
    return blocks_;
}

SDL_Color Tetromino::getColor() const {
    return color_;
}

SDL_Point Tetromino::getPosition() const {
    return position_;
}

void Tetromino::setPosition(int x, int y) {
    position_.x = x;
    position_.y = y;
}

void Tetromino::move(int dx, int dy) {
    position_.x += dx;
    position_.y += dy;
}

// einfache 90°-Rotation um (0,0)
void Tetromino::rotate() {
    for (auto& b : blocks_) {
        int x = b.x;
        b.x = -b.y;
        b.y = x;
    }
}