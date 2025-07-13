// === main.cpp ===
#include "Game.h"
#include <iostream>

int main() {
    Game game;

    if (!game.init("Game of Life", 1000, 1000)) 
        return -1;

    game.run();
    std::cout<<"End Game"<<std::endl;
    game.clean();
    return 0;
}
