// Enemy.cpp
#include "Enemy.h"
#include <SDL2/SDL.h>
#include <cmath>

Enemy::Enemy(int x,int y,int sp,int bs,Uint32 fi)
 : x_(x), y_(y), speed_(sp),
   bSpeed_(bs),
   lastFire_(0), fireInterval_(fi)
{}

void Enemy::update() {
    y_ += speed_;
}

void Enemy::draw(SDL_Renderer* r) const {
    // einfacher Alienschwarm‑Look mit mehreren Rechtecken
    SDL_SetRenderDrawColor(r, 200,0,200,255);
    SDL_Rect body{ x_, y_+10, W, H-10 };
    SDL_RenderFillRect(r,&body);
    SDL_SetRenderDrawColor(r,150,0,150,255);
    SDL_Rect head{ x_+10, y_, W-20, 10 };
    SDL_RenderFillRect(r,&head);
    // Augen
    SDL_SetRenderDrawColor(r,255,255,255,255);
    SDL_Rect eye1{ x_+12, y_+2, 5, 5 };
    SDL_Rect eye2{ x_+W-17, y_+2, 5, 5 };
    SDL_RenderFillRect(r,&eye1); SDL_RenderFillRect(r,&eye2);
}

bool Enemy::readyToFire(Uint32 now) {
    if((std::rand() % 5) == 0)
        return (now - lastFire_) >= fireInterval_;
    else
        lastFire_ = now;
    return false;
}

Bullet Enemy::fire(Uint32 now) {
    lastFire_ = now;
    // schieße gerade nach unten
    return Bullet(x_+W/2, y_+H, 0.0f, 1.0f, bSpeed_);
}

bool Enemy::isHit(const Bullet& b) const {
    float bx=b.getX(), by=b.getY();
    return bx > x_ && bx < x_+W && by > y_ && by < y_+H;
}

bool Enemy::reachedBottom(int h) const {
    return y_+H >= h;
}
