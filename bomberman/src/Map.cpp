#include "Map.h"
#include <cstdlib>
#include <ctime>

Map::Map()
 : cols_(0)
 , rows_(0)
 , tileSize_(0.f)
{
    // Zufallsquelle initialisieren
    std::srand(static_cast<unsigned>(std::time(nullptr)));
}

void Map::init(int cols, int rows, float tileSize, float softDensity) {
    cols_      = cols;
    rows_      = rows;
    tileSize_  = tileSize;
    solidBlocks_.clear();
    softBlocks_.clear();

    for (int r = 0; r < rows_; ++r) {
        for (int c = 0; c < cols_; ++c) {
            int x = static_cast<int>(c * tileSize_);
            int y = static_cast<int>(r * tileSize_);

            // Rand und regelmäßige feste Wände
            if (r == 0 || c == 0 || r == rows_ - 1 || c == cols_ - 1
             || (r % 2 == 0 && c % 2 == 0)) {
                solidBlocks_.push_back(SDL_Rect{ x, y,
                                                 int(tileSize_),
                                                 int(tileSize_) });
            }
            else {
                // Weiche Blöcke mit Wahrscheinlichkeit softDensity
                float rnd = static_cast<float>(std::rand()) / RAND_MAX;
                if (rnd < softDensity) {
                    softBlocks_.emplace_back(x, y, int(tileSize_));
                }
            }
        }
    }
}

void Map::render(SDL_Renderer* renderer) const {
    // Hintergrund (Boden)
    SDL_SetRenderDrawColor(renderer,  30,  30,  30, 255);
    SDL_RenderClear(renderer);

    // Feste Wände (grau)
    SDL_SetRenderDrawColor(renderer, 100, 100, 100, 255);
    for (const auto& rect : solidBlocks_) {
        SDL_FRect rectF = {
            float(rect.x), float(rect.y),
            float(rect.w), float(rect.h)
        };
        SDL_RenderFillRect(renderer, &rectF);
    }

    // Weiche Blöcke (braun), nur wenn nicht zerstört
    SDL_SetRenderDrawColor(renderer, 150,  75,   0, 255);
    for (const auto& blk : softBlocks_) {
        if (!blk.isDestroyed()) {
            SDL_FRect blkF = blk.getBoundsF();
            SDL_RenderFillRect(renderer, &blkF);
        }
    }
}
