// === Game.h ===
#pragma once
#include <SDL2/SDL.h>
#include <string>
#include "Gitter.h"

const int time_delay = 100;

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
    int round;
    Gitter gitter;
    
    int screen_width;
    int screen_height;
    bool paused;
};

