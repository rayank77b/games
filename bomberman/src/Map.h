#pragma once
#include <SDL3/SDL.h>
#include <vector>

/**
 * Die Map verwaltet feste und weiche Blöcke in einem
 * Gitter aus `cols × rows` Zellen. Feste Blöcke (Wände)
 * sind unzerstörbar, weiche Blöcke (SoftBlocks) können
 * durch Explosionen zerstört werden.
 */
class Map {
public:
    /** Ein weicher Block, der zerstört werden kann */
    struct SoftBlock {
        SDL_Rect bounds;    ///< Pixel-Position & Größe
        bool destroyed;     ///< true sobald von Explosion getroffen

        SoftBlock(int x, int y, int size)
         : bounds{ x, y, size, size }
         , destroyed(false)
        {}

        void destroy() { destroyed = true; }
        bool isDestroyed() const { return destroyed; }
        SDL_Rect getBounds() const { return bounds; }
        SDL_FRect getBoundsF() const { return SDL_FRect{float(bounds.x),float(bounds.y), float(bounds.w), float(bounds.h)}; }
    };

    Map();

    /** 
     * cols × rows ist die Gittergröße,
     * tileSize ist die Kachelgröße in Pixeln,
     * softDensity ∈ [0,1] ist der Anteil weicher Blöcke.
     */
    void init(int cols, int rows, float tileSize, float softDensity);

    /** Leere Implementierung (kann Animationen o. Ä. aufnehmen) */
    void update(float /*dt*/) {}

    /** Zeichnet Boden, feste Wände und weiche (nicht zerstörte) Blöcke */
    void render(SDL_Renderer* renderer) const;

    int cols() const       { return cols_; }
    int rows() const       { return rows_; }
    float getTileSize() const { return tileSize_; }

    /** Zugriff auf alle weichen Blöcke für Kollision/Explosion */
    std::vector<SoftBlock>& getSoftBlocks() { return softBlocks_; }

private:
    int              cols_, rows_;
    float            tileSize_;
    std::vector<SDL_Rect> solidBlocks_;
    std::vector<SoftBlock> softBlocks_;
};
