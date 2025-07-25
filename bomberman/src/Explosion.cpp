#include "Explosion.h"
#include <SDL3/SDL.h>

// Konstruktor: berechnet alle Segmente (Mitte + 4 Richtungen bis range)
Explosion::Explosion(int gridX, int gridY, int range, float tileSize)
 : startTime_(SDL_GetTicks())
{
    float x0 = gridX * tileSize;
    float y0 = gridY * tileSize;

    // zentrales Segment
    segments_.push_back({ SDL_FRect{ x0,        y0,        tileSize, tileSize } });

    // in alle vier Richtungen
    for (int i = 1; i <= range; ++i) {
        segments_.push_back({ SDL_FRect{ x0,             y0 - i*tileSize, tileSize, tileSize } }); // oben
        segments_.push_back({ SDL_FRect{ x0,             y0 + i*tileSize, tileSize, tileSize } }); // unten
        segments_.push_back({ SDL_FRect{ x0 - i*tileSize, y0,              tileSize, tileSize } }); // links
        segments_.push_back({ SDL_FRect{ x0 + i*tileSize, y0,              tileSize, tileSize } }); // rechts
    }
}

void Explosion::update(float /*dt*/) {
    // aktuell keine Animation nötig
}

void Explosion::render(SDL_Renderer* r) {
    // für jedes Segment: erst äußere Flamme (Orange), dann innere Flamme (Gelb)
    for (const auto& seg : segments_) {
        // outer
        SDL_SetRenderDrawColor(r, 255, 100,   0, 255);
        SDL_RenderFillRects(r, &seg.rectF, 1);

        // inner (20% eingerückt)
        SDL_FRect inner {
            seg.rectF.x + seg.rectF.w * 0.2f,
            seg.rectF.y + seg.rectF.h * 0.2f,
            seg.rectF.w * 0.6f,
            seg.rectF.h * 0.6f
        };
        SDL_SetRenderDrawColor(r, 255, 255,   0, 255);
        SDL_RenderFillRects(r, &inner, 1);
    }
}

SDL_Rect Explosion::getBounds() const {
    // grob: Hülle um alle Segmente
    if (segments_.empty()) return SDL_Rect{0,0,0,0};
    float minx = segments_[0].rectF.x;
    float miny = segments_[0].rectF.y;
    float maxx = minx + segments_[0].rectF.w;
    float maxy = miny + segments_[0].rectF.h;
    for (const auto& seg : segments_) {
        minx = std::min(minx, seg.rectF.x);
        miny = std::min(miny, seg.rectF.y);
        maxx = std::max(maxx, seg.rectF.x + seg.rectF.w);
        maxy = std::max(maxy, seg.rectF.y + seg.rectF.h);
    }
    return SDL_Rect{ int(minx), int(miny), int(maxx-minx), int(maxy-miny) };
}

bool Explosion::finished() const {
    return (SDL_GetTicks() - startTime_) >= duration_;
}

bool Explosion::intersects(const SDL_Rect& rect) const {
    SDL_Rect segRect;
    for (const auto& seg : segments_) {
        segRect.x = int(seg.rectF.x);
        segRect.y = int(seg.rectF.y);
        segRect.w = int(seg.rectF.w);
        segRect.h = int(seg.rectF.h);
        if (SDL_HasRectIntersection(&segRect, &rect)) {
            return true;
        }
    }
    return false;
}
