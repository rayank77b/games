// === Bar.cpp ===
#include "Bar.h"
#include <cstdlib>

Bar::Bar() {
    px = 0;
    py = 0;
    width = 100;
    height = 20;
}

void Bar::set(int x, int w, int h) {
    px = x;
    py = screen_height - to_screen_height;

    width = w;
    height = h;
}

bool Bar::checkCollisionBorder() {
    // test for Left Border
    if(px <= 0) 
        return true;
    // test for right Border
    if((px+width) >= screen_width) 
        return true;
    return false;
}

void Bar::changePosition(SDL_Keycode key) {
    if (key == SDLK_LEFT) {
        px -= shift;
        if(px<0) px = 0;
    }
    if (key == SDLK_RIGHT) {
        px += shift;
        if((px+width) >= screen_width) px=screen_width-width;
    }
}

void Bar::render(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 200, 200, 0, 255);
    SDL_Rect rect = { px, py, width, height };
    SDL_RenderFillRect(renderer, &rect);
}
