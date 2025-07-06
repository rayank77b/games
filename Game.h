// === Game.h ===
#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Snake.h"
#include "Food.h"

class Game {
public:
    Game();
    ~Game();
    bool init(const char* title, int width, int height);
    void run();
    void clean();

private:
    void handleEvents();
    void update();
    void render();

    SDL_Window* window;
    SDL_Renderer* renderer;
    bool running;
    Snake snake;
    Food food;
    int gridSize;
    int screen_width;
    int screen_height;
    TTF_Font* font;
    int score;
    bool paused;
};

