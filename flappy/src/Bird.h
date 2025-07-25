#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>

class Bird : public Entity {
public:
    Bird(float x, float y, float gravity, float jumpVel);
    void flap();
    void update(float delta) override;
    void render(SDL_Renderer* rdr) override;
    SDL_FRect getBounds() const override;

    static constexpr float W = 40.0, H = 30.0;

private:
    float x_, y_;
    float velocity_;
    float gravity_;
    float jumpVel_;
};
