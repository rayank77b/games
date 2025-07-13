#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#include "Planet.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 800;
const int GRID_SIZE = 100;
const int PLANET_SIZE = 10;

void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);  // white lines
    for(int x=0; x<SCREEN_HEIGHT; x+=GRID_SIZE) {
        SDL_RenderDrawLine(renderer, 0, x, SCREEN_WIDTH,x);
    }
    for(int y=0; y<SCREEN_WIDTH; y+=GRID_SIZE) {
        SDL_RenderDrawLine(renderer, y, 0, y, SCREEN_HEIGHT);
    }
}

int main(int argc, char* argv[]) {

    //SDL_Init(SDL_INIT_VIDEO);
    // Initialize SDL. SDL_Init will return -1 if it fails.
	if ( SDL_Init( SDL_INIT_EVERYTHING ) < 0 ) {
		std::cout << "Error initializing SDL: " << SDL_GetError() << std::endl;
		system("pause");
		// End the program
		return 1;
	}

    SDL_Window* window = SDL_CreateWindow("Simple Planet Conquer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    bool running = true;
    SDL_Event event;

    std::vector<Planet> planets;

    Uint32 lastSpawnTime = SDL_GetTicks();

    // create gitter
    drawGrid(renderer);

    // create random planets
    planets.push_back(Planet(0, 2, A, 0));
    planets.push_back(Planet(11, 7, B, 1));
    planets.push_back(Planet(5, 5, A, 1));
    planets.push_back(Planet(3, 7, C, 1));

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            } else if (event.type == SDL_MOUSEBUTTONDOWN) {
                int mx, my;
                SDL_GetMouseState(&mx, &my);
                int x = int(mx/100); 
                int y = int(my/100);
                std::cout<<"x: "<<int(mx/100)<<"  y: "<<int(my/100)<<std::endl;
                for (size_t i = 0; i < planets.size(); ++i) {
                    if(planets[i].existPlanet(x, y)) {
                        planets[i].printPlanetInfo();
                    }
                }
            }
        }

        // update all 1 Second
        if (SDL_GetTicks() - lastSpawnTime > 1000) {
            lastSpawnTime = SDL_GetTicks();
            // update
            for (size_t i = 0; i < planets.size(); ++i) {
                planets[i].update();
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
        SDL_RenderClear(renderer);

        for (size_t i = 0; i < planets.size(); ++i) {
            planets[i].draw(renderer);
        }

        drawGrid(renderer);

        SDL_RenderPresent(renderer);
        SDL_Delay(16);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
