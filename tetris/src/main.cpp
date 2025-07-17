#include "Game.h"

int main(int argc, char* argv[]) {
    Game game;
    if (!game.init("Tetris", Renderer::SCREEN_WIDTH, Renderer::SCREEN_HEIGHT)) {
        return -1;
    }
    game.run();
    game.cleanUp();
    return 0;
}