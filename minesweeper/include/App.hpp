#pragma once
#include <SDL3/SDL.h>

class App {
public:
    App(int width, int height);
    ~App();
    bool init();
    void clear();
    void present();
    SDL_Renderer* renderer() const;

private:
    int width_;
    int height_;
    SDL_Window* window_ = nullptr;
    SDL_Renderer* renderer_ = nullptr;
};
