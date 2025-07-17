#pragma once
#include <vector>
#include <SDL2/SDL.h>

class Tetromino {
public:
    Tetromino(SDL_Color col);
    virtual ~Tetromino() = default;

    const std::vector<SDL_Point>& getBlocks() const;
    SDL_Color getColor() const;
    SDL_Point getPosition() const;

    void setPosition(int x, int y);
    void move(int dx, int dy);
    virtual void rotate();

protected:
    std::vector<SDL_Point> blocks_;  // relative coords
    SDL_Color color_;
    SDL_Point position_;             // in Zellen-Koordinaten
};