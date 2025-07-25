// PlayState.h
#pragma once
#include "GameState.h"
#include <vector>
#include "Bird.h"
#include "Pipe.h"

class PlayState : public GameState {
public:
    PlayState();
    void handleEvent(Game& g, const SDL_Event& e) override;
    void update    (Game& g, float delta) override;
    void render    (Game& g, SDL_Renderer* r) override;
private:
    Bird   bird_;
    std::vector<Pipe> pipes_;
    float  accumulator_;
    Uint32 lastSpawn_;
};
