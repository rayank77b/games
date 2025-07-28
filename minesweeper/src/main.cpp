#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include "App.hpp"
#include "Grid.hpp"

// —–––––––––––––––––––––––––––––––––––––––––––––
// Window & Grid configuration
// —–––––––––––––––––––––––––––––––––––––––––––––
constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;

// change these to adjust your grid
constexpr int GRID_COLS = 16;
constexpr int GRID_ROWS = 16;

// —–––––––––––––––––––––––––––––––––––––––––––––
// Globals
// —–––––––––––––––––––––––––––––––––––––––––––––
static App* app = nullptr;
static Grid* grid = nullptr;

// —–––––––––––––––––––––––––––––––––––––––––––––
// SDL3 callbacks
// —–––––––––––––––––––––––––––––––––––––––––––––
extern "C" {

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    app = new App(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!app->init()) return SDL_APP_FAILURE;

    grid = new Grid(GRID_ROWS, GRID_COLS, WINDOW_WIDTH, WINDOW_HEIGHT, app->font());
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    app->clear();
    grid->draw(app->renderer());
    app->present();
    SDL_Delay(16); // ~60 FPS cap
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) 
        return SDL_APP_FAILURE;

    if (event->type == SDL_EVENT_KEY_DOWN && 
        event->key.key == SDLK_ESCAPE) 
        return SDL_APP_FAILURE;
    
    if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
        int mx = event->button.x;
        int my = event->button.y;
        bool right = false;
        if(event->button.button==SDL_BUTTON_RIGHT)  right = true;
        grid->handleMouseClick(event->button.x, event->button.y, right);  
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    delete grid;
    delete app;
}

} // extern "C"