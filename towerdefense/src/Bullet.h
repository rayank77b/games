#pragma once
#include <SDL2/SDL.h>

class Bullet {
public:
    Bullet(float x, float y, float dx, float dy, Uint32 id);

    void update();
    void draw(SDL_Renderer* renderer) const;
    void deactivate();

    float getX() const;
    float getY() const;

    Uint32 getID() const;

private:
    float x_, y_;
    float dx_, dy_;
    Uint32 id_;
    float speed_;
};