#pragma once
#include "Entity.h"
#include <SDL3/SDL.h>

/**
 * Ein einfacher KI-Gegner für Bomberman.
 * Bewegt sich entlang des Rasters, kann Bomben legen und stirbt bei Explosion.
 */
class Enemy : public Entity {
public:
    /** Legt die Kachelgröße fest (Pixel pro Zelle) */
    static void setTileSize(float ts) { tileSize_ = ts; }

    /**
     * @param gridX      Spaltenindex im Raster
     * @param gridY      Zeilenindex im Raster
     * @param speed      Bewegungsgeschwindigkeit in Pixel/s
     * @param maxBombs   max. gleichzeitig ablegbare Bomben
     * @param bombFuse   ms bis zur Explosion
     * @param bombRange  Reichweite der Bombe in Zellen
     * @param tileSize   Pixelgröße einer Rasterkachel
     */
    Enemy(int gridX, int gridY,
          float speed,
          int maxBombs,
          Uint32 bombFuse,
          int bombRange,
          float tileSize);

    // Entity‑Interface
    void update(float dt) override;
    void render(SDL_Renderer* renderer) override;
    SDL_Rect getBounds() const override;

    // Bomben‑Management
    bool canDropBomb(Uint32 now) const;

    /** Merkt sich den Zeitpunkt, ab dem der nächste Bombenwurf möglich ist */
    void onBombDropped();

    /** Markiert den Gegner als tot */
    void kill();
    bool isDead() const;

    /** Testet Kollision mit einem Rechteck (z.B. Explosion) */
    bool intersects(const SDL_Rect& rect) const;

    /** Gibt die aktuelle Rasterposition zurück */
    SDL_Point getGridPos() const;

private:
    int     gridX_, gridY_;
    float   x_, y_;
    float   speed_;
    int     maxBombs_, activeBombs_;
    Uint32  lastBombTime_, bombFuse_;
    int     bombRange_;
    bool    dead_;

    static float tileSize_;  ///< Pixelgröße einer Zelle
};