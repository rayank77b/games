#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>

class Player : public Entity {
public:
    Player(int col, int row, int speed, int maxBombs);
    void handleInput(const SDL_Event& e);
    void update(float dt) override;
    void render(SDL_Renderer* r) override;
    SDL_Rect getBounds() const override;
    bool canDropBomb() const;
    SDL_Point getGridPos() const;
    void onPowerUp(int type);
private:
    int col_, row_;
    float x_, y_;
    int speed_;
    int maxBombs_, activeBombs_;
    float moveX_, moveY_;
    float tileSize_;
};
