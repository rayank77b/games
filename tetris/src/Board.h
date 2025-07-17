#pragma once
#include <array>
#include <SDL2/SDL.h>
#include "Tetromino.h"

class Board {
public:
    static const int WIDTH  = 10;
    static const int HEIGHT = 20;

    Board();
    void reset();

    bool isInside(int x, int y) const;
    bool isOccupied(int x, int y) const;

    bool canPlace(const Tetromino& t) const;
    bool moveIfPossible(Tetromino& t, int dx, int dy) const;
    void place(const Tetromino& t);

    // räumt alle vollen Zeilen, gibt Anzahl zurück
    int clearLines();

    const SDL_Color& cell(int x, int y) const;

private:
    std::array<std::array<SDL_Color, WIDTH>, HEIGHT> grid_;
};