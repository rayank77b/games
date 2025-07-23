#pragma once
#include <SDL2/SDL.h>

class Bullet {
public:
    Bullet(float x, float y, float dx, float dy);

    void update();
    void draw(SDL_Renderer* renderer) const;
    void deactivate();

    float getX() const;
    float getY() const;

private:
    float x_, y_;
    float dx_, dy_;
    float speed_;
};