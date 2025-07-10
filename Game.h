// === Game.h ===
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>
#include "defs.h"
#include "Ball.h"
#include "Bar.h"
#include "Brick.h"

class Game {
public:
    Game();
    ~Game();
    bool init(const char* title);
    void run();
    void clean();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Ball ball;
    Bar bar;
    std::vector<Brick> bricks;

    int score;
    bool paused;
};
