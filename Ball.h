// === Ball.h ===
#pragma once
#include <SDL2/SDL.h>
#include "defs.h"
#include "Pos.h"
#include "Bar.h"
#include "Brick.h"


class Ball {
    Pos pos;
    Velocity velocity;
    int radius;
    int debugtimer2;

public:
    Ball();

    void set(int x, int y, int vx, int vy, int r=20);
    void setPos(const Pos p);
    void setVelocity(const Velocity v);

    Pos getPos() const { return pos; };
    Velocity getVelocity() const { return velocity; };
    int getRadius() const { return radius; };

    void move();
    void checkCollisionBorder();
    bool checkHitDownBorder();
    void checkHitBar(const Bar bar);
    bool checkCollisionBrick(const Brick b);
    bool checkCollision(const int bx, const int by, const int bw, const int bh);

    void changeDirection(const int bx, const int by, const int bw, const int bh);

    void render(SDL_Renderer* renderer) const;

};