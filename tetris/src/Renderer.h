#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <memory>
#include "Board.h"
#include "Tetromino.h"

class Renderer {
public:
    static const int BLOCK_SIZE   = 30;
    static const int INFO_WIDTH   = 150;
    static const int SCREEN_WIDTH = Board::WIDTH * BLOCK_SIZE + INFO_WIDTH;
    static const int SCREEN_HEIGHT= Board::HEIGHT * BLOCK_SIZE;

    Renderer();
    ~Renderer();

    bool init(const char* title, int w, int h);
    void clear();
    void drawBoard(const Board& board);
    void drawTetromino(const Tetromino& t);
    void drawNext(const std::unique_ptr<Tetromino>& next);
    void drawInfo(int score, int level, int lines);
    void present();
    void cleanUp();

private:
    SDL_Window*   window_;
    SDL_Renderer* renderer_;
    TTF_Font*     font_;

    SDL_Rect infoRect_;
};