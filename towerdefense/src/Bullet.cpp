#include "Bullet.h"
#include "Config.h"

Bullet::Bullet(float x, float y, float dx, float dy)
 : x_(x), y_(y), dx_(dx), dy_(dy), speed_(5.0f) {}

void Bullet::update() {
    x_ += dx_ * speed_;
    y_ += dy_ * speed_;
}

void Bullet::draw(SDL_Renderer* renderer) const {
    SDL_Rect r{ int(x_), int(y_), BULLET_SIZE, BULLET_SIZE };
    SDL_SetRenderDrawColor(renderer, 0,255,0,255);
    SDL_RenderFillRect(renderer, &r);
}

void Bullet::deactivate() {
    x_ = -100; y_ = -100;
}

float Bullet::getX() const { return x_; }
float Bullet::getY() const { return y_; }