// === Bar.h ===
#pragma once
#include "defs.h"
#include <SDL2/SDL.h>

const int to_screen_height=100;
const int shift = 50;

class Bar {
    int px;
    int py;
    int width;
    int height;

public:
    Bar();
    
    void set(int x, int w, int h);

    int getPos() const { return px; };
    int getPosy() const { return py; };
    int getWidth() const { return width; };
    int getHeight() const { return height; };

    bool checkCollisionBorder();

    void changePosition(SDL_Keycode key);

    void render(SDL_Renderer* renderer) const;

};
