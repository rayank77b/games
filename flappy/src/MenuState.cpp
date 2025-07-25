// MenuState.cpp
#include "MenuState.h"
#include "Game.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void MenuState::handleEvent(Game& g, const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key== SDLK_SPACE) {
        g.changeState(Game::State::Playing);
    }
}

void MenuState::render(Game& g, SDL_Renderer* r) {
    // Titel
    TTF_Font* font = g.getFont();
    auto txt1 = "FLAPPY BIRD";
    SDL_Color white{255,255,255,255};
    auto surf1 = TTF_RenderText_Blended(font, txt1,0, white);
    auto tex1  = SDL_CreateTextureFromSurface(r, surf1);
    SDL_Rect d1{ g.w()/2 - surf1->w/2, 200, surf1->w, surf1->h };
    SDL_FRect dstF{
        .x = static_cast<float>(d1.x),
        .y = static_cast<float>(d1.y),
        .w = static_cast<float>(d1.w),
        .h = static_cast<float>(d1.h)
    };
    if (!SDL_RenderTexture(r, tex1, nullptr, &dstF)) {
        // Im Fehlerfall: SDL_GetError() auslesen
        printf("RenderTexture failed: %s\n", SDL_GetError());
    }
    SDL_DestroySurface(surf1); 
    SDL_DestroyTexture(tex1);
    // Anweisung
    auto txt2 = "Press SPACE to start";
    auto surf2 = TTF_RenderText_Blended(font, txt2,0, white);
    auto tex2  = SDL_CreateTextureFromSurface(r, surf2);
    SDL_Rect d2{ g.w()/2 - surf2->w/2, 400, surf2->w, surf2->h };
    dstF = {
        .x = static_cast<float>(d2.x),
        .y = static_cast<float>(d2.y),
        .w = static_cast<float>(d2.w),
        .h = static_cast<float>(d2.h)
    };
    if (!SDL_RenderTexture(r, tex2, nullptr, &dstF)) {
        // Im Fehlerfall: SDL_GetError() auslesen
        printf("RenderTexture failed: %s\n", SDL_GetError());
    }
    SDL_DestroySurface(surf2); 
    SDL_DestroyTexture(tex2);
}
