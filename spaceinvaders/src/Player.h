// Player.h
#pragma once
#include <SDL2/SDL.h>
#include "Bullet.h"

class Player {
public:
    Player(int x,int y,int speed,int bSpeed, Uint32 shootInt);
    void handleEvent(const SDL_Event& e);
    void update(int screenWidth, int screenHeight);;
    void draw(SDL_Renderer* rdr) const;
    bool canShoot(Uint32 now) const;
    Bullet shoot(Uint32 now);
    float getX() const;
    float getY() const;
    
    static constexpr int W=60, H=20;

private:
    int x_, y_, speed_;
    int bSpeed_;
    Uint32 lastShot_, shootInterval_;
    bool movLeft_, movRight_;
};
