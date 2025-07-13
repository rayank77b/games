// === main.cpp ===
#include "Game.h"

int main() {
    Game game;

    if (!game.init("Brick Breaker")) 
        return -1;

    game.run();
    game.clean();
    return 0;
}

