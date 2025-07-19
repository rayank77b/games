// === Game.h ===
#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Snake.h"
#include "Food.h"
#include "TextRenderer.h"

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
    TextRenderer textRenderer;
    int score;
    bool paused;
};

