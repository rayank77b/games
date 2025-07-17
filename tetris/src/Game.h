#pragma once
#include <SDL2/SDL.h>
#include <memory>
#include "Board.h"
#include "Tetromino.h"
#include "Renderer.h"
#include "InputHandler.h"

class Game {
public:
    Game();
    ~Game();

    bool init(const char* title, int width, int height);
    void run();
    void cleanUp();

private:
    void spawnTetromino();
    void lockTetromino();
    void updateTiming();
    std::unique_ptr<Tetromino> generateRandomTetromino();

    Board board_;
    std::unique_ptr<Tetromino> current_;
    std::unique_ptr<Tetromino> next_;

    Renderer       renderer_;
    InputHandler   input_;

    int   score_;
    int   level_;
    int   totalLines_;

    Uint32 lastDropTime_;
    Uint32 dropInterval_;  // in ms
};