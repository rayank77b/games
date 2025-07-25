#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>
#include <vector>

class Explosion : public Entity {
public:
    // Erzeugt eine Explosion auf Zelle (gridX,gridY) mit gegebener Reichweite (Zellen)
    // und der Kachelgröße (Pixel)
    Explosion(int gridX, int gridY, int range, float tileSize);

    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    SDL_Rect getBounds() const override;

    // Prüft, ob die Explosion vorbei ist
    bool finished() const;

    // Prüft Kollision mit einem Rechteck
    bool intersects(const SDL_Rect& rect) const;

private:
    struct Segment {
        SDL_FRect rectF;
    };

    std::vector<Segment> segments_;
    Uint32               startTime_;            // ms beim Erzeugen
    static constexpr Uint32 duration_ = 500;      // Leben der Explosion in ms
};
