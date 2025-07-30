#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <chrono>
#include <iostream>
#include <string>
#include "App.hpp"
#include "Grid.hpp"

// —–––––––––––––––––––––––––––––––––––––––––––––
// Window & Grid configuration
// —–––––––––––––––––––––––––––––––––––––––––––––
constexpr int WINDOW_WIDTH  = 1000;
constexpr int WINDOW_HEIGHT = 1000;

// change these to adjust your grid
constexpr int GRID_COLS = 20;
constexpr int GRID_ROWS = 20;
constexpr int DEFAULT_MINE_COUNT = 40; 

// —–––––––––––––––––––––––––––––––––––––––––––––
// Globals
// —–––––––––––––––––––––––––––––––––––––––––––––
static App* app = nullptr;
static Grid* grid = nullptr;
std::chrono::_V2::system_clock::time_point start;
std::chrono::_V2::system_clock::time_point end;
bool messed = false;

// —–––––––––––––––––––––––––––––––––––––––––––––
// SDL3 callbacks
// —–––––––––––––––––––––––––––––––––––––––––––––
extern "C" {

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    int columns = GRID_COLS;  
    int mines   = DEFAULT_MINE_COUNT;

    // Parse parameters manually
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg == "-c" && i + 1 < argc) {
            columns = std::stoi(argv[++i]);  // take next argument as value
        }
        else if (arg == "-m" && i + 1 < argc) {
            mines = std::stoi(argv[++i]);    // take next argument as value
        }
        else {
            std::cerr << "Unknown or incomplete parameter: " << arg << "\n";
            return SDL_APP_FAILURE;
        }
    }

    app = new App(WINDOW_WIDTH, WINDOW_HEIGHT);
    if (!app->init()) return SDL_APP_FAILURE;

    grid = new Grid(columns, columns, mines, WINDOW_WIDTH, WINDOW_HEIGHT, app->font(), app->fontBig());

    start = std::chrono::high_resolution_clock::now();
    messed = false;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void* appstate) {
    if(app->gameState==GameState::RESTART) {
        grid->restart();
        app->gameState=GameState::RUN;
        start = std::chrono::high_resolution_clock::now();
        messed = false;
    } else {
        if(!messed){
            end = std::chrono::high_resolution_clock::now();
        }
        std::chrono::duration<double> duration = end - start;
        double sekunden = duration.count();
        
        app->clear();
        grid->draw(app->renderer(), sekunden);

        if( app->gameState==GameState::GAMEOVER ||
            app->gameState==GameState::YOUWON ){
            if(!messed){  // stop mess time
                messed = true;
            }
            grid->drawGameOver(app->renderer(), app->gameState==GameState::GAMEOVER, sekunden);
        }
        app->present();
    } 
    SDL_Delay(100); // ~10 FPS cap
    //if(grid->gameOver_)
    //    return SDL_APP_SUCCESS;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    if (event->type == SDL_EVENT_QUIT) 
        return SDL_APP_FAILURE;

    if (event->type == SDL_EVENT_KEY_DOWN && 
        event->key.key == SDLK_ESCAPE) 
        return SDL_APP_FAILURE;
    
    if ((app->gameState == GameState::GAMEOVER || 
        app->gameState == GameState::YOUWON) &&
        event->type == SDL_EVENT_KEY_DOWN   &&
        event->key.key == SDLK_R) {
            app->gameState = GameState::RESTART;
    }
    
    if (app->gameState == GameState::RUN) {
        if (event->type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            app->gameState = grid->handleMouseClick(
                event->button.x, 
                event->button.y, 
                event->button.button==SDL_BUTTON_RIGHT
            );  
        }
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    delete grid;
    delete app;
}

} // extern "C"