// === Brick.cpp ===
#include "Brick.h"
#include <cstdlib>

Brick::Brick(int x, int y) {
    px = x;
    py = y;
}

void Brick::set(int x, int y) {
    px = x;
    py = y;
}

void Brick::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 100, 100, 255);
    SDL_Rect rect = { px, py, brick_width-2, brick_height-2 };
    SDL_RenderFillRect(renderer, &rect);
}