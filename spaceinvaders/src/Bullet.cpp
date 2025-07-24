#include "Bullet.h"

Bullet::Bullet(float x,float y,float dx,float dy,int speed)
 : x_(x), y_(y), dx_(dx), dy_(dy), speed_(speed)
{}

void Bullet::update() {
    x_ += dx_ * speed_;
    y_ += dy_ * speed_;
}

void Bullet::draw(SDL_Renderer* rdr) const {
    SDL_Rect r{ int(x_), int(y_), W, H };
    SDL_SetRenderDrawColor(rdr,255,255,0,255);
    SDL_RenderFillRect(rdr,&r);
}

bool Bullet::isOffScreen(int w,int h) const {
    return x_<0 || x_>w || y_<0 || y_>h;
}

void Bullet::deactivate() {
    x_ = -100; y_ = -100;
}

float Bullet::getX() const { return x_; }
float Bullet::getY() const { return y_; }
