#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Enemy.h"
#include "Bullet.h"
#include "Config.h"

class Tower {
public:
    Tower(int x, int y) : x_(x), y_(y), health_(TOWER_HEALTH), lastShotTime_(0) {};

    void update(const std::vector<Enemy>& enemies,
                std::vector<Bullet>& bullets);

    void draw(SDL_Renderer* renderer) const;

    void damage(int amount);
    int  getHealth() const;

    int getX() const;  // für Kollision
    int getY() const;

private:
    bool canShoot() const;
    Bullet shootAt(const Enemy& e);

    int x_, y_;
    int     health_;
    Uint32 lastShotTime_;
};
