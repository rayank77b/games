#include "App.hpp"
#include <SDL3/SDL_log.h>

App::App(int width, int height)
    : width_(width), height_(height) {}

App::~App() {
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);
    SDL_Quit();
}

bool App::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow("Minesweeper - SDL3", width_, height_, SDL_WINDOW_OPENGL);
    if (!window_) return false;

    renderer_ = SDL_CreateRenderer(window_, nullptr);
    return renderer_ != nullptr;
}

void App::clear() {
    SDL_SetRenderDrawColor(renderer_, 30, 30, 30, 255);
    SDL_RenderClear(renderer_);
}

void App::present() {
    SDL_RenderPresent(renderer_);
}

SDL_Renderer* App::renderer() const {
    return renderer_;
}
