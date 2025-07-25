#include "Renderer.h"

Renderer::Renderer(SDL_Window* win) {
    rnd_ = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
}
Renderer::~Renderer() {
    if (rnd_) SDL_DestroyRenderer(rnd_);
}
SDL_Renderer* Renderer::get() const { return rnd_; }
