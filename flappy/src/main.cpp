#include "Game.h"

int main() {
    Game game;
    if (!game.init("Flappy Bird")) return -1;
    game.run();
    return 0;
}
