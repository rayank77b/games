#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>

class Pipe : public Entity {
public:
    Pipe(float x, float gapY, float gapH, float speed);
    void update(float delta) override;
    void render(SDL_Renderer* rdr) override;
    SDL_FRect getBounds() const override;
    SDL_FRect getBoundsDown() const;
    bool passed(float birdX) const;  // für Punkteszählung
    bool isOffscreen(float screenW) const;

private:
    float x_;
    float gapY_, gapH_;
    float speed_;
    static constexpr float W = 80;
};
