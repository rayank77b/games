#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include "Enemy.h"
#include "Tower.h"
#include "Bullet.h"


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
    void writeText(std::string& txt, SDL_Color& color,const int x,const int y);
    void renderStats();

    bool loadScore(const std::string& path = "score.txt");
    bool saveScore(const std::string& path = "score.txt") const;

    SDL_Window*   window_;
    SDL_Renderer* renderer_;
    TTF_Font*     font_;
    TTF_Font*     fontBig_;
    bool          running_;

    std::vector<Enemy>  enemies_;
    std::vector<Tower>  towers_;
    std::vector<Bullet> bullets_;

    Uint32 lastSpawnTime_;
    Uint32 spawnInterval_;
    Uint32 lastMoneyTime_;
    int    score_;
    int    playMoney_;
    bool   paused_;
    bool   gameover_;
    int    scoreMax_;
};