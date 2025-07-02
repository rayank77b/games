#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "Planet.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PLANET_SIZE = 10;

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Simple Tower Defense", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    std::vector<Planet> planets;

    Uint32 lastSpawnTime = SDL_GetTicks();

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                
            }
        }

        // create
        if (SDL_GetTicks() - lastSpawnTime > 2000) {
            // planets.push_back({0, rand() % (SCREEN_HEIGHT - ENEMY_SIZE), 2});
            lastSpawnTime = SDL_GetTicks();
        }

        // update
        for (size_t i = 0; i < planets.size(); ++i) {
            planets[i].update();
        }


        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderClear(renderer);

        for (size_t i = 0; i < planets.size(); ++i) {
            planets[i].draw(renderer);
        }

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
