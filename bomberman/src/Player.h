#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>

class Player : public Entity {
public:
    Player(int gridX, int gridY,
           float speed, int maxBombs, float tileSize);

    // Input & Update
    void handleInput(const SDL_Event& e);
    void update(float dt) override;

    // Draw & Bounds
    void render(SDL_Renderer* r) override;
    SDL_Rect getBounds() const override;

    // Bomben‑Management
    bool canDropBomb() const;
    void onBombDropped();   // Bombenanzahl erhöhen
    void onBombExploded();  // Bombenanzahl verringern

    // Power‑ups etc.
    void onPowerUp(int type);

    // Raster‑Position
    SDL_Point getGridPos() const;

private:
    int     gridX_, gridY_;
    float   x_, y_;
    float   speed_;
    int     maxBombs_;       // Max gleichzeitig
    int     activeBombs_;    // aktuell liegende
    float   tileSize_;

    int     moveX_, moveY_;
};
