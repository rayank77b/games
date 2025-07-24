#include "Game.h"

int main(int, char**) {
    Game game;
    if (!game.init("Space Invaders")) return -1;
    game.run();
    return 0;
}
