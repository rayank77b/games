#include "Game.h"

int main(int, char*[]) {
    Game game;
    if (!game.init("Simple Tower Defense")) return -1;
    game.run();
    // cleanUp wird im Destruktor aufgerufen
    return 0;
}