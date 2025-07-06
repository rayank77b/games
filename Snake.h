// === Snake.h ===
#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "Point.h"

class Snake {
public:
    void init(int gridWidth, int gridHeight);
    void move();
    void changeDirection(SDL_Keycode key);
    bool checkCollision() const;
    bool checkCollision(const class Food& food) const;
    bool eats(const class Food& food);
    void render(SDL_Renderer* renderer, int gridSize) const;

private:
    std::vector<Point> body;
    Point dir;
    int gridW, gridH;
};

