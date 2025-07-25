#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>   // must include only here
#include <SDL3/SDL.h>
#include "Game.h"

// Wir übergeben hier einen Zeiger auf unser Game‑Objekt
static Game* g_app = nullptr;

extern "C" {

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv) {
    // Game initialisieren
    g_app = new Game;
    if (!g_app->init()) {
        delete g_app;
        g_app = nullptr;
        return SDL_APP_FAILURE;
    }
    *appstate = g_app;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
    Game* game = static_cast<Game*>(appstate);
    return game->onEvent(event);
}

SDL_AppResult SDL_AppIterate(void *appstate) {
    static Uint64 last = SDL_GetTicks();
    Uint64 now = SDL_GetTicks();
    float dt = (now - last) / 1000.0f;
    last = now;
    Game* game = static_cast<Game*>(appstate);
    return game->onIterate(dt);
}

void SDL_AppQuit(void *appstate, SDL_AppResult result) {
    Game* game = static_cast<Game*>(appstate);
    if (game) {
        game->cleanup();
        delete game;
    }
}
} // extern "C"
