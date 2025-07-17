#include "InputHandler.h"

void InputHandler::handleInput(bool& quit, Tetromino& t, const Board& board) {
    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        if (e.type == SDL_QUIT) quit = true;
        else if (e.type == SDL_KEYDOWN) {
            switch (e.key.keysym.sym) {
                case SDLK_LEFT:
                    board.moveIfPossible(t, -1, 0);
                    break;
                case SDLK_RIGHT:
                    board.moveIfPossible(t, +1, 0);
                    break;
                case SDLK_DOWN:
                    board.moveIfPossible(t, 0, +1);
                    break;
                case SDLK_UP:
                    t.rotate();
                    if (!board.canPlace(t)) t.rotate(), t.rotate(), t.rotate();
                    break;
                case SDLK_SPACE:
                    // Hard drop
                    while (board.moveIfPossible(t, 0, +1));
                    break;
                case SDLK_ESCAPE:
                    quit = true;
                    break;
            }
        }
    }
}