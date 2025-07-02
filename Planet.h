// Planet.h
#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>

class Planet {
private:
    int pos_x;
    int pos_y;
    int count_ships;
    int production_ships;
    static const int radius = 20;

public:
    Planet(int x, int y, int count, int production);

    void update();
    void draw(SDL_Renderer* renderer) const;

    int getShipCount() const;
    void setShipCount(int count);

    int getX() const;
    int getY() const;
};

#endif // PLANET_H