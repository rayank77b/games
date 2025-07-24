#pragma once
#include <vector>
#include <SDL2/SDL.h>
#include "Enemy.h"
#include "Bullet.h"
#include "Config.h"

static Uint32 globalID = 0;

class Tower {
public:
    Tower(int x, int y);

    void update(const std::vector<Enemy>& enemies,
                std::vector<Bullet>& bullets);

    void draw(SDL_Renderer* renderer) const;

    void damage(int amount);
    int  getHealth() const;

    int getX() const;  // für Kollision
    int getY() const;

    Uint32 getID() const;

private:
    bool canShoot() const;
    Bullet shootAt(const Enemy& e);

    int x_, y_;
    int     health_;
    Uint32 lastShotTime_;
    Uint32 id_;
};
