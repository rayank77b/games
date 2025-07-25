#pragma once
#include <SDL3/SDL.h>

class Game;

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleEvent(Game&, const SDL_Event&) = 0;
    virtual void update     (Game&, float dt)        = 0;
    virtual void render     (Game&, SDL_Renderer*)    = 0;
};
