#include "App.hpp"
#include <SDL3/SDL_log.h>
#include <SDL3_ttf/SDL_ttf.h>

App::App(int width, int height)
    : width_(width), height_(height) {}

App::~App() {
    if (renderer_) SDL_DestroyRenderer(renderer_);
    if (window_) SDL_DestroyWindow(window_);

    SDL_Quit();

    if (font_) TTF_CloseFont(font_);

    TTF_Quit();
}

bool App::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }
    SDL_Log("SDL_Init OK");
    
    window_ = SDL_CreateWindow("Minesweeper - SDL3", width_, height_, SDL_WINDOW_OPENGL);
    if (!window_) return false;
    SDL_Log("SDL_CreateWindow OK");

    renderer_ = SDL_CreateRenderer(window_, NULL);
    if(renderer_==NULL) {
        SDL_Log("SDL_CreateRenderer Fail: ", SDL_GetError());
        return false;
    }
    SDL_Log("SDL_CreateRenderer OK");

    if (!TTF_Init()) {
        SDL_Log("TTF_Init failed: %s", SDL_GetError());
        return false;
    }
    SDL_Log("TTF_Init Success!");

    font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans-Bold.ttf", 16);
    if (font_==NULL) {
        SDL_Log("Failed to load font: %s", SDL_GetError());
        return false;
    } 
    SDL_Log("Fonts loaded OK");
    return true;
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

TTF_Font* App::font() const {
    return font_;
}