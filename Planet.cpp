// Planet.cpp
#include "Planet.h"

Planet::Planet(int x, int y, enum PLANETCLASS pclass, int player) {
    pos_x = x;
    pos_y = y;
    player_id = player;

    count_ships = 20;
    production_ships = 2;
    radius = 29;
    planet_class = pclass;

    switch(planet_class) {
        case A:
            count_ships = 100;
            production_ships = 10;
            radius = 45;
            break;
        case B:
            count_ships = 50;
            production_ships = 5;
            radius = 30;
            break;
        case C:
            count_ships = 20;
            production_ships = 2;
            radius = 15;
            break;
    }
}

void Planet::update() {
    count_ships += production_ships;
}

void Planet::draw(SDL_Renderer* renderer) const {
    if(player_id==0) {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    } else {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
    }
    for (int w = 0; w < radius * 2; w++) {
        for (int h = 0; h < radius * 2; h++) {
            int dx = radius - w;
            int dy = radius - h;
            if ((dx*dx + dy*dy) <= (radius * radius)) {
                SDL_RenderDrawPoint(renderer, (pos_x*100+50) + dx, (pos_y*100+50) + dy);
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

int Planet::getPlayer() const {
    return player_id;
}

void Planet::setPlayer(int player) {
    player_id = player;
}

int Planet::getX() const {
    return pos_x;
}

int Planet::getY() const {
    return pos_y;
}

bool Planet::existPlanet(int x, int y) const {
    if((pos_x == x) && (pos_y == y))
        return true;
    return false;
}

void Planet::printPlanetInfo() {
    std::cout<<"Planet: "<<planet_class<<"   ";
    switch(planet_class) {
        case A:
            std::cout<<"CLASS A "<<std::endl;
            break;
        case B:
            std::cout<<"CLASS B "<<std::endl;
            break;
        case C:
            std::cout<<"CLASS C "<<std::endl;
            break;
    }
    std::cout<<"    playerid: "<<player_id<<std::endl;
    std::cout<<"    x,y: "<<pos_x<<","<<pos_y<<std::endl;
    std::cout<<"    countships:   "<<count_ships<<std::endl;
    std::cout<<"    production_ships: "<<production_ships<<std::endl;
}