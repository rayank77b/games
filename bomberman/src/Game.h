#pragma once
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <memory>
#include <vector>
#include "Config.h"
#include "Renderer.h"
#include "GameState.h"

class Game {
public:
    enum class StateID { Menu, Playing, GameOver };

    Game();
    ~Game();
    bool init();
    void cleanup();
    void requestQuit();

    // Aufruf durch App‑Callbacks
    SDL_AppResult onEvent(SDL_Event*);
    SDL_AppResult onIterate(float dt);

    void changeState(StateID);
    SDL_Renderer* getRenderer();
    TTF_Font*     getFont();
    Config&       getConfig();
    int           getScore() const;
    void          addScore(int);

    int  w() const;
    int  h() const;

    int  getHighScore() const;

private:
    void loadHighScore();
    void saveHighScore();

    SDL_Window*           win_;
    std::unique_ptr<Renderer> rnd_;
    TTF_Font*             font_;
    Config                cfg_;
    std::unique_ptr<GameState> state_;
    StateID               curState_;
    bool                  quit_;
    
    int            screenW_, screenH_;

    int                   score_, highScore_;
};
