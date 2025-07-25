// GameOverState.cpp
#include "Game.h"
#include "GameOverState.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>

void GameOverState::handleEvent(Game& g, const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN) {
        if (e.key.key == SDLK_R) g.changeState(Game::StateID ::Menu);
        if (e.key.key == SDLK_Q) g.requestQuit();
    }
}

void GameOverState::update(Game& g, float dt) {
    // Im GameOver‑State gibt es keine laufenden Updates:
    // Warten auf Eingabe im handleEvent() (R oder Q).
}

void GameOverState::render(Game& g, SDL_Renderer* r) {
    TTF_Font* font = g.getFont();
    SDL_Color white{255,255,255,255};
    auto s1 = TTF_RenderText_Blended(font, "--- GAME OVER ---", 0, {255,0,0,255});
    auto s2 = TTF_RenderText_Blended(font,
            (g.getScore()>g.getHighScore() ? "NEW HIGH SCORE!" : "Press R to retry, Q to quit"),
            0, white);
    auto s3 = TTF_RenderText_Blended(font,
            ("Score: "+std::to_string(g.getScore())).c_str(), 0, white);
    int cy = g.h()/2 - 50;
    auto draw = [&](SDL_Surface* s, int y){
        auto tex = SDL_CreateTextureFromSurface(r,s);
        SDL_Rect dst{g.w()/2 - s->w/2, y, s->w, s->h};
        SDL_FRect dstF{
            .x = static_cast<float>(dst.x),
            .y = static_cast<float>(dst.y),
            .w = static_cast<float>(dst.w),
            .h = static_cast<float>(dst.h)
        };
        if (!SDL_RenderTexture(r, tex, nullptr, &dstF)) {
            // Im Fehlerfall: SDL_GetError() auslesen
            printf("RenderTexture failed: %s\n", SDL_GetError());
        }
        SDL_DestroyTexture(tex); 
        SDL_DestroySurface(s);
    };
    draw(s1, cy);
    draw(s2, cy+40);
    draw(s3, cy+80);
}
