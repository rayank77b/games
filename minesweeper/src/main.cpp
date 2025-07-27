#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>

// —–––––––––––––––––––––––––––––––––––––––––––––
// Window & Grid configuration
// —–––––––––––––––––––––––––––––––––––––––––––––
constexpr int WINDOW_WIDTH  = 800;
constexpr int WINDOW_HEIGHT = 600;

// change these to adjust your grid
constexpr int GRID_COLS = 16;
constexpr int GRID_ROWS = 16;

// derived constants
constexpr int CELL_WIDTH  = WINDOW_WIDTH  / GRID_COLS;
constexpr int CELL_HEIGHT = WINDOW_HEIGHT / GRID_ROWS;

// —–––––––––––––––––––––––––––––––––––––––––––––
// Globals
// —–––––––––––––––––––––––––––––––––––––––––––––
static SDL_Window*   window   = nullptr;
static SDL_Renderer* renderer = nullptr;

// —–––––––––––––––––––––––––––––––––––––––––––––
// Helper: draw the grid lines
// —–––––––––––––––––––––––––––––––––––––––––––––
void drawGrid(SDL_Renderer* rnd) {
    // choose a grid color (light gray)
    SDL_SetRenderDrawColor(rnd, 200, 200, 200, 255);

    // vertical lines
    for (int c = 0; c <= GRID_COLS; ++c) {
        int x = c * CELL_WIDTH;
        SDL_RenderLine(rnd, x, 0, x, WINDOW_HEIGHT);
    }

    // horizontal lines
    for (int r = 0; r <= GRID_ROWS; ++r) {
        int y = r * CELL_HEIGHT;
        SDL_RenderLine(rnd, 0, y, WINDOW_WIDTH, y);
    }
}

// —–––––––––––––––––––––––––––––––––––––––––––––
// SDL3 callbacks
// —–––––––––––––––––––––––––––––––––––––––––––––
extern "C" {

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    window = SDL_CreateWindow(
        "Minesweeper – SDL3",
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL
    );
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, nullptr);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    if (event->type == SDL_EVENT_QUIT) {
        return SDL_APP_FAILURE;  // Exit event loop
    }
    if (event->type == SDL_EVENT_KEY_DOWN &&
        event->key.key == SDLK_ESCAPE)
    {
        return SDL_APP_FAILURE;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    // clear to dark gray
    SDL_SetRenderDrawColor(renderer, 30, 30, 30, 255);
    SDL_RenderClear(renderer);

    // ** draw the grid **
    drawGrid(renderer);

    SDL_RenderPresent(renderer);
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window)   SDL_DestroyWindow(window);
    SDL_Quit();
}

} // extern "C"