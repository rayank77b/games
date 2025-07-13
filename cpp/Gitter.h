// === Gitter.h ===
#pragma once
#include <SDL2/SDL.h>
#include <array>

const int grid_count = 100;
const int grid_width = 10;

class Gitter {
public:
    void init();
    void update();
    void clearGitter();
    void clearGitterNew();
    int calculateNeighbor(int x, int y);
    void render(SDL_Renderer* renderer) const;

private:
    std::array<std::array<bool,grid_count>, grid_count> gitter;
    std::array<std::array<bool,grid_count>, grid_count> gitter_new;
};
