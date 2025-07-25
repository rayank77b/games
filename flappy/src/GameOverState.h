// GameOverState.h
#pragma once
#include "GameState.h"

class GameOverState : public GameState {
public:
    void handleEvent(Game& g, const SDL_Event& e)   override;
    void update     (Game& g, float)                override;
    void render     (Game& g, SDL_Renderer* r)      override;
};
