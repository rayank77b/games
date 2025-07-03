// Planet.h
#ifndef PLANET_H
#define PLANET_H

#include <SDL2/SDL.h>
#include <iostream>

enum PLANETCLASS {A=0, B, C};
//enum PLANETCOLOR {PLAYER, ENEMY_A};

class Planet {
private:
    int pos_x;
    int pos_y;
    enum PLANETCLASS planet_class;
    int player_id;
    int count_ships;
    int production_ships;
    int radius;

public:
    Planet(int x, int y, enum PLANETCLASS pclass, int player);

    void update();
    void draw(SDL_Renderer* renderer) const;

    int getShipCount() const;
    void setShipCount(int count);

    int getX() const;
    int getY() const;

    int getPlayer() const;
    void setPlayer(int player);

    bool existPlanet(int x, int y) const;
    void printPlanetInfo();
};

#endif // PLANET_H