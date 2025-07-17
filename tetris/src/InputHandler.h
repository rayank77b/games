#pragma once
#include <SDL2/SDL.h>
#include "Tetromino.h"
#include "Board.h"

class InputHandler {
public:
    void handleInput(bool& quit, Tetromino& current, const Board& board);
};