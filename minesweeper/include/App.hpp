#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

class App {
public:
    App(int width, int height);
    ~App();
    bool init();
    void clear();
    void present();
    SDL_Renderer* renderer() const;
    TTF_Font* font() const;

private:
    int width_;
    int height_;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
    TTF_Font* font_ = nullptr;
};
