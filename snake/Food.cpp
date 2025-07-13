// === Food.cpp ===
#include "Food.h"
#include <cstdlib>
#include <ctime>

void Food::spawn(int gridW, int gridH) {
    std::srand(std::time(nullptr));
    pos.x = std::rand() % gridW;
    pos.y = std::rand() % gridH;
}

void Food::render(SDL_Renderer* renderer, int gridSize) const {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    SDL_Rect rect = { pos.x * gridSize, pos.y * gridSize, gridSize, gridSize };
    SDL_RenderFillRect(renderer, &rect);
}