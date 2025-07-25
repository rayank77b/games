#pragma once
#include <SDL3/SDL.h>

class Game;  // nur Vorwärtsdeklaration

class GameState {
public:
    virtual ~GameState() = default;
    virtual void handleEvent(Game&, const SDL_Event&) = 0;
    virtual void update(Game&, float delta) = 0;
    virtual void render(Game&, SDL_Renderer* rdr) = 0;
};
