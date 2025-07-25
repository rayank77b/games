#include "Enemy.h"
#include "Config.h"
#include <SDL3/SDL.h>
#include <cstdlib>

float Enemy::tileSize_ = 0.0f;

Enemy::Enemy(int gridX, int gridY,
             float speed,
             int maxBombs,
             Uint32 bombFuse,
             int bombRange,
             float tileSize)
 : gridX_(gridX)
 , gridY_(gridY)
 , speed_(speed)
 , maxBombs_(maxBombs)
 , activeBombs_(0)
 , lastBombTime_(0)
 , bombFuse_(bombFuse)
 , bombRange_(bombRange)
 , dead_(false)
{
    // setze einmalig die statische Kachelgröße
    tileSize_ = tileSize;
    // Initiale Pixelkoordinaten
    x_ = gridX_ * tileSize_;
    y_ = gridY_ * tileSize_;
}

// Bewegung: einfache Zufalls‑KI (nach rechts/links/oben/unten)
void Enemy::update(float dt) {
    if (dead_) return;

    // Beispiel‑KI: zufällige Richtung alle paar Frames
    static float moveAcc = 0.0f;
    moveAcc += dt;
    if (moveAcc > 1.0f) {
        int dir = std::rand() % 4;
        switch (dir) {
            case 0: ++gridX_; break; // rechts
            case 1: --gridX_; break; // links
            case 2: ++gridY_; break; // unten
            case 3: --gridY_; break; // oben
        }
        moveAcc = 0;
    }

    // Pixel‑Koordinate an Rasterposition anpassen
    x_ = gridX_ * tileSize_;
    y_ = gridY_ * tileSize_;
}

void Enemy::render(SDL_Renderer* r) {
    if (dead_) return;

    // Körper (rot)
    SDL_FRect body{
        x_ + tileSize_ * 0.1f,
        y_ + tileSize_ * 0.1f,
        tileSize_ * 0.8f,
        tileSize_ * 0.8f
    };
    SDL_SetRenderDrawColor(r, 200, 0, 0, 255);
    SDL_RenderFillRect(r, &body);

    // Augen
    SDL_FRect eye1{
        x_ + tileSize_ * 0.25f,
        y_ + tileSize_ * 0.25f,
        tileSize_ * 0.15f,
        tileSize_ * 0.15f
    };
    SDL_FRect eye2{
        x_ + tileSize_ * 0.60f,
        y_ + tileSize_ * 0.25f,
        tileSize_ * 0.15f,
        tileSize_ * 0.15f
    };
    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderFillRect(r, &eye1);
    SDL_RenderFillRect(r, &eye2);
}

SDL_Rect Enemy::getBounds() const {
    return SDL_Rect{
        int(x_), int(y_),
        int(tileSize_), int(tileSize_)
    };
}

bool Enemy::canDropBomb(Uint32 now) const {
    return !dead_
        && activeBombs_ < maxBombs_
        && (now - lastBombTime_) >= bombFuse_;
}

void Enemy::onBombDropped() {
    if (activeBombs_ < maxBombs_) {
        ++activeBombs_;
        lastBombTime_ = SDL_GetTicks();
    }
}

void Enemy::kill() {
    dead_ = true;
}

bool Enemy::isDead() const {
    return dead_;
}

bool Enemy::intersects(const SDL_Rect& rect) const {
    SDL_Rect me = getBounds();
    return SDL_HasRectIntersection(&me, &rect);
}

SDL_Point Enemy::getGridPos() const {
    return SDL_Point{ gridX_, gridY_ };
}
