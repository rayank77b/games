#include "Bomb.h"
#include <SDL3/SDL.h>

// statisches Member initialisieren
float Bomb::tileSize_ = 0.0f;

Bomb::Bomb(int gridX, int gridY, Uint32 fuseTime, int range)
 : gridX_(gridX)
 , gridY_(gridY)
 , x_(gridX * tileSize_)
 , y_(gridY * tileSize_)
 , fuseTime_(fuseTime)
 , range_(range)
 , dropTime_(SDL_GetTicks())
 , hasExploded_(false)
{}

void Bomb::update(float /*dt*/) {
    // Aktuell keine Animation, könnte hier z.B. Blinken einstöpseln
}

void Bomb::render(SDL_Renderer* r) {
    if (hasExploded_) return;

    // Bombenkörper (dunkel)
    SDL_FRect body{
        x_ + tileSize_ * 0.1f,
        y_ + tileSize_ * 0.1f,
        tileSize_ * 0.8f,
        tileSize_ * 0.8f
    };
    SDL_SetRenderDrawColor(r, 50, 50, 50, 255);
    SDL_RenderFillRect(r, &body);

    // “Zündschnur”
    int cx = int(x_ + tileSize_ / 2);
    int cy = int(y_);
    SDL_SetRenderDrawColor(r, 200, 50, 50, 255);
    SDL_RenderLine(
        r,
        cx, cy,
        cx, cy - int(tileSize_ * 0.3f)
    );
}

SDL_Rect Bomb::getBounds() const {
    return SDL_Rect{
        int(x_), int(y_),
        int(tileSize_), int(tileSize_)
    };
}

bool Bomb::shouldExplode(Uint32 now) const {
    return !hasExploded_ && (now - dropTime_) >= fuseTime_;
}

bool Bomb::exploded(Uint32 now) const {
    // Kleines Delay, damit PlayState die Bombe noch erkennt
    return hasExploded_ && (now - dropTime_) >= (fuseTime_ + 100);
}

Explosion Bomb::explode() {
    hasExploded_ = true;
    // Explosion centered on this grid cell
    return Explosion(gridX_, gridY_, range_, tileSize_);
}
