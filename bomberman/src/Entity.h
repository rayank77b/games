#pragma once
#include <SDL3/SDL.h>

class Entity {
public:
    virtual ~Entity() = default;
    virtual void update(float dt) = 0;
    virtual void render(SDL_Renderer* r) = 0;
    virtual SDL_Rect getBounds() const = 0;
};
