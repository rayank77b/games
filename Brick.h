// === Brick.h ===
#pragma once
#include "defs.h"
#include <SDL2/SDL.h>


class Brick {
    int px;
    int py;

public:
    Brick(int x, int y);
    
    void set(int x, int y);

    int getPos() const { return px; };
    int getPosy() const { return py; };

    void render(SDL_Renderer* renderer) const;

};