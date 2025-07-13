// === Game.cpp ===
#include "Game.h"
#include <iostream>

Game::Game() : window(nullptr), renderer(nullptr), running(false) {}
Game::~Game() {}

bool Game::init(const char* title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) 
        return false;
    window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    screen_width = width;
    screen_height = height;

    running = (window && renderer);

    gitter.init();

    round = 0;

    paused = true;

    return running;
}

void Game::run() {
    while (running) {
        handleEvents();
        update();
        render();
        SDL_Delay(time_delay);
    }
}

void Game::handleEvents() {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) {
            running = false;
        }
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_SPACE:
                    paused = !paused;
                    break;
                default:
                    //running = false;
                    break;
            }
        }
    }
}

void Game::update() {

    if (paused) return; 
    
    round ++;
    std::cout<<"round: "<<round<<std::endl;

    gitter.update();

}

void Game::render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    gitter.render(renderer);

    SDL_RenderPresent(renderer);
}

void Game::clean() {
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}