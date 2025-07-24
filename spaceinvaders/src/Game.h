// Game.h
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include "Config.h"
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"

enum class State { Playing, GameOver };

class Game {
public:
    Game();
    ~Game();
    bool init(const char* title);
    void run();
    void cleanUp();

private:
    void loadConfig();
    void loadHighScore();
    void saveHighScore();
    void reset();
    void handleEvents();
    void update();
    void render();
    void spawnWave();

    SDL_Window*   win_;
    SDL_Renderer* ren_;
    TTF_Font*     font_;

    Config        cfg_;
    int           screenW_, screenH_;

    State         state_;
    Player*       player_;
    std::vector<Enemy>        enemies_;
    std::vector<Bullet>       pBullets_;
    std::vector<Bullet>       eBullets_;

    Uint32        lastSpawn_;
    int           baseECount_;
    int           baseESpeed_;
    int           baseEBInterval_;
    Uint32        eSpawnInt_;

    int           level_;
    int           score_, highScore_;
    int           lvlEIncr_, lvlSpeedIncr_, lvlEBIntDec_;
};
