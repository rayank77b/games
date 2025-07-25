#include "Game.h"
#include "Renderer.h"
#include "MenuState.h"
#include "PlayState.h"
#include "GameOverState.h"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <fstream>

Game::Game()
 : win_(nullptr), rnd_(nullptr), font_(nullptr),
   state_(State::Menu), quit_(false),
   screenW_(0), screenH_(0),
   score_(0), highScore_(0)
{}

Game::~Game() { cleanUp(); }

bool Game::loadConfig() {
    if (!cfg_.load("config.ini")) return false;
    screenW_ = cfg_.getInt("ScreenWidth",1200);
    screenH_ = cfg_.getInt("ScreenHeight",1000);
    return true;
}

bool Game::init(const char* title) {
    if (!loadConfig()) return false;

    if (!SDL_Init(SDL_INIT_VIDEO|SDL_INIT_EVENTS)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return false;
    }
    if (!TTF_Init())               return false;

    //  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
    win_ = SDL_CreateWindow(title, screenW_, screenH_, 0);
    if (!win_) return false;

    rnd_ = std::make_unique<Renderer>(win_);  //  SDL_CreateRenderer
    
    font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 
                        cfg_.getInt("MenuFontSize",48));
    if (!font_) return false;

    loadHighScore();
    changeState(State::Menu);

    return true;
}

void Game::loadHighScore() {
    std::ifstream in("score.txt");
    if (!in.is_open() || !(in>>highScore_)) highScore_ = 0;
}

void Game::saveHighScore() {
    if (score_ > highScore_) {
        std::ofstream out("score.txt", std::ofstream::trunc);
        out << score_;
    }
}

void Game::changeState(State s) {
    state_ = s;
    score_ = (s==State::Menu ? 0 : score_);
    switch (s) {
      case State::Menu:     st_ = std::make_unique<MenuState>();      break;
      case State::Playing:  st_ = std::make_unique<PlayState>();      break;
      case State::GameOver: st_ = std::make_unique<GameOverState>();  break;
    }
    if (s==State::GameOver) saveHighScore();
}

void Game::requestQuit() { quit_ = true; }

int Game::w() const { return screenW_; }
int Game::h() const { return screenH_; }

TTF_Font* Game::getFont() const { return font_; }
Config&   Game::getConfig() { return cfg_; }
void      Game::increaseScore()   { ++score_; }
int       Game::getScore() const  { return score_; }
int       Game::getHighScore()const{ return highScore_; }

void Game::cleanUp() {
    saveHighScore();
    if (font_)   TTF_CloseFont(font_);
    TTF_Quit();
    if (rnd_)    rnd_.reset();
    if (win_)    SDL_DestroyWindow(win_);
    SDL_Quit();
}

void Game::run() {
    SDL_Event e;
    Uint64 last = SDL_GetTicks();
    while (!quit_) {
        Uint64 now = SDL_GetTicks();
        float delta = (now - last) / 1000.0f;
        last = now;

        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_EVENT_QUIT) quit_ = true;
            else st_->handleEvent(*this, e);
        }
        st_->update(*this, delta);

        SDL_RenderClear(rnd_->get());
        st_->render(*this, rnd_->get());
        SDL_RenderPresent(rnd_->get());
    }
}
