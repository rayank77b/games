#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"
#include <SDL2/SDL_ttf.h>

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title);
    void run();
    void cleanUp();

private:
    void handleEvents();
    void update();
    void render();
    void spawnEnemy();
    void renderStats();

    SDL_Window*   window_;
    SDL_Renderer* renderer_;
    TTF_Font*     font_;
    bool          running_;

    std::vector<Enemy>  enemies_;
    std::vector<Tower>  towers_;
    std::vector<Bullet> bullets_;

    Uint32 lastSpawnTime_;
    Uint32 spawnInterval_;
    Uint32 lastMoneyTime_;
    int    score_;
    int    playMoney_;
};