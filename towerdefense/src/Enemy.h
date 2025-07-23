#pragma once
#include <SDL2/SDL.h>

class Enemy {
public:
    Enemy(int x, int y, int speed);

    void update();
    void draw(SDL_Renderer* renderer) const;
    bool isHit(float bx, float by) const;

    void damage(int amount);
    int getHealth() const { return health_; };
    int getX() const { return x_;      };
    int getY() const { return y_;      };

private:
    int x_, y_;
    int speed_;
    int health_;
};