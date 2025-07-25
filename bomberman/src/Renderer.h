#pragma once
#include <SDL3/SDL.h>
class Renderer {
public:
    Renderer(SDL_Window* win);
    ~Renderer();
    SDL_Renderer* get() const;
private:
    SDL_Renderer* rnd_;
};
