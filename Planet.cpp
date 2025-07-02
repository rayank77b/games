// Planet.cpp
#include "Planet.h"

Planet::Planet(int x, int y, int count, int production)
    : pos_x(x), pos_y(y), count_ships(count), production_ships(production) {}

void Planet::update() {
    count_ships += production_ships;
}

void Planet::draw(SDL_Renderer* renderer) const {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, pos_x + dx, pos_y + dy);
            }
        }
    }
}

int Planet::getShipCount() const {
    return count_ships;
}

void Planet::setShipCount(int count) {
    count_ships = count;
}

int Planet::getX() const {
    return pos_x;
}

int Planet::getY() const {
    return pos_y;
}