#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>
#include "Explosion.h"

class Bomb : public Entity {
public:
    // Muss zu Spielstart gesetzt werden, z.B. nach Map-Init:
    // Bomb::setTileSize(map.getTileSize());
    static void setTileSize(float ts) { tileSize_ = ts; }

    // gridX/gridY in Zellen-Koordinaten, fuseTime in ms, range in Zellen
    Bomb(int gridX, int gridY, Uint32 fuseTime, int range);

    // Entity-Interface
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    SDL_Rect getBounds() const override;

    // Explosion-Logik
    bool shouldExplode(Uint32 now) const;
    bool exploded(Uint32 now)    const;
    Explosion explode();  // Liefert eine Explosion und markiert als explodiert

private:
    int     gridX_, gridY_;
    float   x_, y_;            // Pixel-Koordinaten (grid * tileSize)
    Uint32  fuseTime_;         // ms bis zur Explosion
    int     range_;            // Reichweite in Zellen
    Uint32  dropTime_;         // Zeit-Stempel beim Ablegen
    bool    hasExploded_;
    static float tileSize_;    // Zellen-Größe in Pixel
};
