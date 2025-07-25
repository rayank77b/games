// Renderer.cpp
#include "Renderer.h"

Renderer::Renderer(SDL_Window* w) {
    rnd_ = SDL_CreateRenderer(w, NULL); // the name of the rendering driver to initialize, or NULL to let SDL choose one.
}

Renderer::~Renderer() {
    if (rnd_) SDL_DestroyRenderer(rnd_);
}

SDL_Renderer* Renderer::get() const { return rnd_; }
