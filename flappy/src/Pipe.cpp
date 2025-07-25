#include "Pipe.h"
#include <iostream>

Pipe::Pipe(float x, float gapY, float gapH, float speed)
 : x_(x), gapY_(gapY), gapH_(gapH), speed_(speed)
{}

void Pipe::update(float delta) {
    x_ -= speed_ * delta;
}

void Pipe::render(SDL_Renderer* r) {
    // obere Röhre
    SDL_SetRenderDrawColor(r, 0,125,0,255);
    SDL_FRect top{ x_, 0, W, gapY_ };
    SDL_RenderFillRect(r, &top);
    // untere Röhre
    SDL_FRect bot{ x_, gapY_+gapH_, W, 10000 };
    SDL_RenderFillRect(r, &bot);
    SDL_SetRenderDrawColor(r, 0,0,0,255);  // sonst wird alles Gruen
}

SDL_FRect Pipe::getBounds() const {
    return SDL_FRect{ x_, 0, W, gapY_}; 
}

SDL_FRect Pipe::getBoundsDown() const {
    return SDL_FRect{ x_, gapY_+gapH_, W, 1000}; 
}

bool Pipe::passed(float birdX) const {
    return x_ + W < birdX;
}

bool Pipe::isOffscreen(float w) const {
    return x_ + W < 0;
}
