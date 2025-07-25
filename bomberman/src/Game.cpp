#include "Game.h"
#include "MenuState.h"
#include "PlayState.h"
#include "GameOverState.h"
#include <fstream>

Game::Game()
 : win_(nullptr), font_(nullptr),
   curState_(StateID::Menu),
   quit_(false), score_(0), highScore_(0) {}

Game::~Game() {
    // sorgt dafür, dass cleanup() im Destruktor aufgerufen wird
    cleanup();
}

bool Game::init() {
    if (!cfg_.load("config.ini")) return false;
    int w = cfg_.getInt("ScreenWidth",1200);
    int h = cfg_.getInt("ScreenHeight",1000);

    if (!SDL_Init(SDL_INIT_VIDEO)) return false;
    if (!TTF_Init())               return false;

    win_ = SDL_CreateWindow("Bomberman",
        w, h, 0);
    if (!win_) return false;

    rnd_  = std::make_unique<Renderer>(win_);
    font_ = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
                         cfg_.getInt("MenuFontSize",48));

    loadHighScore();
    changeState(StateID::Menu);
    return true;
}

void Game::cleanup() {
    saveHighScore();
    if (font_)   TTF_CloseFont(font_);
    TTF_Quit();
    if (rnd_)    rnd_.reset();
    if (win_)    SDL_DestroyWindow(win_);
    SDL_Quit();
}
void Game::requestQuit() { quit_ = true; }

int Game::w() const { return screenW_; }
int Game::h() const { return screenH_; }

SDL_AppResult Game::onEvent(SDL_Event* e) {
    if (e->type == SDL_EVENT_QUIT) {
        quit_ = true;
        return SDL_APP_CONTINUE;
    }
    state_->handleEvent(*this,*e);
    return quit_ ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

SDL_AppResult Game::onIterate(float dt) {
    state_->update(*this, dt);
    SDL_RenderClear(getRenderer());
    state_->render(*this, getRenderer());
    SDL_RenderPresent(getRenderer());
    return quit_ ? SDL_APP_SUCCESS : SDL_APP_CONTINUE;
}

void Game::changeState(StateID id) {
    curState_ = id;
    switch(id) {
      case StateID::Menu:     state_ = std::make_unique<MenuState>();    break;
      case StateID::Playing:  state_ = std::make_unique<PlayState>();    break;
      case StateID::GameOver: state_ = std::make_unique<GameOverState>();break;
    }
    score_ = (id==StateID::Menu ? 0 : score_);
    if (id==StateID::GameOver) saveHighScore();
}

SDL_Renderer* Game::getRenderer() { return rnd_->get(); }
TTF_Font*     Game::getFont()     { return font_; }
Config&       Game::getConfig()   { return cfg_; }
int           Game::getScore() const { return score_; }
void          Game::addScore(int v) { score_ += v; }

void Game::loadHighScore() {
    std::ifstream in("score.txt");
    if (!(in>>highScore_)) highScore_ = 0;
}

void Game::saveHighScore() {
    if (score_ > highScore_) {
        std::ofstream out("score.txt", std::ofstream::trunc);
        out<<score_;
    }
}

int  Game::getHighScore() const {
    return highScore_;
}
