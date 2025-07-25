#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include "Config.h"
#include "GameState.h"

class Game {
public:
    enum class State { Menu, Playing, GameOver };

    Game();
    ~Game();

    bool init(const char* title);
    void run();
    void changeState(State);
    void requestQuit();
    int  w() const;
    int  h() const;
    TTF_Font* getFont() const;
    Config&   getConfig();
    void increaseScore();
    int  getScore() const;
    int  getHighScore() const;

private:
    bool loadConfig();
    void loadHighScore();
    void saveHighScore();
    void cleanUp();

    SDL_Window*    win_;
    std::unique_ptr<class Renderer> rnd_;
    TTF_Font*      font_;
    Config         cfg_;

    State          state_;
    std::unique_ptr<GameState> st_;

    bool           quit_;
    int            screenW_, screenH_;

    int            score_, highScore_;
};
