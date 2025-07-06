// === Food.h ===
#pragma once
#include <SDL2/SDL.h>
#include "Point.h"

class Food {
public:
    void spawn(int gridW, int gridH);
    void render(SDL_Renderer* renderer, int gridSize) const;
    Point getPosition() const { return pos; }

private:
   Point pos;
};