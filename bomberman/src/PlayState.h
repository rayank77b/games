#pragma once
#include "GameState.h"
#include <vector>
#include <SDL3/SDL.h>
#include "Player.h"
#include "Map.h"
#include "Bomb.h"
#include "Explosion.h"
#include "Enemy.h"

class PlayState : public GameState {
public:
    PlayState();
    void handleEvent(Game& game, const SDL_Event& event) override;
    void update     (Game& game, float delta) override;
    void render     (Game& game, SDL_Renderer* renderer) override;
private:
    Player                     player_;
    Map                        map_;
    std::vector<Bomb>          bombs_;
    std::vector<Explosion>     explosions_;
    std::vector<Enemy>         enemies_;
    Uint32                     lastEnemySpawn_;
    bool                       initialized_;
};
