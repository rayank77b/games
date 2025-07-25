#include "Bird.h"
#include <iostream>

Bird::Bird(float x, float y, float gravity, float jumpVel)
 : x_(x), y_(y), velocity_(0),
   gravity_(gravity), jumpVel_(jumpVel)
{}

void Bird::flap() {
    velocity_ = -jumpVel_;
}

void Bird::update(float delta) {
    velocity_ += gravity_ * delta;
    y_       += velocity_ * delta;
}

void Bird::render(SDL_Renderer* r) {
    // Körper
    SDL_FRect body{ x_, y_+10, W, H-10 };
    SDL_SetRenderDrawColor(r, 255,200,0,255);
    SDL_RenderFillRect(r, &body);
    // Kopf
    SDL_FRect head{ x_+10, y_, W-20, 10 };
    SDL_SetRenderDrawColor(r, 255,230,0,255);
    SDL_RenderFillRect(r, &head);
    // Auge
    SDL_FRect eye{ x_+25, y_+5, 5, 5 };
    SDL_SetRenderDrawColor(r, 0,0,0,255);
    SDL_RenderFillRect(r, &eye);
}

SDL_FRect Bird::getBounds() const {
    return SDL_FRect{ x_, y_, W, H };
}
