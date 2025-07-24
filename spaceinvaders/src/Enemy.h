// Enemy.h
#pragma once
#include <SDL2/SDL.h>
#include "Bullet.h"

class Enemy {
public:
    Enemy(int x,int y,int speed,int bSpeed, Uint32 fireInt);
    void update();
    void draw(SDL_Renderer* renderer) const;
    bool readyToFire(Uint32 now);
    Bullet fire(Uint32 now);
    bool isHit(const Bullet& b) const;
    bool reachedBottom(int h) const;

    static constexpr int W=40, H=30;

private:
    int x_, y_, speed_;
    int bSpeed_;
    Uint32 lastFire_, fireInterval_;
};
