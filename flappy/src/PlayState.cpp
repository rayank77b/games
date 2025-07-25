// PlayState.cpp
#include "PlayState.h"
#include "Game.h"
#include <SDL3_ttf/SDL_ttf.h>
#include <cstdlib>
#include <algorithm>
#include <iostream>

PlayState::PlayState()
 : bird_(100, 500, 800, 350)
 , accumulator_(0)
 , lastSpawn_(0)
{
}

// in init der PlayState setzen wir bird_ gravity/jump
void PlayState::handleEvent(Game& g, const SDL_Event& e) {
    if (e.type == SDL_EVENT_KEY_DOWN && e.key.key == SDLK_SPACE)
        bird_.flap();
}

void PlayState::update(Game& g, float delta) {
    auto& cfg = g.getConfig();

    bird_.update(delta);

    // neue Pipes spawnen
    accumulator_ += delta*1000;
    if (accumulator_ >= cfg.getInt("PipeSpawnInterval",1500)) {
        int gapY = rand() % (g.h() - cfg.getInt("PipeGap",250) - 200) + 100;
        pipes_.emplace_back(
            g.w(),
            gapY,
            cfg.getInt("PipeGap",250),
            cfg.getFloat("PipeSpeed",200)
        );
        accumulator_ = 0;
    }

    // alle Entities updaten
    for (auto& p : pipes_) p.update(delta);

    SDL_FRect birdRect = bird_.getBounds();
        
    // Kollision
    for (auto& p : pipes_) {
        SDL_FRect pipeRectTop = p.getBounds();
        SDL_FRect pipeRectDown = p.getBoundsDown();
        if (SDL_HasRectIntersectionFloat(&birdRect, &pipeRectTop)
            || SDL_HasRectIntersectionFloat(&birdRect, &pipeRectDown)) {
            g.changeState(Game::State::GameOver);
            return;
        }
    }
    // Offscreen Pipes löschen und Punkte zählen
    pipes_.erase(
      std::remove_if(pipes_.begin(), pipes_.end(),
        [&](const Pipe& p){
          if (p.passed(bird_.getBounds().x)) {
              g.increaseScore();
              return true;
          }
          return p.isOffscreen(g.w());
        }),
      pipes_.end()
    );
}

void PlayState::render(Game& g, SDL_Renderer* r) {
    
    bird_.render(r);

    for (auto& p : pipes_) p.render(r);

    // Score oben
    TTF_Font* font = g.getFont();
    auto txt = "Score: " + std::to_string(g.getScore());
    SDL_Color white{255,255,255,255};
    auto surf = TTF_RenderText_Blended(font, txt.c_str(), 0, white);
    auto tex  = SDL_CreateTextureFromSurface(r, surf);
    SDL_FRect dstF{10,10, float(surf->w), float(surf->h)};
    
    if (!SDL_RenderTexture(r, tex, nullptr, &dstF)) {
        // Im Fehlerfall: SDL_GetError() auslesen
        printf("RenderTexture failed: %s\n", SDL_GetError());
    }

    SDL_DestroySurface(surf); 
    SDL_DestroyTexture(tex);
}

