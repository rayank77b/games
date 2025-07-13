// === main.cpp ===
#include "Game.h"

int main() {
    Game game;

    if (!game.init("Snake Game", 1000, 800)) 
        return -1;

    game.run();
    game.clean();
    return 0;
}
